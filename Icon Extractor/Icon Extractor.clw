; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CIconExtractorDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Icon Extractor.h"

ClassCount=3
Class1=CIconExtractorApp
Class2=CIconExtractorDlg

ResourceCount=4
Resource2=IDD_ABOUT
Resource3=IDD_ICONEXTRACTOR_DIALOG
Resource1=IDR_MAINFRAME
Class3=CAboutDlg
Resource4=IDR_POPMENU

[CLS:CIconExtractorApp]
Type=0
HeaderFile=Icon Extractor.h
ImplementationFile=Icon Extractor.cpp
Filter=N
BaseClass=CWinApp
VirtualFilter=AC

[CLS:CIconExtractorDlg]
Type=0
HeaderFile=Icon ExtractorDlg.h
ImplementationFile=Icon ExtractorDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CIconExtractorDlg



[DLG:IDD_ICONEXTRACTOR_DIALOG]
Type=1
Class=CIconExtractorDlg
ControlCount=4
Control1=IDC_LIST_ICONS,SysListView32,1350631424
Control2=IDC_STATIC_PATH,static,1342308865
Control3=IDC_EDIT_PATH,edit,1350633600
Control4=IDC_BUTTON_BROWSE,button,1342242816

[MNU:IDR_POPMENU]
Type=1
Class=?
Command1=ID_MENUITEM_REFRESH
Command2=ID_MENUITEM_EXTRACT
Command3=ID_MENUITEM_SELECTALL
Command4=ID_MENUITEM_ABOUT
CommandCount=4

[DLG:IDD_ABOUT]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDC_STATIC,static,1342177283
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC_LINK,static,1342308352

[CLS:CAboutDlg]
Type=0
HeaderFile=AboutDlg.h
ImplementationFile=AboutDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=ID_MENUITEM_ABOUT
VirtualFilter=dWC

