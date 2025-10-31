object Form_Main: TForm_Main
  Left = 0
  Top = 0
  Width = 1360
  Height = 720
  AutoScroll = True
  Caption = 
    'RSA-Diffie-Hellman and Vigenere-Multiplicative Cipher for Image ' +
    'Cryptography'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -16
  Font.Name = 'Segoe UI'
  Font.Style = [fsBold]
  OnCreate = FormCreate
  DesignSize = (
    1344
    681)
  TextHeight = 21
  object Shape_TopLine: TShape
    Left = 8
    Top = 48
    Width = 1328
    Height = 4
    Anchors = [akLeft, akTop, akRight]
    Brush.Color = clBlack
    ExplicitWidth = 1064
  end
  object GroupBox_HowToUse: TGroupBox
    Left = 182
    Top = 58
    Width = 1154
    Height = 587
    Caption = 'How To Use'
    TabOrder = 0
    Visible = False
    object RichEdit1: TRichEdit
      Left = 2
      Top = 23
      Width = 1150
      Height = 562
      Align = alClient
      Color = clBtnFace
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Segoe UI'
      Font.Style = [fsBold]
      Lines.Strings = (
        '1. Enter Keys'
        
          '    - In the "Decryption Input Key" section, enter the required ' +
          'numerical key (sK).'
        
          '    - Enter the text key for the Vigenere cipher in the "Vigener' +
          'e Key (Vs)" field.'
        '    Note: sK obtained from encryption app.'
        '2. Load Input Image'
        '    - Click the "Browse" button in the "Input Image" section.'
        '    - Select the image file you want to decrypt.'
        '    The image should appear in the "Input Image" display area.'
        '3. Decrypt the Image'
        '    - Click the "Decrypt" button.'
        
          '    The program will perform the RSA-Diffie-Hellman and Vigenere' +
          '-Multiplicative cipher decryption using the entered keys.'
        '4. View Decrypted Image'
        
          '    The decrypted image will be displayed in the "Output Image" ' +
          'area.'
        '5. Save Decrypted Image'
        '    - Click the "Save" button in the "Output Image" section.'
        
          '    - Choose a location and filename to save the decrypted image' +
          '.'
        ''
        'Optional: '
        '1. Save Keys'
        
          '    - Click the "Save Input Key to txt file" button to save the ' +
          'keys to a text file.'
        '2. Save Calculation Result'
        
          '    - Click the "Save" button in calculation section to save the' +
          ' calculation results to a text file.'
        ''
        'Reset:'
        ' - Use the "Reset" buttons to clear the input or output image.'
        ' - Use the "Reset Key" button to clear the entered keys.'
        ' - Use the "Reset All" button to clear everything.')
      ParentFont = False
      PlainText = True
      ReadOnly = True
      ScrollBars = ssBoth
      TabOrder = 0
    end
  end
  object GroupBox_Decryption: TGroupBox
    Left = 182
    Top = 58
    Width = 1154
    Height = 587
    Caption = 'Decryption'
    TabOrder = 1
    object SpeedButton_Decrypt: TSpeedButton
      Left = 443
      Top = 52
      Width = 89
      Height = 29
      Caption = 'Decrypt'
      Enabled = False
      OnClick = SpeedButton_DecryptClick
    end
    object SpeedButton_ResetAll: TSpeedButton
      Left = 443
      Top = 87
      Width = 89
      Height = 29
      Caption = 'Reset All'
      OnClick = SpeedButton_ResetAllClick
    end
    object Label_Runtime: TLabel
      Left = 553
      Top = 73
      Width = 69
      Height = 21
      Caption = 'Runtime:'
    end
    object Label_ms: TLabel
      Left = 628
      Top = 73
      Width = 4
      Height = 21
    end
    object GroupBox_InputKey: TGroupBox
      Left = 0
      Top = 24
      Width = 417
      Height = 119
      Caption = 'Input Key'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Segoe UI'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 0
      object Label_VigenereKey: TLabel
        Left = 152
        Top = 28
        Width = 114
        Height = 17
        Caption = 'Vigenere Key (Vs):'
      end
      object SpeedButton_SaveKey: TSpeedButton
        Left = 168
        Top = 87
        Width = 165
        Height = 27
        Caption = 'Save input key to .txt file'
        Enabled = False
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Segoe UI'
        Font.Style = [fsBold]
        ParentFont = False
        OnClick = SpeedButton_SaveKeyClick
      end
      object SpeedButton_ResetKey: TSpeedButton
        Left = 89
        Top = 89
        Width = 73
        Height = 22
        Caption = 'Reset Key'
        OnClick = SpeedButton_ResetKeyClick
      end
      object Label17: TLabel
        Left = 16
        Top = 28
        Width = 25
        Height = 17
        Caption = 'EsK:'
      end
      object Label2: TLabel
        Left = 28
        Top = 59
        Width = 12
        Height = 17
        Caption = 'd:'
      end
      object Label3: TLabel
        Left = 160
        Top = 59
        Width = 12
        Height = 17
        Caption = 'n:'
      end
      object Edit_VigenereKey: TEdit
        Left = 280
        Top = 24
        Width = 121
        Height = 25
        TabOrder = 0
        OnChange = Edit_VigenereKeyChange
      end
      object Edit_enc_sK: TEdit
        Left = 45
        Top = 24
        Width = 85
        Height = 25
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Segoe UI'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 1
        OnChange = Edit_enc_sKChange
        OnKeyPress = Edit_enc_sKKeyPress
      end
      object Edit_d: TEdit
        Left = 45
        Top = 55
        Width = 85
        Height = 25
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Segoe UI'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 2
        OnChange = Edit_dChange
        OnKeyPress = Edit_dKeyPress
      end
      object Edit_n: TEdit
        Left = 179
        Top = 56
        Width = 85
        Height = 25
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Segoe UI'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 3
        OnChange = Edit_nChange
        OnKeyPress = Edit_nKeyPress
      end
    end
    object GroupBox_InputImage: TGroupBox
      Left = 3
      Top = 149
      Width = 334
      Height = 422
      Caption = 'Input Image'
      TabOrder = 1
      object SpeedButton_BrowseImage: TSpeedButton
        Left = 240
        Top = 215
        Width = 81
        Height = 31
        Caption = 'Browse'
        OnClick = SpeedButton_BrowseImageClick
      end
      object SpeedButton_ResetInputImage: TSpeedButton
        Left = 128
        Top = 215
        Width = 81
        Height = 31
        Caption = 'Reset'
        OnClick = SpeedButton_ResetInputImageClick
      end
      object SpeedButton_RGBtoCSV: TSpeedButton
        Left = 14
        Top = 214
        Width = 99
        Height = 31
        Caption = 'Save to CSV'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Segoe UI'
        Font.Style = [fsBold]
        ParentFont = False
        OnClick = SpeedButton_RGBtoCSVClick
      end
      object DBGrid1: TDBGrid
        Left = 2
        Top = 252
        Width = 330
        Height = 168
        DataSource = DataSource1
        TabOrder = 0
        TitleFont.Charset = DEFAULT_CHARSET
        TitleFont.Color = clWindowText
        TitleFont.Height = -16
        TitleFont.Name = 'Segoe UI'
        TitleFont.Style = [fsBold]
      end
      object Panel1: TPanel
        Left = 13
        Top = 26
        Width = 308
        Height = 183
        Caption = 'No Image'
        TabOrder = 1
        object Image_Input: TImage
          Left = 1
          Top = 1
          Width = 306
          Height = 181
          Align = alClient
          Center = True
          Proportional = True
          ExplicitLeft = 6
          ExplicitTop = 3
          ExplicitWidth = 296
          ExplicitHeight = 175
        end
      end
    end
    object GroupBox_OutputImage: TGroupBox
      Left = 616
      Top = 149
      Width = 334
      Height = 422
      Caption = 'Output Image'
      TabOrder = 2
      object SpeedButton_SaveImage: TSpeedButton
        Left = 240
        Top = 215
        Width = 81
        Height = 31
        Caption = 'Save'
        OnClick = SpeedButton_SaveImageClick
      end
      object SpeedButton_OutputImage: TSpeedButton
        Left = 144
        Top = 215
        Width = 81
        Height = 31
        Caption = 'Reset'
        OnClick = SpeedButton_OutputImageClick
      end
      object Image_OutputSave: TImage
        Left = 104
        Top = 214
        Width = 26
        Height = 30
        Visible = False
      end
      object DBGrid2: TDBGrid
        Left = 3
        Top = 252
        Width = 330
        Height = 168
        DataSource = DataSource2
        TabOrder = 0
        TitleFont.Charset = DEFAULT_CHARSET
        TitleFont.Color = clWindowText
        TitleFont.Height = -16
        TitleFont.Name = 'Segoe UI'
        TitleFont.Style = [fsBold]
      end
      object Panel2: TPanel
        Left = 12
        Top = 27
        Width = 308
        Height = 183
        Caption = 'No Image'
        TabOrder = 1
        object Image_Output: TImage
          Left = 1
          Top = 1
          Width = 306
          Height = 181
          Align = alClient
          Center = True
          Proportional = True
          ExplicitLeft = 13
          ExplicitTop = 9
          ExplicitWidth = 92
          ExplicitHeight = 73
        end
      end
    end
    object GroupBox_Calculation: TGroupBox
      Left = 343
      Top = 149
      Width = 267
      Height = 422
      Caption = 'Calculation'
      TabOrder = 3
      object Label18: TLabel
        Left = 17
        Top = 85
        Width = 25
        Height = 21
        Caption = 'Vk:'
      end
      object SpeedButton_Calculation: TSpeedButton
        Left = 100
        Top = 0
        Width = 71
        Height = 22
        Caption = 'Save'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Segoe UI'
        Font.Style = [fsBold]
        ParentFont = False
        OnClick = SpeedButton_CalculationClick
      end
      object Label1: TLabel
        Left = 17
        Top = 58
        Width = 57
        Height = 21
        Caption = 'Inv(sK):'
      end
      object Label4: TLabel
        Left = 17
        Top = 28
        Width = 21
        Height = 21
        Caption = 'sK:'
      end
      object Memo_VkDebug: TMemo
        Left = 10
        Top = 120
        Width = 247
        Height = 294
        Lines.Strings = (
          'Track Vigenere Key')
        ReadOnly = True
        ScrollBars = ssBoth
        TabOrder = 0
      end
      object Edit_InvsK: TEdit
        Left = 80
        Top = 58
        Width = 85
        Height = 25
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Segoe UI'
        Font.Style = [fsBold]
        ParentFont = False
        ReadOnly = True
        TabOrder = 1
      end
      object Edit_sK: TEdit
        Left = 80
        Top = 28
        Width = 85
        Height = 25
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Segoe UI'
        Font.Style = [fsBold]
        ParentFont = False
        ReadOnly = True
        TabOrder = 2
      end
    end
  end
  object StaticText_Title: TStaticText
    Left = 8
    Top = 8
    Width = 762
    Height = 34
    Caption = 
      'RSA-Diffie-Hellman and Vigenere-Multiplicative Cipher for Image ' +
      'Decryption'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -21
    Font.Name = 'Segoe UI'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 2
  end
  object Panel_Sidebar: TPanel
    Left = 8
    Top = 58
    Width = 168
    Height = 615
    Anchors = [akLeft, akTop, akBottom]
    TabOrder = 3
    object SpeedButton_MenuDecryption: TSpeedButton
      Left = 8
      Top = 8
      Width = 153
      Height = 33
      Align = alCustom
      Caption = 'Decryption'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Segoe UI'
      Font.Style = [fsBold]
      ParentFont = False
      OnClick = SpeedButton_MenuDecryptionClick
    end
    object SpeedButton_HowToUse: TSpeedButton
      Left = 8
      Top = 40
      Width = 153
      Height = 33
      Caption = 'How To Use'
      OnClick = SpeedButton_HowToUseClick
    end
  end
  object ProgressBar1: TProgressBar
    Left = 182
    Top = 651
    Width = 1154
    Height = 22
    TabOrder = 4
    Visible = False
  end
  object StaticText_Name: TStaticText
    Left = 1085
    Top = 17
    Width = 251
    Height = 25
    Caption = 'Adhimas Wahyu - 082011233011'
    TabOrder = 5
  end
  object SaveDialog_InputKey: TSaveDialog
    Left = 23
    Top = 146
  end
  object DataSource1: TDataSource
    DataSet = ClientDataSet1
    Left = 26
    Top = 207
  end
  object ClientDataSet1: TClientDataSet
    Aggregates = <>
    Params = <>
    Left = 58
    Top = 207
  end
  object DataSource2: TDataSource
    DataSet = ClientDataSet2
    Left = 23
    Top = 263
  end
  object ClientDataSet2: TClientDataSet
    Aggregates = <>
    Params = <>
    Left = 55
    Top = 263
  end
  object OpenPictureDialog1: TOpenPictureDialog
    Left = 18
    Top = 319
  end
  object SaveDialog_Calculation: TSaveDialog
    Left = 54
    Top = 151
  end
  object SavePictureDialog1: TSavePictureDialog
    Left = 55
    Top = 322
  end
end
