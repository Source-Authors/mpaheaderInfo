// GNU LESSER GENERAL PUBLIC LICENSE
// Version 3, 29 June 2007
//
// Copyright (C) 2007 Free Software Foundation, Inc. <http://fsf.org/>
//
// Everyone is permitted to copy and distribute verbatim copies of this license
// document, but changing it is not allowed.
//
// This version of the GNU Lesser General Public License incorporates the terms
// and conditions of version 3 of the GNU General Public License, supplemented
// by the additional permissions listed below.

#include "stdafx.h"
#include "base_dlg.h"

IMPLEMENT_DYNAMIC(CBaseDlg, CDialog)

BEGIN_MESSAGE_MAP(CBaseDlg, CDialog)
  ON_WM_DESTROY()
  ON_MESSAGE(WM_DPICHANGED, OnDpiChanged)
END_MESSAGE_MAP()

CBaseDlg::CBaseDlg() : m_dpi_behavior{false} {}

CBaseDlg::CBaseDlg(LPCTSTR lpszTemplateName, CWnd* pParentWnd)
    : CDialog{lpszTemplateName, pParentWnd}, m_dpi_behavior{false} {}

CBaseDlg::CBaseDlg(UINT nIDTemplate, CWnd* pParentWnd)
    : CDialog{nIDTemplate, pParentWnd}, m_dpi_behavior{false} {}

CBaseDlg::~CBaseDlg() = default;

BOOL CBaseDlg::OnInitDialog() {
  const BOOL rc{__super::OnInitDialog()};
  // dimhotepus: Try to add main icon for dialog.
  HANDLE hExeIcon = LoadImageW(GetModuleHandleW(nullptr), MAKEINTRESOURCEW(101),
                               IMAGE_ICON, 0, 0, LR_SHARED);
  if (hExeIcon) {
    SendMessage(WM_SETICON, ICON_BIG, (LPARAM)hExeIcon);
  }

  m_dpi_behavior.OnCreateWindow(m_hWnd);
  return rc;
}

void CBaseDlg::OnDestroy() {
  m_dpi_behavior.OnDestroyWindow();

  __super::OnDestroy();
}

LRESULT CBaseDlg::OnDpiChanged(WPARAM wParam, LPARAM lParam) {
  return m_dpi_behavior.OnWindowDpiChanged(wParam, lParam);
}
