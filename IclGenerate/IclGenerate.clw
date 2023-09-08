; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CAboutDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "IclGenerate.h"

ClassCount=3
Class1=CIclGenerateApp
Class2=CIclGenerateDlg

ResourceCount=4
Resource2=IDD_ABOUT_DIALOG
Resource3=IDD_ICLGENERATE_DIALOG
Resource1=IDR_MAINFRAME
Class3=CAboutDlg
Resource4=IDR_POPMENU

[CLS:CIclGenerateApp]
Type=0
HeaderFile=IclGenerate.h
ImplementationFile=IclGenerate.cpp
Filter=N

[CLS:CIclGenerateDlg]
Type=0
HeaderFile=IclGenerateDlg.h
ImplementationFile=IclGenerateDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CIclGenerateDlg



[DLG:IDD_ICLGENERATE_DIALOG]
Type=1
Class=CIclGenerateDlg
ControlCount=5
Control1=IDC_LIST_ICON,SysListView32,1350631424
Control2=IDC_STATIC_TIP,static,1342308865
Control3=IDC_EDIT_PATH,edit,1350633600
Control4=IDC_BUTTON_BROWSE,button,1342242816
Control5=IDC_CHECK_SUB,button,1342275587

[MNU:IDR_POPMENU]
Type=1
Class=?
Command1=ID_MENUITEM_REFIND
Command2=ID_MENUITEM_SAVEAS
Command3=ID_MENUITEM_EXPORT
Command4=ID_MENUITEM_ALLSELECT
Command5=ID_MENUITEM_ABOUT
CommandCount=5

[DLG:IDD_ABOUT_DIALOG]
Type=1
Class=CAboutDlg
ControlCount=5
Control1=IDOK,button,1342242817
Control2=IDC_STATIC,static,1342177283
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC_LINK,static,1342308352

[CLS:CAboutDlg]
Type=0
HeaderFile=AboutDlg.h
ImplementationFile=AboutDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CAboutDlg
VirtualFilter=dWC

