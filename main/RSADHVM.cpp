//---------------------------------------------------------------------------

#include <vcl.h>

#pragma hdrstop

#include "RSADHVM.h"

using namespace std;

#include <stdlib.h>
#include <math.h>
#include <random>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <System.SysUtils.hpp>
#include <Vcl.Imaging.jpeg.hpp>
#include <Graphics.hpp>

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm_Main *Form_Main;
__int64 Image_Width;
__int64 Image_Height;
//---------------------------------------------------------------------------
__fastcall TForm_Main::TForm_Main(TComponent* Owner)
	: TForm(Owner)
{
	//DBGrid for input image
	ClientDataSet1->FieldDefs->Add("X", ftInteger);
	ClientDataSet1->FieldDefs->Add("Y", ftInteger);
	ClientDataSet1->FieldDefs->Add("R", ftInteger);
	ClientDataSet1->FieldDefs->Add("G", ftInteger);
	ClientDataSet1->FieldDefs->Add("B", ftInteger);
	ClientDataSet1->CreateDataSet();
	DataSource1->DataSet = ClientDataSet1;
	DBGrid1->DataSource = DataSource1;

	//DBGrid for output image
	ClientDataSet2->FieldDefs->Add("X", ftInteger);
	ClientDataSet2->FieldDefs->Add("Y", ftInteger);
	ClientDataSet2->FieldDefs->Add("R", ftInteger);
	ClientDataSet2->FieldDefs->Add("G", ftInteger);
	ClientDataSet2->FieldDefs->Add("B", ftInteger);
	ClientDataSet2->CreateDataSet();
	DataSource2->DataSet = ClientDataSet2;
	DBGrid2->DataSource = DataSource2;
}
//---------------------------------------------------------------------------

void __fastcall TForm_Main::FormCreate(TObject *Sender)
{
	//Maximized window
	Form_Main->WindowState = wsMaximized;

	//Max length input
	Edit_p1->MaxLength = 10;
	Edit_p2->MaxLength = 10;
	Edit_q1->MaxLength = 10;
	Edit_q2->MaxLength = 10;
	Edit_x->MaxLength = 10;
	Edit_y->MaxLength = 10;
}
//---------------------------------------------------------------------------

void __fastcall TForm_Main::CheckEdits()
{
	SpeedButton_Encrypt->Enabled = !Edit_p1->Text.IsEmpty() && !Edit_p2->Text.IsEmpty() && !Edit_q1->Text.IsEmpty()
									&& !Edit_q2->Text.IsEmpty() && !Edit_VigenereKey->Text.IsEmpty()
									&& !Edit_x->Text.IsEmpty() && !Edit_y->Text.IsEmpty();
    SpeedButton_SaveKey->Enabled = !Edit_p1->Text.IsEmpty() && !Edit_p2->Text.IsEmpty() && !Edit_q1->Text.IsEmpty()
									&& !Edit_q2->Text.IsEmpty() && !Edit_VigenereKey->Text.IsEmpty()
									&& !Edit_x->Text.IsEmpty() && !Edit_y->Text.IsEmpty();
}
//---------------------------------------------------------------------------

void __fastcall TForm_Main::Edit_p1KeyPress(TObject *Sender, System::WideChar &Key)

