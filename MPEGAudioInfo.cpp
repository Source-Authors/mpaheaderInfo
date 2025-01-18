// MPEGAudioInfo.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "MPEGAudioInfo.h"
#include "MPEGAudioInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace {

CMPEGAudioInfoApp app;

}  // namespace

BEGIN_MESSAGE_MAP(CMPEGAudioInfoApp, CWinApp)
  ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

CMPEGAudioInfoApp::CMPEGAudioInfoApp() {}

BOOL CMPEGAudioInfoApp::InitInstance() {
  BOOL rc = __super::InitInstance();
  if (!rc) {
    AfxMessageBox("Failed to initialize Mpeg Audio Info app.\n", MB_ICONERROR);
    return FALSE;
  }

  AfxEnableControlContainer();

  if (FAILED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED))) {
    AfxMessageBox("Failed to initialize COM.\n", MB_ICONERROR);
    return FALSE;
  }

  // Standard initialization
  // If you are not using these features and wish to reduce the size
  // of your final executable, you should remove from the following
  // the specific initialization routines you do not need
  // Change the registry key under which our settings are stored
  // TODO: You should modify this string to be something appropriate
  // such as the name of your company or organization
  SetRegistryKey(_T("MPEG Audio Info"));

  MpegAudioInfoDlg dlg;
  m_pMainWnd = &dlg;

  INT_PTR nResponse = dlg.DoModal();
  if (nResponse == IDOK) {
    // TODO: Place code here to handle when the dialog is
    //  dismissed with OK
  } else if (nResponse == IDCANCEL) {
    // TODO: Place code here to handle when the dialog is
    //  dismissed with Cancel
  }

  CoUninitialize();

  // Since the dialog has been closed, return FALSE so that we exit the
  //  application, rather than start the application's message pump.
  return FALSE;
}
