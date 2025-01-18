// MPEGAudioInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MPEGAudioInfoDlg.h"
#include "MPEGAudioInfo.h"
#include "AboutDlg.h"
#include "Platform.h"

#include "LameTag.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace {

const TCHAR* IsVarTrue(bool var) { return var ? _T("Yes") : _T("No"); }

}  // namespace

MpegAudioInfoDlg::MpegAudioInfoDlg(CWnd* parent)
    : CBaseDlg(MpegAudioInfoDlg::IDD, parent),
      m_strFile(_T("")),
      m_strInfo1(_T("")),
      m_strInfo2(_T("")),
      m_strVBRInfo(_T("")),
      m_strFrameNo(_T("")),
      m_strOutput(_T("")),
      m_mpa_file(nullptr),
      m_frame(nullptr),
      m_frame_no(0),
      m_strTagInfo(_T("")),
      m_strFileInfo(_T("")) {
  m_hIcon = AfxGetApp()->LoadIcon(SRC_IDI_APP_MAIN);
}

MpegAudioInfoDlg::~MpegAudioInfoDlg() {
  delete m_frame;
  delete m_mpa_file;
}

void MpegAudioInfoDlg::DoDataExchange(CDataExchange* pDX) {
  __super::DoDataExchange(pDX);

  DDX_Text(pDX, IDC_FILE, m_strFile);
  DDX_Text(pDX, IDC_INFO1, m_strInfo1);
  DDX_Text(pDX, IDC_INFO2, m_strInfo2);
  DDX_Text(pDX, IDC_VBRINFO, m_strVBRInfo);
  DDX_Text(pDX, IDC_FRAMENO, m_strFrameNo);
  DDX_Text(pDX, IDC_OUTPUT, m_strOutput);
  DDX_Control(pDX, IDC_PREVFRAME, m_CtrlPrevFrame);
  DDX_Control(pDX, IDC_NEXTFRAME, m_CtrlNextFrame);
  DDX_Control(pDX, IDC_LASTFRAME, m_CtrlLastFrame);
  DDX_Control(pDX, IDC_FIRSTFRAME, m_CtrlFirstFrame);
  DDX_Control(pDX, IDC_CHECKFILE, m_CtrlCheckFile);
  DDX_Text(pDX, IDC_TAGINFO, m_strTagInfo);
  DDX_Text(pDX, IDC_FILEINFO, m_strFileInfo);
}

BEGIN_MESSAGE_MAP(MpegAudioInfoDlg, CBaseDlg)
  ON_WM_PAINT()
  ON_WM_QUERYDRAGICON()

  ON_BN_CLICKED(IDC_OPENFILE, OnBnClickedOpenFile)
  ON_BN_CLICKED(IDC_NEXTFRAME, OnBnClickedNextframe)
  ON_BN_CLICKED(IDC_PREVFRAME, OnBnClickedPrevframe)
  ON_BN_CLICKED(IDC_FIRSTFRAME, OnBnClickedFirstframe)
  ON_BN_CLICKED(IDC_LASTFRAME, OnBnClickedLastframe)
  ON_BN_CLICKED(IDC_CHECKFILE, OnBnClickedCheckfile)
  ON_BN_CLICKED(IDC_ABOUT, OnBnClickedAbout)
  ON_WM_DROPFILES()
END_MESSAGE_MAP()

