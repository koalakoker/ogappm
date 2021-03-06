; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "Open GApp"
#define MyAppVersion "1.2.2"
#define MyAppPublisher "Koalakoker"
#define MyAppURL "http://www.koalakoker.com/"
#define MyAppExeName "OGapp.exe"

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{5339A766-FB83-4660-96DC-342683087DF9}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}\OpenGApp
DisableProgramGroupPage=yes
LicenseFile=C:\Project\ogapp\ogappm\gpl-3.0.txt
InfoAfterFile=C:\Project\ogapp\ogappm\readme.txt
OutputDir=C:\Project\ogapp\build-OGapp-Desktop_Qt_5_7_0_MinGW_32bit-Release\release\setup
OutputBaseFilename=OGapp_1.2.2_win_171202
Compression=lzma
SolidCompression=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "italian"; MessagesFile: "compiler:Languages\Italian.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked; OnlyBelowVersion: 0,6.1

[Files]
Source: "C:\Project\ogapp\build-OGapp-Desktop_Qt_5_7_0_MinGW_32bit-Release\release\deploy\OGapp.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Project\ogapp\build-OGapp-Desktop_Qt_5_7_0_MinGW_32bit-Release\release\deploy\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{commonprograms}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: quicklaunchicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

