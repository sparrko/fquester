; Этот сценарий создан с помощью Мастера Inno Setup.
; ОБРАТИТЕСЬ К СПРАВОЧНОЙ ДОКУМЕНТАЦИИ, ЧТОБЫ ИСПОЛЬЗОВАТЬ ВСЕ ВОЗМОЖНОСТИ INNO SETUP!

[Setup]
; Примечание: Значение AppId является уникальным идентификатором для этого приложения.
; Не используйте одно и тоже значение AppId для разных приложений.
; (Для создания нового значения GUID, выберите в меню "Инструменты" пункт "Создать GUID".)
AppId={{B3E2374B-05D4-4E6F-9DE1-94D6F4CAE6EF}
AppName=FQuester
AppVersion=1.1
;AppVerName=QAL Redactor Alpha
DefaultDirName=C:/Games/FQuester
DefaultGroupName=FQuester
AllowNoIcons=yes
OutputDir=C:\Users\79372\Desktop
OutputBaseFilename=setup
Compression=lzma
SolidCompression=yes

[Languages]
Name: "russian"; MessagesFile: "compiler:Languages\Russian.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "C:\FQuester\FQuester.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\FQuester\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
; Примечание: Не используйте флаги "ignoreversion" для общих системных файлов.

[Setup]
WizardImageFile = "C:\mysmallimage.bmp"

[Icons]
Name: "{group}\FQuester"; Filename: "{app}\FQuester.exe"
Name: "{group}\{cm:UninstallProgram,QAL Redactor}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\FQuester"; Filename: "{app}\FQuester.exe"; Tasks: desktopicon

[Registry]
Root: HKCU; Subkey: "Software\EDO"; Flags: uninsdeletekeyifempty
Root: HKCU; Subkey: "Software\EDO\FQuester"; Flags: uninsdeletekey
Root: HKLM; Subkey: "Software\EDO"; Flags: uninsdeletekeyifempty
Root: HKLM; Subkey: "Software\EDO\FQuester"; Flags: uninsdeletekey
Root: HKLM; Subkey: "Software\EDO\FQuester\Settings"; ValueType: string; ValueName: "Path"; ValueData: "{app}"

Root: HKCR; SubKey: QAL Pack; ValueType: string; ValueData: FQuester QAL Pack; Flags: uninsdeletekey
Root: HKCR; SubKey: QAL Pack\Shell\Open\Command; ValueType: string; ValueData: """{app}\FRP.exe"" ""%1""";  Flags: uninsdeletevalue
Root: HKCR; Subkey: QAL Pack\DefaultIcon; ValueType: string; ValueData: {app}\FRP.exe;   Flags: uninsdeletevalue
Root: HKCR; SubKey: .qal; ValueType: string; ValueData: QAL Pack; Flags: uninsdeletekey

[Run]
Filename: "{src}\drivers\com\SETUP.EXE"; Description: "Установить драйвер для приемника"; Flags: postinstall waituntilidle skipifsilent runascurrentuser
; Filename: "{app}\FQuester Redactor.exe"; Description: "{cm:LaunchProgram,QAL Redactor}"; Flags: nowait postinstall skipifsilent 