BOOL MpegAudioInfoDlg::OnInitDialog() {
  BOOL rc = __super::OnInitDialog();
  if (!rc) return rc;

  // Set the icon for this dialog.
  SetIcon(m_hIcon, TRUE);   // Set big icon
  SetIcon(m_hIcon, FALSE);  // Set small icon

  DragAcceptFiles(TRUE);  // accept drag&drop of files
  return TRUE;            // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
// to draw the icon.  For MFC applications using the document/view model, this
// is automatically done for you by the framework.
void MpegAudioInfoDlg::OnPaint() {
  if (IsIconic()) {
    CPaintDC dc(this);  // device context for painting

    SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()),
                0);

    // Center icon in client rectangle
    int cxIcon =
        GetSystemMetricsForDpi(SM_CXICON, m_dpi_behavior.GetCurrentDpiX());
    int cyIcon =
        GetSystemMetricsForDpi(SM_CYICON, m_dpi_behavior.GetCurrentDpiY());

    CRect rect;
    GetClientRect(&rect);
    int x = (rect.Width() - cxIcon + 1) / 2;
    int y = (rect.Height() - cyIcon + 1) / 2;

    // Draw the icon
    dc.DrawIcon(x, y, m_hIcon);
  } else {
    __super::OnPaint();
  }
}

// The system calls this function to obtain the cursor to display while the user
// drags the minimized window.
HCURSOR MpegAudioInfoDlg::OnQueryDragIcon() {
  return static_cast<HCURSOR>(m_hIcon);
}

