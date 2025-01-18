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

#ifndef MPA_HEADER_INFO_BASE_DLG_H_
#define MPA_HEADER_INFO_BASE_DLG_H_

#include "dpi_wnd_behavior.h"

class CBaseDlg : public CDialog {
  DECLARE_DYNAMIC(CBaseDlg)

 public:
  CBaseDlg();
  explicit CBaseDlg(LPCTSTR lpszTemplateName, CWnd* pParentWnd = nullptr);
  explicit CBaseDlg(UINT nIDTemplate, CWnd* pParentWnd = nullptr);
  virtual ~CBaseDlg();

 protected:
  BOOL OnInitDialog() override;

  afx_msg void OnDestroy();
  afx_msg LRESULT OnDpiChanged(WPARAM wParam, LPARAM lParam);
  DECLARE_MESSAGE_MAP()

  DpiWindowBehavior m_dpi_behavior;
};

#endif  // !MPA_HEADER_INFO_BASE_DLG_H_
