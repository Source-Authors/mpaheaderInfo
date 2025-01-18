// AboutDlg.cpp : implementation file

#include "stdafx.h"
#include "AboutDlg.h"

IMPLEMENT_DYNAMIC(AboutDlg, CBaseDlg)

AboutDlg::AboutDlg(CWnd* parent)
    : CBaseDlg(AboutDlg::IDD, parent),
      m_hIcon(AfxGetApp()->LoadIcon(SRC_IDI_APP_MAIN)) {}

AboutDlg::~AboutDlg() {}

BOOL AboutDlg::OnInitDialog() {
  BOOL rc = __super::OnInitDialog();
  if (!rc) return rc;

  // Set the icon for this dialog.
  SetIcon(m_hIcon, TRUE);
  SetIcon(m_hIcon, FALSE);

  return TRUE;
}

void AboutDlg::DoDataExchange(CDataExchange* pDX) {
  __super::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(AboutDlg, CBaseDlg)
END_MESSAGE_MAP()