void MpegAudioInfoDlg::OnBnClickedOpenFile() {
  constexpr TCHAR szFilter[] =
      _T("MPEG Audio Files (*.mp*)|*.mp*|All Files (*.*)|*.*||");

  CFileDialog file_dlg(true, nullptr, nullptr,
                       OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
  file_dlg.m_ofn.lpstrTitle = _T("Open MPEG Audio File");

  if (file_dlg.DoModal() == IDOK) {
    LoadMPEGFile(file_dlg.GetPathName());
  }
}

// display frame specific information
void MpegAudioInfoDlg::GetFrameInfo(CMPAFrame* frame, unsigned int frame_no) {
  m_strInfo1.Format(
      _T("%s %s\n%u kbps, %s\nSampling Rate: %u Hz\n\nCopyright: %s, CRC: %s, ")
      _T("Private: %s,\nOriginal: %s, Emphasis: %s"),
      CMPAHeader::m_szMPEGVersions[static_cast<int>(
          frame->m_pHeader->m_Version)],
      CMPAHeader::m_szLayers[static_cast<int>(frame->m_pHeader->m_Layer)],
      frame->m_pHeader->GetBytesPerSecond() / 125,
      CMPAHeader::m_szChannelModes[static_cast<int>(
          frame->m_pHeader->m_ChannelMode)],
      frame->m_pHeader->m_dwSamplesPerSec,
      IsVarTrue(frame->m_pHeader->m_bCopyright),
      IsVarTrue(frame->m_pHeader->m_bCRC),
      IsVarTrue(frame->m_pHeader->m_bPrivate),
      IsVarTrue(frame->m_pHeader->m_bOriginal),
      CMPAHeader::m_szEmphasis[static_cast<int>(frame->m_pHeader->m_Emphasis)]);

  m_strInfo2.Format(
      _T("Beginning Of Header: Byte %u\nComputed Frame Size: %u Byte%s\n")
      _T("Padding Size: %u Byte%s\nSample%s Per Frame: %u"),
      frame->m_dwOffset, frame->m_dwFrameSize,
      frame->m_dwFrameSize > 1 ? "s" : "", frame->m_pHeader->m_dwPaddingSize,
      frame->m_pHeader->m_dwPaddingSize > 1 ? "s" : "",
      frame->m_pHeader->m_dwSamplesPerFrame > 1 ? "s" : "",
      frame->m_pHeader->m_dwSamplesPerFrame);

  if (frame_no == 0)
    m_strFrameNo = _T("Frame ?");
  else
    m_strFrameNo.Format(_T("Frame %u"), frame_no);

  const bool is_first = frame_no == 1U;
  m_CtrlFirstFrame.EnableWindow(!is_first);
  m_CtrlPrevFrame.EnableWindow(!is_first);

  const bool is_last = frame->IsLast();
  m_CtrlLastFrame.EnableWindow(!is_last);
  m_CtrlNextFrame.EnableWindow(!is_last);

  UpdateData(false);
}

void MpegAudioInfoDlg::LoadMPEGFile(LPCTSTR file_path) {
  m_strFile = file_path;
  m_strOutput.Empty();
  m_strTagInfo.Empty();
  m_strVBRInfo.Empty();
  m_strFileInfo.Empty();

  // remove previous frame
  if (m_frame) {
    delete m_frame;
    m_frame = nullptr;
    m_frame_no = 0;
  }

  // remove previous file
  if (m_mpa_file) {
    delete m_mpa_file;
    m_mpa_file = nullptr;
  }

  try {
    m_mpa_file = new CMPAFile(file_path);

    m_CtrlPrevFrame.EnableWindow(true);
    m_CtrlNextFrame.EnableWindow(true);
    m_CtrlFirstFrame.EnableWindow(true);
    m_CtrlLastFrame.EnableWindow(true);
    m_CtrlCheckFile.EnableWindow(true);

    size_t nTag = 0;
    CTag* pTag;
    CString strTag;
    while ((pTag = m_mpa_file->m_pTags->GetNextTag(nTag)) != nullptr) {
      strTag.Format(_T("%s V%.2f:\tPosition: Byte %u\n\t\tSize: %u Byte%s\n"),
                    pTag->GetName(), pTag->GetVersion(), pTag->GetOffset(),
                    pTag->GetSize(), pTag->GetSize() > 1 ? "s" : "");

      m_strTagInfo += strTag;
    }

    if (m_mpa_file->m_pVBRHeader) {
      CString strHelp, strHeader;
      CXINGHeader* pXINGHeader;
      CVBRIHeader* pVBRHeader;

      // we need runtime type information for the dynamic_cast -> compile this
      // with /GR switch is it a XING header?
      if ((pXINGHeader =
               dynamic_cast<CXINGHeader*>(m_mpa_file->m_pVBRHeader)) != NULL) {
        if (pXINGHeader->m_dwQuality != -1)
          strHelp.Format(_T("Quality: %u\n"), pXINGHeader->m_dwQuality);
        else
          strHelp.Format(_T("Quality: not set\n"));

        // check for LAME tag
        if (pXINGHeader->m_pLAMETag) {
          if (pXINGHeader->m_pLAMETag->IsSimpleTag()) {
            strHelp.Append(_T("\nSimple LAME Tag\n"));
            strHelp.AppendFormat(
                _T("Encoder: %s\n"),
                pXINGHeader->m_pLAMETag->m_strEncoder.GetString());
          } else {
            strHelp.AppendFormat(_T("\nLAME Tag Rev. %hhu\n"),
                                 pXINGHeader->m_pLAMETag->m_bRevision);
            strHelp.AppendFormat(
                _T("Encoder: %s\n"),
                pXINGHeader->m_pLAMETag->m_strEncoder.GetString());
            strHelp.AppendFormat(_T("VBR Info: %s\n"),
                                 pXINGHeader->m_pLAMETag->GetVBRInfo());
            if (pXINGHeader->m_pLAMETag->IsVBR())
              strHelp.AppendFormat(_T("Minimum Bitrate: %hhu kbps\n"),
                                   pXINGHeader->m_pLAMETag->m_bBitrate);
            else if (pXINGHeader->m_pLAMETag->IsCBR())
              strHelp.AppendFormat(_T("Bitrate: %hhu kbps\n"),
                                   pXINGHeader->m_pLAMETag->m_bBitrate);
            else if (pXINGHeader->m_pLAMETag->IsABR())
              strHelp.AppendFormat(_T("Average Bitrate: %hhu kbps\n"),
                                   pXINGHeader->m_pLAMETag->m_bBitrate);

            strHelp.AppendFormat(_T("Lowpass Filter: %u Hz\n"),
                                 pXINGHeader->m_pLAMETag->m_dwLowpassFilterHz);
          }
        }
        strHeader = _T("XING");
      }
      // is it a VBRI header?
      else if ((pVBRHeader = dynamic_cast<CVBRIHeader*>(
                    m_mpa_file->m_pVBRHeader)) != NULL) {
        strHelp.Format(_T("Version %u\nDelay %.2f\nQuality %u\n"),
                       pVBRHeader->m_dwVersion, pVBRHeader->m_fDelay,
                       pVBRHeader->m_dwQuality);
        strHeader = _T("VBRI");
      }

      m_strVBRInfo.Format(
          _T("%s\nOffset: Byte %u\nNumber Of Frames: %u\nFile Size: %u Byte%s ")
          _T("(%u KiB%s)\n"),
          strHeader.GetString(), m_mpa_file->m_pVBRHeader->m_dwOffset,
          m_mpa_file->m_pVBRHeader->m_dwFrames,
          m_mpa_file->m_pVBRHeader->m_dwBytes,
          m_mpa_file->m_pVBRHeader->m_dwBytes > 1 ? "s" : "",
          m_mpa_file->m_pVBRHeader->m_dwBytes / 1024,
          m_mpa_file->m_pVBRHeader->m_dwBytes / 1024 > 1 ? "s" : "");
      m_strVBRInfo += strHelp;
    } else
      m_strVBRInfo.Format(_T("None"));

    unsigned int uLengthSec = m_mpa_file->GetLengthSec();
    unsigned int uFileSize = m_mpa_file->GetFileSize();
    m_strFileInfo.Format(
        _T("File Size (without tags): %u Byte%s (%u KiB%s)\nEstimated ")
        _T("Length: %.2u:%.2u"),
        uFileSize, uFileSize > 1 ? "s" : "", uFileSize / 1024,
        uFileSize / 1024 > 1 ? "s" : "", uLengthSec / 60, uLengthSec % 60);

    // current frame is first frame	(need new instance)
    OnBnClickedFirstframe();
  } catch (CMPAException& ex) {
    CString error_msg;
    error_msg.Format(
        "Sorry, unable to open '%s'. %s\n\nIs it valid MPEG Audio file?",
        file_path, ex.GetErrorDescription());
    DumpSystemError(error_msg.GetString());

    m_strFile.Empty();

    m_CtrlPrevFrame.EnableWindow(false);
    m_CtrlNextFrame.EnableWindow(false);
    m_CtrlFirstFrame.EnableWindow(false);
    m_CtrlLastFrame.EnableWindow(false);
    m_CtrlCheckFile.EnableWindow(false);
  }

  UpdateData(false);
}

void MpegAudioInfoDlg::OnBnClickedNextframe() {
  if (auto* frame = m_mpa_file->GetFrame(CMPAFile::GetType::Next, m_frame);
      frame) {
    m_frame = frame;

    if (m_frame_no > 0) ++m_frame_no;

    GetFrameInfo(m_frame, m_frame_no);
  }
}

void MpegAudioInfoDlg::OnBnClickedPrevframe() {
  if (auto* frame = m_mpa_file->GetFrame(CMPAFile::GetType::Prev, m_frame);
      frame) {
    m_frame = frame;

    if (m_frame_no > 1) --m_frame_no;

    GetFrameInfo(m_frame, m_frame_no);
  }
}

void MpegAudioInfoDlg::OnBnClickedFirstframe() {
  if ((m_frame = m_mpa_file->GetFrame(CMPAFile::GetType::First, m_frame)) !=
      nullptr) {
    m_frame_no = 1;

    GetFrameInfo(m_frame, m_frame_no);
  }
}

void MpegAudioInfoDlg::OnBnClickedLastframe() {
  if ((m_frame = m_mpa_file->GetFrame(CMPAFile::GetType::Last, m_frame)) !=
      nullptr) {
    m_frame_no = 0;

    GetFrameInfo(m_frame, m_frame_no);
  }
}

void MpegAudioInfoDlg::OnBnClickedCheckfile() {
  m_strOutput = _T("");

  // go through every frame of the file and look if the next frame is at the
  // computed offset and valid
  CMPAFrame* current_mpa_frame = m_mpa_file->GetFrame(CMPAFile::GetType::First);
  CMPAFrame* prev_mpa_frame = m_mpa_file->GetFrame(CMPAFile::GetType::First);
  if (!current_mpa_frame || !prev_mpa_frame) {
    m_strOutput =
        _T("Couldn't find first frame. This is probably no MPEG audio file!");
    return;
  }

  unsigned int uPaddedFrameNo = 0, uUnpaddedFrameNo = 0, uTotalFrameNo = 0;
  unsigned int uAverBitrate = current_mpa_frame->m_pHeader->m_dwBitrate;
  bool bVBR = false;
  CString strHelp;

  do {
    uTotalFrameNo++;

    // check CRC
    if (current_mpa_frame->m_pHeader->m_bCRC) {
      if (!current_mpa_frame->CheckCRC()) {
        strHelp.Format(_T("Wrong CRC checksum in frame %u\r\n"), uTotalFrameNo);
        m_strOutput += strHelp;
      };
    }

    // is it VBR?
    if (current_mpa_frame->m_pHeader->m_dwBitrate != uAverBitrate) {
      // aver. bitrate += (new bitrate - aver. bitrate / #frames)
      // convert to int (some values can be negative)
      uAverBitrate += (int)((int)current_mpa_frame->m_pHeader->m_dwBitrate -
                            (int)uAverBitrate) /
                      (int)uTotalFrameNo;
      bVBR = true;
    }

    // even in VBR files these values must not change
    if (current_mpa_frame->m_pHeader->m_dwSamplesPerSec !=
        prev_mpa_frame->m_pHeader->m_dwSamplesPerSec) {
      CString strNewSamplingRate, strOldSamplingRate;
      strOldSamplingRate.Format(_T("%u Hz"),
                                prev_mpa_frame->m_pHeader->m_dwSamplesPerSec);
      // new sampling rate
      strNewSamplingRate.Format(
          _T("%u Hz"), current_mpa_frame->m_pHeader->m_dwSamplesPerSec);

      InvalidChange(_T("Sampling rate"), uTotalFrameNo, strOldSamplingRate,
                    strNewSamplingRate);
    }

    if (current_mpa_frame->m_pHeader->m_Layer !=
        prev_mpa_frame->m_pHeader->m_Layer) {
      InvalidChange(_T("Layer"), uTotalFrameNo,
                    CMPAHeader::m_szLayers[static_cast<int>(
                        prev_mpa_frame->m_pHeader->m_Layer)],
                    CMPAHeader::m_szLayers[static_cast<int>(
                        current_mpa_frame->m_pHeader->m_Layer)]);
    }

    if (current_mpa_frame->m_pHeader->m_Version !=
        prev_mpa_frame->m_pHeader->m_Version) {
      InvalidChange(_T("Version"), uTotalFrameNo,
                    CMPAHeader::m_szMPEGVersions[static_cast<int>(
                        prev_mpa_frame->m_pHeader->m_Version)],
                    CMPAHeader::m_szMPEGVersions[static_cast<int>(
                        current_mpa_frame->m_pHeader->m_Version)]);
    }

    if (uTotalFrameNo > 1 && current_mpa_frame->m_dwOffset !=
                                 prev_mpa_frame->GetSubsequentHeaderOffset()) {
      strHelp.Format(
          _T("Frame %u header expected at byte %u, but found at byte %u.\r\n")
          _T("Frame %u (bytes %u-%u) was %u bytes long (expected %u ")
          _T("bytes).\r\n\r\n"),
          uTotalFrameNo, prev_mpa_frame->GetSubsequentHeaderOffset(),
          current_mpa_frame->m_dwOffset, uTotalFrameNo - 1,
          prev_mpa_frame->m_dwOffset, current_mpa_frame->m_dwOffset,
          current_mpa_frame->m_dwOffset - prev_mpa_frame->m_dwOffset,
          prev_mpa_frame->m_dwFrameSize);
      m_strOutput += strHelp;
    }

    if (current_mpa_frame->m_pHeader->m_dwPaddingSize > 0)
      uPaddedFrameNo++;
    else
      uUnpaddedFrameNo++;

    delete prev_mpa_frame;
    prev_mpa_frame = current_mpa_frame;
  } while ((current_mpa_frame = m_mpa_file->GetFrame(
                CMPAFile::GetType::Next, current_mpa_frame, false)) != nullptr);

  if (prev_mpa_frame->GetSubsequentHeaderOffset() > m_mpa_file->GetEnd()) {
    strHelp.Format(
        _T("Last frame truncated.\r\nFrame %u (bytes %u-%u) was %u bytes ")
        _T("long (expected %u bytes)\r\n\r\n"),
        uTotalFrameNo, prev_mpa_frame->m_dwOffset, m_mpa_file->GetEnd(),
        m_mpa_file->GetEnd() - prev_mpa_frame->m_dwOffset,
        prev_mpa_frame->m_dwFrameSize);
    m_strOutput += strHelp;
  } else if (prev_mpa_frame->GetSubsequentHeaderOffset() <
             m_mpa_file->GetEnd()) {
    strHelp.Format(
        _T("Free space or unrecognized Tag after last frame %u (bytes ")
        _T("%u-%u).\r\n\r\n"),
        uTotalFrameNo, prev_mpa_frame->m_dwOffset, m_mpa_file->GetEnd());
    m_strOutput += strHelp;
  }

  if (m_strOutput.IsEmpty()) {
    m_strOutput += (_T("No errors found in file.\r\n\r\n"));
  }

  m_strOutput += (_T("\r\nSummary:\r\n===============\r\n"));
  strHelp.Format(_T("Total number of frames: %u, unpadded: %u, padded: %u\r\n"),
                 uTotalFrameNo, uUnpaddedFrameNo, uPaddedFrameNo);
  m_strOutput += strHelp;

  const unsigned int uLengthSec =
      prev_mpa_frame->m_pHeader->GetLengthSecond(uTotalFrameNo);
  if (bVBR)
    strHelp.Format(_T("File is VBR. Average bitrate is %u kbps.\r\n"),
                   uAverBitrate / 1000);
  else
    strHelp.Format(_T("File is CBR. Bitrate of each frame is %u kbps.\r\n"),
                   uAverBitrate / 1000);
  m_strOutput += strHelp;

  strHelp.Format(_T("Exact length: %.2u:%.2u\r\n"), uLengthSec / 60,
                 uLengthSec % 60);
  m_strOutput += strHelp;

  delete prev_mpa_frame;

  UpdateData(false);
}

void MpegAudioInfoDlg::InvalidChange(LPCTSTR szWhat, unsigned int uFrame,
                                     LPCTSTR szOldValue, LPCTSTR szNewValue) {
  CString strHelp;
  strHelp.Format(_T("%s changed in frame %u from %s to %s\r\n"), szWhat, uFrame,
                 szOldValue, szNewValue);
  m_strOutput += strHelp;
}

void MpegAudioInfoDlg::OnBnClickedAbout() {
  AboutDlg dlg;
  dlg.DoModal();
}

void MpegAudioInfoDlg::OnDropFiles(HDROP hDropInfo) {
  // get information about dropped files

  // get size of filename of first file
  UINT nBufferSize = ::DragQueryFile(hDropInfo, 0, NULL, 0);
  if (!nBufferSize) return;

  ++nBufferSize;

  LPTSTR szFilename = new TCHAR[nBufferSize];

  // get filename of first file
  if (!::DragQueryFile(hDropInfo, 0, szFilename, nBufferSize)) return;

  LoadMPEGFile(szFilename);

  delete[] szFilename;

  ::DragFinish(hDropInfo);

  __super::OnDropFiles(hDropInfo);
}
