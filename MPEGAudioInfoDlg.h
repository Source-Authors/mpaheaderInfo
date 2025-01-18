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

#ifndef MPA_HEADER_INFO_MPEG_AUDIO_INFO_DLG_H_
#define MPA_HEADER_INFO_MPEG_AUDIO_INFO_DLG_H_

#include "mpafile.h"
#include "base_dlg.h"
#include "resource.h"

// MPEG Audio Info dialog.
class MpegAudioInfoDlg : public CBaseDlg {
 public:
  explicit MpegAudioInfoDlg(CWnd* parent = nullptr);
  MpegAudioInfoDlg::~MpegAudioInfoDlg();

  // Dialog Data
  enum { IDD = IDD_MPEGAUDIOINFO_DIALOG };

 protected:
  void DoDataExchange(CDataExchange* de) override;

 private:
  HICON m_hIcon;
  CMPAFile* m_mpa_file;
  CMPAFrame* m_frame;  // current frame
  unsigned int m_frame_no;

  void InvalidChange(LPCTSTR szWhat, unsigned int uFrame, LPCTSTR szOldValue,
                     LPCTSTR szNewValue);

  // Generated message map functions
  DECLARE_MESSAGE_MAP()
  BOOL OnInitDialog() override;
  afx_msg void OnPaint();
  afx_msg HCURSOR OnQueryDragIcon();
  afx_msg void OnBnClickedOpenFile();
  afx_msg void OnBnClickedNextframe();
  afx_msg void OnBnClickedPrevframe();
  afx_msg void OnBnClickedFirstframe();
  afx_msg void OnBnClickedLastframe();
  afx_msg void OnBnClickedCheckfile();
  afx_msg void OnBnClickedAbout();

  void LoadMPEGFile(LPCTSTR szFile);
  void GetFrameInfo(CMPAFrame* pFrame, unsigned int uFrameNo);

  CString m_strFile;
  CString m_strInfo1;
  CString m_strInfo2;

  CString m_strVBRInfo;
  CString m_strFrameNo;
  CString m_strOutput;

  CString m_strTagInfo;
  CString m_strFileInfo;

  CButton m_CtrlPrevFrame;
  CButton m_CtrlNextFrame;
  CButton m_CtrlLastFrame;
  CButton m_CtrlFirstFrame;
  CButton m_CtrlCheckFile;

 public:
  afx_msg void OnDropFiles(HDROP hDropInfo);
};

#endif  // !MPA_HEADER_INFO_MPEG_AUDIO_INFO_DLG_H_
