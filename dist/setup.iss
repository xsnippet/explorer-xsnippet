#define AppName "explorer-xsnippet"
#define AppVersion "0.1"
#define AppPublisher "Igor Kalnitsky"
#define AppURL "http://github.com/ikalnitsky/explorer-xsnippet"
#define AppExeName "explorer-nautilus.exe"

[Setup]
AppId={{1322315F-87CE-4577-8234-A9E02BE363BC}
AppName={#AppName}
AppVersion={#AppVersion}
AppPublisher={#AppPublisher}
AppPublisherURL={#AppURL}
AppSupportURL={#AppURL}
AppUpdatesURL={#AppURL}
DefaultDirName={pf}\{#AppName}
DefaultGroupName={#AppName}
DisableProgramGroupPage=yes
LicenseFile=..\bin\gpl-3.0.txt
OutputDir=.\
OutputBaseFilename={#AppName}-{#AppVersion}
SetupIconFile=..\src\explorer-nautilus\xsnippet.ico
Compression=lzma
SolidCompression=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Files]
Source: "..\bin\explorer-nautilus.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs

[Icons]
Name: "{sendto}\xsnippet.org"; Filename: "{app}\{#AppExeName}"

