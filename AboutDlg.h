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

#ifndef MPA_HEADER_INFO_ABOUT_DLG_H_
#define MPA_HEADER_INFO_ABOUT_DLG_H_

#include "base_dlg.h"
#include "resource.h"

// About dialog.
class AboutDlg : public CBaseDlg {
  DECLARE_DYNAMIC(AboutDlg)

 public:
  explicit AboutDlg(CWnd* parent = nullptr);
  virtual ~AboutDlg();

  // Dialog Data
  enum { IDD = IDD_ABOUTBOX };

 protected:
  void DoDataExchange(CDataExchange* de) override;
  BOOL OnInitDialog() override;

  DECLARE_MESSAGE_MAP()

 private:
  HICON m_hIcon;
};

#endif  // !MPA_HEADER_INFO_ABOUT_DLG_H_