{
	if (!isdigit(Key) && Key != '\b') // Allow digits and backspace
	{
		Key = 0; // Discard the key press
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm_Main::Edit_p2KeyPress(TObject *Sender, System::WideChar &Key)

{
	if (!isdigit(Key) && Key != '\b') // Allow digits and backspace
	{
		Key = 0; // Discard the key press
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm_Main::Edit_q1KeyPress(TObject *Sender, System::WideChar &Key)

{
    if (!isdigit(Key) && Key != '\b') // Allow digits and backspace
	{
		Key = 0; // Discard the key press
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm_Main::Edit_q2KeyPress(TObject *Sender, System::WideChar &Key)

{
    if (!isdigit(Key) && Key != '\b') // Allow digits and backspace
	{
		Key = 0; // Discard the key press
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm_Main::Edit_xKeyPress(TObject *Sender, System::WideChar &Key)

{
    if (!isdigit(Key) && Key != '\b') // Allow digits and backspace
	{
		Key = 0; // Discard the key press
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm_Main::Edit_yKeyPress(TObject *Sender, System::WideChar &Key)

{
    if (!isdigit(Key) && Key != '\b') // Allow digits and backspace
	{
		Key = 0; // Discard the key press
	}
}
//---------------------------------------------------------------------------





bool IsPrime(int number)
{
	if (number <= 1) {
        return false;
    }
    if (number <= 3) {
        return true;
	}
	if (number % 2 == 0 || number % 3 == 0) {
		return false;
	}
	for (int i = 5; i <= sqrt(number); i = i + 6) {
		if (number % i == 0 || number % (i + 2) == 0) {
			return false;
		}
	}
	return true;
}
//---------------------------------------------------------------------------

__int64 ExtendedGCD(__int64 a, __int64 b, __int64& x, __int64& y) {
    if (a == 0) {
        x = 0;
        y = 1;
        return b;
    }

    __int64 x1, y1;
	__int64 gcd = ExtendedGCD(b % a, a, x1, y1);

    x = y1 - (b / a) * x1;
    y = x1;

	return gcd;
}
//---------------------------------------------------------------------------

__int64 ModInverse(__int64 a, __int64 m) {
    __int64 x, y;
    __int64 gcd = ExtendedGCD(a, m, x, y);

    if (gcd != 1) {
		return -1; // Inverse doesn't exist
    } else {
        return (x % m + m) % m; // Ensure positive result
    }
}
//---------------------------------------------------------------------------

__int64 ModPow(__int64 base, __int64 exponent, __int64 modulus) {
	__int64 result = 1;
	base = base % modulus; // Update base if it's larger than or equal to modulus

	if (base == 0) {
		return 0; // In case base is 0
	}

	while (exponent > 0) {
		// If exponent is odd, multiply base with result
		if (exponent & 1)
			result = (result * base) % modulus;

		// exponent must be even now
		exponent = exponent >> 1; // exponent = exponent/2
		base = (base * base) % modulus;
	}
	return result;
}
//---------------------------------------------------------------------------

void __fastcall TForm_Main::GenerateSK(__int64 p1, __int64 q1, __int64 p2, __int64 q2, __int64 x, __int64 y, __int64& e, __int64& d, __int64& n, __int64& enc_sK)
{
	//Counting n
	__int64 n1 = p1 * q1; Edit_n1->Text = AnsiString(n1);
	__int64 n2 = p2 * q2; Edit_n2->Text = AnsiString(n2);

	//Counting totient Euler
	__int64 phi_n1 = (p1 - 1) * (q1 - 1); Edit_phi_n1->Text = AnsiString(phi_n1);
	__int64 phi_n2 = (p2 - 1) * (q2 - 1); Edit_phi_n2->Text = AnsiString(phi_n2);

	//Dummy for ExtendedGCD
	__int64 temp1, temp2, temp3, temp4;

	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<__int64> diste1(2, phi_n1);
	uniform_int_distribution<__int64> diste2(2, phi_n2);

	__int64 e1 = diste1(gen);
	__int64 e2 = diste2(gen);

	//Choosing e where gcd(e, phi_n) = 1
	while(ExtendedGCD(e1, phi_n1, temp1, temp2) != 1) {
			e1++;
	}
	Edit_e1->Text = AnsiString(e1);

	while(ExtendedGCD(e2, phi_n2, temp3, temp4) != 1) {
			e2++;
	}
	Edit_e2->Text = AnsiString(e2);

	//Counting d
	__int64 d1 = ModInverse(e1, phi_n1); Edit_d1->Text = AnsiString(d1);
	__int64 d2 = ModInverse(e2, phi_n2); Edit_d2->Text = AnsiString(d2);

	//Generate R
	uniform_int_distribution<__int64> distR1(2, e1 - 1);
	uniform_int_distribution<__int64> distR2(2, e2 - 1);

	__int64 R1 = distR1(gen); Edit_R1->Text = AnsiString(R1);
	__int64 R2 = distR2(gen); Edit_R2->Text = AnsiString(R2);

	//Diffie Hellman key exchange
	__int64 P = R1 * R2; Edit_P->Text = AnsiString(P);
	__int64 G = ModPow(n1, n2, e1 * e2); Edit_G->Text = AnsiString(G);

	__int64 A = ModPow(G, x, P); Edit_A->Text = AnsiString(A);
	__int64 B = ModPow(G, y, P); Edit_B->Text = AnsiString(B);

	__int64 K = ModPow(B, x, P); Edit_K->Text = AnsiString(K);
	__int64 K1 = ModPow(A, y, P); Edit_K1->Text = AnsiString(K1);

	//Choose RSA key (e, d, n) with random dist
	uniform_int_distribution<__int64> edn(0, 1);
	if (edn(gen) == 0) {
		e = e1;
		d = d1;
		n = n1;
	} else {
		e = e2;
		d = d2;
		n = n2;
	}
	Edit_e->Text = AnsiString(e);
	Edit_d->Text = AnsiString(d);
	Edit_n->Text = AnsiString(n);

	//Verify shared keys are equal
	if (K != K1) {
		ShowMessage("Shared keys are not the same!");
		return;
	}

	//Ensure sK is odd and < n
	__int64 sK = K % n;  //Reduce sK to be < n
	if (sK % 2 == 0) {
		sK += 1;
		if (sK >= n) {
			sK -= 2;  //Make sure still odd and < n
		}
	}

	Edit_sK->Text = AnsiString(sK);

	//Encrypting sK
	enc_sK = ModPow(sK, e, n);
	Edit_enc_sK->Text = AnsiString(enc_sK);
}
//---------------------------------------------------------------------------

String __fastcall TForm_Main::GenerateVk(String Vs, int NumOfPix)
{
	if (Vs.IsEmpty()) {
		throw invalid_argument("Vigenere key cannot be empty!");
	}

	String Vk = "";
	int VsLength = Vs.Length(); // Store the length of Vs

	for (int i = 0; i < NumOfPix; ++i) {
		Vk += Vs[i % VsLength + 1]; // Use modulo to cycle through Vs
	}

	return Vk;
}
//---------------------------------------------------------------------------




void __fastcall TForm_Main::Edit_p1Change(TObject *Sender)
{
	try {
		int num = StrToInt(Edit_p1->Text);
		if (IsPrime(num)) {
			StaticText_p1Warning->Caption = "Prime";
		} else {
			StaticText_p1Warning->Caption = "Not prime";
		}
		CheckEdits();
	} catch (EConvertError &E) {
		StaticText_p1Warning->Caption = "Empty input";
	} catch (std::invalid_argument &e) {
		StaticText_p1Warning->Caption = "Empty input";
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm_Main::Edit_p2Change(TObject *Sender)
{
	try {
		int num = StrToInt(Edit_p2->Text);
		if (IsPrime(num)) {
			StaticText_p2Warning->Caption = "Prime";
		} else {
			StaticText_p2Warning->Caption = "Not prime";
		}
		CheckEdits();
	} catch (EConvertError &E) {
		StaticText_p2Warning->Caption = "Empty input";
	} catch (std::invalid_argument &e) {
		StaticText_p2Warning->Caption = "Empty input";
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm_Main::Edit_q1Change(TObject *Sender)
{
    try {
		int num = StrToInt(Edit_q1->Text);
		if (IsPrime(num)) {
			StaticText_q1Warning->Caption = "Prime";
		} else {
			StaticText_q1Warning->Caption = "Not prime";
		}
		CheckEdits();
	} catch (EConvertError &E) {
		StaticText_q1Warning->Caption = "Empty input";
	} catch (std::invalid_argument &e) {
		StaticText_q1Warning->Caption = "Empty input";
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm_Main::Edit_q2Change(TObject *Sender)
{
	try {
		int num = StrToInt(Edit_q2->Text);
		if (IsPrime(num)) {
			StaticText_q2Warning->Caption = "Prime";
		} else {
			StaticText_q2Warning->Caption = "Not prime";
		}
        CheckEdits();
	} catch (EConvertError &E) {
		StaticText_q2Warning->Caption = "Empty input";
	} catch (std::invalid_argument &e) {
		StaticText_q2Warning->Caption = "Empty input";
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm_Main::Edit_xChange(TObject *Sender)
{
    CheckEdits();
}
//---------------------------------------------------------------------------

void __fastcall TForm_Main::Edit_yChange(TObject *Sender)
{
    CheckEdits();
}
//---------------------------------------------------------------------------

void __fastcall TForm_Main::Edit_VigenereKeyChange(TObject *Sender)
{
    CheckEdits();
}
//---------------------------------------------------------------------------






void __fastcall TForm_Main::SpeedButton_SaveKeyClick(TObject *Sender)
{
	SaveDialog_InputKey->Title = "Save Input Key File";
    SaveDialog_InputKey->Filter = "Text files (*.txt)|*.txt|All files (*.*)|*.*";
	SaveDialog_InputKey->DefaultExt = ".txt";

	if (SaveDialog_InputKey->Execute()) {
		String filePath = SaveDialog_InputKey->FileName.c_str();

		ofstream keyFile(filePath.c_str());

		if (keyFile.is_open()) {
			keyFile << "===== Input Key =====" << endl;
			keyFile << "p1 = " << AnsiString(Edit_p1->Text.c_str()) << endl;
			keyFile << "p2 = " << AnsiString(Edit_p2->Text.c_str()) << endl;
			keyFile << "q1 = " << AnsiString(Edit_q1->Text.c_str()) << endl;
			keyFile << "q2 = " << AnsiString(Edit_q2->Text.c_str()) << endl;
			keyFile << "x = " << AnsiString(Edit_x->Text.c_str()) << endl;
			keyFile << "y = " << AnsiString(Edit_y->Text.c_str()) << endl;
			keyFile << "Vigenere Key = " << AnsiString(Edit_VigenereKey->Text.c_str()) << endl;
			keyFile.close();

			ShowMessage("Input key saved to " + String(filePath.c_str()));
		} else {
			ShowMessage("Error opening file!");
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm_Main::SpeedButton_MenuEncryptionClick(TObject *Sender)
{
	if (GroupBox_Encryption->Visible == false && GroupBox_HowToUse->Visible == true) {
		   GroupBox_Encryption->Visible = true;
		   GroupBox_HowToUse->Visible = false;
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm_Main::SpeedButton_HowToUseClick(TObject *Sender)
{
	if (GroupBox_Encryption->Visible == true && GroupBox_HowToUse->Visible == false) {
		   GroupBox_Encryption->Visible = false;
		   GroupBox_HowToUse->Visible = true;
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm_Main::SpeedButton_BrowseImageClick(TObject *Sender)
{
	OpenPictureDialog1->Filter = "BMP files (*.bmp)|*.bmp";
	if (OpenPictureDialog1->Execute()) {
		try{
            TBitmap *bmp = new TBitmap();
			bmp->LoadFromFile(OpenPictureDialog1->FileName);
			Image_Width = bmp->Width;
            Image_Height = bmp->Height;

            TJPEGImage *jpg = new TJPEGImage();
            jpg->Assign(bmp); // Convert BMP to JPEG

            // Optional: Adjust JPEG quality for speed/size tradeoff
            jpg->CompressionQuality = 80; // Adjust as needed (0-100)

            TMemoryStream *jpgStream = new TMemoryStream();
			jpg->SaveToStream(jpgStream);
			jpgStream->Position = 0; // Reset stream position

			TJPEGImage *displayJpeg = new TJPEGImage();
			displayJpeg->LoadFromStream(jpgStream);

			Image_Input->Picture->Assign(displayJpeg); // Display JPEG in TImage

			ClientDataSet1->DisableControls();
			ClientDataSet1->EmptyDataSet();

			for (int y = 0; y < bmp->Height; y++) {
				for (int x = 0; x < bmp->Width; x++){
					TColor pixelColor = bmp->Canvas->Pixels[x][y];
					int r = GetRValue(pixelColor);
					int g = GetGValue(pixelColor);
					int b = GetBValue(pixelColor);

					ClientDataSet1->Append();
					ClientDataSet1->FieldByName("X")->AsInteger = x;
					ClientDataSet1->FieldByName("Y")->AsInteger = y;
					ClientDataSet1->FieldByName("R")->AsInteger = r;
					ClientDataSet1->FieldByName("G")->AsInteger = g;
					ClientDataSet1->FieldByName("B")->AsInteger = b;
					ClientDataSet1->Post();
				}
			}
			ClientDataSet1->EnableControls();

			delete bmp;
            delete jpg;
			delete jpgStream;
			delete displayJpeg;
		}
		catch (Exception &E){
            ShowMessage("Error: " + E.Message);
        }
	}

}
//---------------------------------------------------------------------------

void __fastcall TForm_Main::SpeedButton_ShowVkClick(TObject *Sender)
{
	if (Memo_VkDebug->Visible == true) {
		   Memo_VkDebug->Visible = false;
		   SpeedButton_ShowVk->Caption = "Show";
	} else {
		Memo_VkDebug->Visible = true;
		SpeedButton_ShowVk->Caption = "Hide";
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm_Main::SpeedButton_CalculationClick(TObject *Sender)
{
	SaveDialog_Calculation->Title = "Save Calculation File";
    SaveDialog_Calculation->Filter = "Text files (*.txt)|*.txt|All files (*.*)|*.*";
	SaveDialog_Calculation->DefaultExt = ".txt";

	if (SaveDialog_Calculation->Execute()) {
		String filePath = SaveDialog_Calculation->FileName.c_str();

		ofstream keyFile(filePath.c_str());

		if (keyFile.is_open()) {
			keyFile << "===== Calculation =====" << endl;
			keyFile << "n1 = " << AnsiString(Edit_n1->Text.c_str()) << endl;
			keyFile << "phi_n1 = " << AnsiString(Edit_phi_n1->Text.c_str()) << endl;
			keyFile << "e1 = " << AnsiString(Edit_e1->Text.c_str()) << endl;
			keyFile << "d1 = " << AnsiString(Edit_d1->Text.c_str()) << endl << endl;

			keyFile << "n2 = " << AnsiString(Edit_n1->Text.c_str()) << endl;
			keyFile << "phi_n2 = " << AnsiString(Edit_phi_n2->Text.c_str()) << endl;
			keyFile << "e2 = " << AnsiString(Edit_e2->Text.c_str()) << endl;
			keyFile << "d2 = " << AnsiString(Edit_d2->Text.c_str()) << endl << endl;

			keyFile << "R1 = " << AnsiString(Edit_R1->Text.c_str()) << endl;
			keyFile << "R2 = " << AnsiString(Edit_R2->Text.c_str()) << endl;
			keyFile << "G = " << AnsiString(Edit_G->Text.c_str()) << endl;
			keyFile << "P = " << AnsiString(Edit_P->Text.c_str()) << endl << endl;
			keyFile << "A = " << AnsiString(Edit_A->Text.c_str()) << endl;
			keyFile << "B = " << AnsiString(Edit_B->Text.c_str()) << endl;
			keyFile << "K = " << AnsiString(Edit_K->Text.c_str()) << endl;
			keyFile << "K' = " << AnsiString(Edit_K1->Text.c_str()) << endl << endl;

			keyFile << "Chosen RSA key " << endl;
			keyFile << "e = " << AnsiString(Edit_e->Text.c_str()) << endl;
			keyFile << "d = " << AnsiString(Edit_d->Text.c_str()) << endl;
			keyFile << "n = " << AnsiString(Edit_n->Text.c_str()) << endl << endl;

			keyFile << "sK = " << AnsiString(Edit_sK->Text.c_str()) << endl;
            keyFile << "Encypted sK = " << AnsiString(Edit_enc_sK->Text.c_str()) << endl;
			keyFile << "Vigenere key = " << endl;
			keyFile << AnsiString(Memo_VkDebug->Lines->Text.c_str()) << endl << endl;
			keyFile.close();

			ShowMessage("Calculation saved to " + String(filePath.c_str()));
		} else {
			ShowMessage("Error opening file!");
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm_Main::SpeedButton_ResetAllClick(TObject *Sender)
{
	//Reset input key
	Edit_p1->Text = "";
	Edit_p2->Text = "";
	Edit_q1->Text = "";
	Edit_q2->Text = "";
    Edit_x->Text = "";
	Edit_y->Text = "";
	Edit_VigenereKey->Text = "";

	//Reset read only input
	Edit_p1->ReadOnly = false;
	Edit_p2->ReadOnly = false;
	Edit_q1->ReadOnly = false;
	Edit_q2->ReadOnly = false;
	Edit_x->ReadOnly = false;
	Edit_y->ReadOnly = false;
	Edit_VigenereKey->ReadOnly = false;

	//Reset input image box
	Image_Input->Picture = NULL;

	//Reset output image box
	Image_Output->Picture = NULL;

	//Reset DBgrid
	ClientDataSet1->Active = true;
	ClientDataSet2->Active = true;
	if(ClientDataSet1->CanModify && ClientDataSet2->CanModify) {
			ClientDataSet1->EmptyDataSet();
			ClientDataSet2->EmptyDataSet();
	}

	//Reset Calc. box
	Edit_n1->Text = "";
	Edit_phi_n1->Text = "";
	Edit_e1->Text = "";
	Edit_d1->Text = "";

	Edit_n2->Text = "";
	Edit_phi_n2->Text = "";
	Edit_e2->Text = "";
	Edit_d2->Text = "";

	Edit_n->Text = "";
	Edit_e->Text = "";
	Edit_d->Text = "";

	Edit_R1->Text = "";
	Edit_R2->Text = "";
	Edit_G->Text = "";
	Edit_P->Text = "";
	Edit_A->Text = "";
	Edit_B->Text = "";
	Edit_K->Text = "";
	Edit_K1->Text = "";
	Edit_sK->Text = "";
	Memo_VkDebug->Clear();
	Memo_VkDebug->Lines->Add("Track Vigenere Key");

    Label_ms->Caption = "";
}
//---------------------------------------------------------------------------

void __fastcall TForm_Main::SpeedButton_ResetKeyClick(TObject *Sender)
{
    //Reset read only input
	Edit_p1->ReadOnly = false;
	Edit_p2->ReadOnly = false;
	Edit_q1->ReadOnly = false;
	Edit_q2->ReadOnly = false;
	Edit_x->ReadOnly = false;
	Edit_y->ReadOnly = false;
	Edit_VigenereKey->ReadOnly = false;

	//Reset input
	Edit_p1->Text = "";
	Edit_p2->Text = "";
	Edit_q1->Text = "";
	Edit_q2->Text = "";
	Edit_x->Text = "";
	Edit_y->Text = "";
	Edit_VigenereKey->Text = "";
}
//---------------------------------------------------------------------------

void __fastcall TForm_Main::SpeedButton_ResetInputImageClick(TObject *Sender)
{
	//Reset image
	Image_Input->Picture = NULL;

	//Reset DBgrid
	ClientDataSet1->Active = true;
	if(ClientDataSet1->CanModify) {
			ClientDataSet1->EmptyDataSet();
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm_Main::SpeedButton_OutputImageClick(TObject *Sender)
{
	//Reset image
	Image_Output->Picture = NULL;

	//Reset DBgrid
	ClientDataSet2->Active = true;
	if(ClientDataSet2->CanModify) {
			ClientDataSet2->EmptyDataSet();
	}
}
//---------------------------------------------------------------------------





void __fastcall TForm_Main::SpeedButton_EncryptClick(TObject *Sender)
{
	//Validation
	if (Edit_p1->Text.IsEmpty() || Edit_q1->Text.IsEmpty() || Edit_p2->Text.IsEmpty() || Edit_q2->Text.IsEmpty() || Edit_VigenereKey->Text.IsEmpty() ||
		Edit_x->Text.IsEmpty() || Edit_y->Text.IsEmpty()) {
			ShowMessage("Please fill in all input key fields!");
			return;
	}

	//Get the input key parameter
	__int64 p1 = StrToInt64(Edit_p1->Text);
	__int64 q1 = StrToInt64(Edit_q1->Text);
	__int64 p2 = StrToInt64(Edit_p2->Text);
	__int64 q2 = StrToInt64(Edit_q2->Text);
	__int64 x = StrToInt64(Edit_x->Text);
	__int64 y = StrToInt64(Edit_y->Text);
	String Vs = Edit_VigenereKey->Text;

	if (ClientDataSet1->RecordCount > 0)
	{
		if (!IsPrime(p1) || !IsPrime(q1) || !IsPrime(p2) || !IsPrime(q1)) {
			if (!IsPrime(p1)) {
				ShowMessage("p1 is not prime");
			}
			if (!IsPrime(q1)) {
				ShowMessage("q1 is not prime");
			}
			if (!IsPrime(p2)) {
				ShowMessage("p2 is not prime");
			}
			if (!IsPrime(q2)) {
				ShowMessage("q2 is not prime");
			}
			return;
		}

		//Read only input
		Edit_p1->ReadOnly = true;
		Edit_p2->ReadOnly = true;
		Edit_q1->ReadOnly = true;
		Edit_q2->ReadOnly = true;
		Edit_x->ReadOnly = true;
		Edit_y->ReadOnly = true;
		Edit_VigenereKey->ReadOnly = true;

		try {
			//Get Start Time
			__int64 startTime = GetTickCount64();

			//Generate sK
			__int64 e, d, n, sK;
			GenerateSK(p1, q1, p2, q2, x, y, e, d, n, sK); //sK still encrypted here

			//Decrypting sK
			sK = ModPow(sK, d, n);

			//Generate Vk
			int NumOfPix =  ClientDataSet1->RecordCount + 1;
			String Vk = GenerateVk(Vs, NumOfPix);

			//Setup Progress Bar
			ProgressBar1->Visible = true;
			ProgressBar1->Max = ClientDataSet1->RecordCount; //Total steps
			ProgressBar1->Position = 0;

			//Set the output RGB table
			ClientDataSet2->DisableControls();
			ClientDataSet2->EmptyDataSet();

			ClientDataSet1->First();
			int VkIndex = 1; //Index to track Vigenere key characters
			while (!ClientDataSet1->Eof) {
				int x = ClientDataSet1->FieldByName("X")->AsInteger;
				int y = ClientDataSet1->FieldByName("Y")->AsInteger;
				int r = ClientDataSet1->FieldByName("R")->AsInteger;
				int g = ClientDataSet1->FieldByName("G")->AsInteger;
				int b = ClientDataSet1->FieldByName("B")->AsInteger;

				//Encryption Vigenere-multiplicative
				char VkChar = Vk[VkIndex % Vk.Length()]; // Get character from Vigenere key
				int VkValue = (int)VkChar; // Convert character to integer

				//Debug, tracking index
				Memo_VkDebug->Lines->Add("(" + AnsiString(x) + ", " + AnsiString(y) + "), " + AnsiString(VkIndex) + ", " + AnsiString(VkChar));

				int encrypted_r = (sK * r + VkValue) % 256; //Modulo 256 to keep within RGB range
				int encrypted_g = (sK * g + VkValue) % 256;
				int encrypted_b = (sK * b + VkValue) % 256;


				ClientDataSet2->Append();
				ClientDataSet2->FieldByName("X")->AsInteger = x;
				ClientDataSet2->FieldByName("Y")->AsInteger = y;
				ClientDataSet2->FieldByName("R")->AsInteger = encrypted_r;
				ClientDataSet2->FieldByName("G")->AsInteger = encrypted_g;
				ClientDataSet2->FieldByName("B")->AsInteger = encrypted_b;
				ClientDataSet2->Post();

				//Update Progress Bar
				ProgressBar1->Position++;
				Application->ProcessMessages(); //Keep UI responsive

				ClientDataSet1->Next(); //Next record
				VkIndex++;
			}

			ClientDataSet2->EnableControls();

			unique_ptr<TSaveDialog> SaveCSVDialog(new TSaveDialog(this));
			SaveCSVDialog->Filter = "CSV Files (*.csv)|*.csv|All Files (*.*)|*.*";
			SaveCSVDialog->DefaultExt = "csv";
			SaveCSVDialog->FileName = "encrypted_rgb_data.csv"; //Default file name

			if (SaveCSVDialog->Execute()) {
				AnsiString CsvFileName = SaveCSVDialog->FileName;
				std::ofstream outFile;
				outFile.open(CsvFileName.c_str(), std::ios::out | std::ios::trunc);

				if (!outFile.is_open()) {
					ShowMessage("Error: Could not open CSV file for writing!");
				} else {
					try {
						// Write Headers
						for (int i = 0; i < ClientDataSet2->FieldCount; ++i) {
                            outFile << "\"" << AnsiString(ClientDataSet2->Fields->Fields[i]->FieldName.c_str()) << "\"";
                            if (i < ClientDataSet2->FieldCount - 1) {
                                outFile << ",";
                            }
						}
						outFile << "\n";

						//Write Data Rows
						ClientDataSet2->First();
						while (!ClientDataSet2->Eof) {
							outFile << "\"" << ClientDataSet2->FieldByName("X")->AsInteger << "\"";
							outFile << ",\"";
							outFile << ClientDataSet2->FieldByName("Y")->AsInteger << "\"";
							outFile << ",\"";
							outFile << ClientDataSet2->FieldByName("R")->AsInteger << "\"";
							outFile << ",\"";
							outFile << ClientDataSet2->FieldByName("G")->AsInteger << "\"";
							outFile << ",\"";
							outFile << ClientDataSet2->FieldByName("B")->AsInteger << "\"";

							outFile << "\n";
							ClientDataSet2->Next();
						}
						ShowMessage("Encrypted RGB data successfully exported to:\n" + CsvFileName);
                    } catch (const Exception &E) {
                        ShowMessage("An error occurred during CSV export: " + E.Message);
					}
                    outFile.close();
				}
			}

			//Display the encrypted image in Image_Output
			TBitmap *encryptedImg = new TBitmap();
			encryptedImg->Width = Image_Width;
			encryptedImg->Height = Image_Height;

			ClientDataSet2->First();
			while (!ClientDataSet2->Eof) {
				int x = ClientDataSet2->FieldByName("X")->AsInteger;
				int y = ClientDataSet2->FieldByName("Y")->AsInteger;
				TColor pixelColor = (TColor) RGB(ClientDataSet2->FieldByName("R")->AsInteger,
									   ClientDataSet2->FieldByName("G")->AsInteger,
									   ClientDataSet2->FieldByName("B")->AsInteger);
				encryptedImg->Canvas->Pixels[x][y] = pixelColor;
				ClientDataSet2->Next();
			}

			//Save as BMP to Image_OutputSaveBMP (hidden)
			Image_OutputSave->Picture->Assign(encryptedImg);

            //Get End Time
			__int64 endTime = GetTickCount64();

			//Calculate Elapsed Time
			__int64 elapsedTime = endTime - startTime;

			//Display Elapsed Time
			Label_ms->Caption = IntToStr(elapsedTime) + " ms";

			//Hide Progress Bar
			ProgressBar1->Visible = false;

			//Display as jpg
			TJPEGImage *jpg = new TJPEGImage();
			jpg->Assign(encryptedImg);
			jpg->CompressionQuality = 80;
			Image_Output->Picture->Assign(jpg);
			Image_Output->Proportional = true;

			delete encryptedImg;
			delete jpg;

			SpeedButton_Encrypt->Enabled = false;

			ShowMessage("Image encrypted");
		}
		catch (Exception &E) {
			ShowMessage("Error: " + E.Message);
		}
	}
	else {
        ShowMessage("Please load an image first.");
    }
}
//---------------------------------------------------------------------------


void __fastcall TForm_Main::SpeedButton_SaveImageClick(TObject *Sender)
{
	if (SavePictureDialog1->Execute()) {
		String fileName = SavePictureDialog1->FileName;
		String fileExt = "";

		//Determine the selected filter index and append the correct extension
		switch (SavePictureDialog1->FilterIndex) {
            case 1: // BMP
                fileExt = ".bmp";
                break;
            case 2: // JPEG
                fileExt = ".jpg";
                break;
            case 3: // PNG
                fileExt = ".png";
                break;
            case 4: // GIF
                fileExt = ".gif";
				break;
			default:
                fileExt = ".bmp"; // Default to BMP if no filter is selected.
                break;
        }

        // Check if the filename already has an extension
        if (ExtractFileExt(fileName).IsEmpty()) {
            fileName += fileExt; // Append the extension
        }

		try {
            Image_OutputSave->Picture->SaveToFile(fileName);
            ShowMessage("Image saved to " + fileName);
        }
		catch (Exception &E) {
            ShowMessage("Error saving image: " + E.Message);
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TForm_Main::SpeedButton_RGBtoCSVClick(TObject *Sender)
{
	ClientDataSet1->EnableControls();

	unique_ptr<TSaveDialog> SaveCSVDialog(new TSaveDialog(this));
	SaveCSVDialog->Filter = "CSV Files (*.csv)|*.csv|All Files (*.*)|*.*";
	SaveCSVDialog->DefaultExt = "csv";
	SaveCSVDialog->FileName = "rgb_data.csv"; //Default file name

	if (SaveCSVDialog->Execute()) {
		AnsiString CsvFileName = SaveCSVDialog->FileName;
		std::ofstream outFile;
		outFile.open(CsvFileName.c_str(), std::ios::out | std::ios::trunc);

		if (!outFile.is_open()) {
			ShowMessage("Error: Could not open CSV file for writing!");
		} else {
			try {
				// Write Headers
				for (int i = 0; i < ClientDataSet1->FieldCount; ++i) {
					outFile << "\"" << AnsiString(ClientDataSet1->Fields->Fields[i]->FieldName.c_str()) << "\"";
					if (i < ClientDataSet1->FieldCount - 1) {
						outFile << ",";
					}
				}
				outFile << "\n";

				//Write Data Rows
				ClientDataSet1->First();
				while (!ClientDataSet1->Eof) {
					outFile << "\"" << ClientDataSet1->FieldByName("X")->AsInteger << "\"";
					outFile << ",\"";
					outFile << ClientDataSet1->FieldByName("Y")->AsInteger << "\"";
					outFile << ",\"";
					outFile << ClientDataSet1->FieldByName("R")->AsInteger << "\"";
					outFile << ",\"";
					outFile << ClientDataSet1->FieldByName("G")->AsInteger << "\"";
					outFile << ",\"";
					outFile << ClientDataSet1->FieldByName("B")->AsInteger << "\"";

					outFile << "\n";
					ClientDataSet1->Next();
				}
				ShowMessage("Encrypted RGB data successfully exported to:\n" + CsvFileName);
			} catch (const Exception &E) {
				ShowMessage("An error occurred during CSV export: " + E.Message);
			}
			outFile.close();
		}
	}
}
//---------------------------------------------------------------------------

