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

#include "Platform.h"
#include "MPAException.h"

/// CMPAException: exception class
//////////////////////////////////////////////

CMPAException::CMPAException(ErrorIDs ErrorID, LPCTSTR szFile,
                             LPCTSTR szFunction, bool bGetLastError)
    : m_ErrorID(ErrorID),
      m_bGetLastError(bGetLastError),
      m_szErrorMsg(nullptr) {
  m_szFile = strdup(szFile);
  m_szFunction = strdup(szFunction);
}

// copy constructor (necessary for exception throwing without pointers)
CMPAException::CMPAException(const CMPAException& Source) {
  m_ErrorID = Source.m_ErrorID;
  m_bGetLastError = Source.m_bGetLastError;
  m_szFile = strdup(Source.m_szFile);
  m_szFunction = strdup(Source.m_szFunction);
  if (Source.m_szErrorMsg) {
    const size_t maxSize = strlen(Source.m_szErrorMsg) + 1;
    m_szErrorMsg = new char[maxSize];
    strcpy(m_szErrorMsg, Source.m_szErrorMsg);
  } else {
    m_szErrorMsg = nullptr;
  }
}

CMPAException& CMPAException::operator=(CMPAException Source) {
  std::swap(m_ErrorID, Source.m_ErrorID);
  std::swap(m_bGetLastError, Source.m_bGetLastError);
  std::swap(m_szFunction, Source.m_szFunction);
  std::swap(m_szFile, Source.m_szFile);
  std::swap(m_szErrorMsg, Source.m_szErrorMsg);

  return *this;
}

// destructor
CMPAException::~CMPAException() {
  free(m_szFile);
  free(m_szFunction);
  delete[] m_szErrorMsg;
}

// should be in resource file for multi language applications
LPCTSTR CMPAException::m_szErrors[static_cast<int>(
    CMPAException::ErrorIDs::NumIDs)] = {
    _T("Can't open the file."),
    _T("Can't set file position."),
    _T("Can't read from file."),
    _T("No VBR Header found."),
    _T("Incomplete VBR Header."),
    _T("No frame found within tolerance range."),
    _T("No frame found before end of file was reached."),
    _T("Header corrupt"),
    _T("Free Bitrate currently not supported"),
    _T("Incompatible Header"),
    _T("Corrupt Lyrics3 Tag")};

constexpr unsigned MAX_ERR_LENGTH{256};

LPCTSTR CMPAException::GetErrorDescription() {
  if (!m_szErrorMsg) {
    m_szErrorMsg = new char[MAX_ERR_LENGTH];
    m_szErrorMsg[0] = '\0';

    char help[MAX_ERR_LENGTH];

    snprintf(help, MAX_ERR_LENGTH, "%s: '%s'\n%s\n%s",
             m_szFunction ? m_szFunction : "N/A", m_szFile ? m_szFile : "N/A",
             m_szErrors[static_cast<int>(m_ErrorID)],
             m_bGetLastError
                 ? std::system_category().message(GetLastSystemError()).c_str()
                 : "N/A");
    // make sure string is null-terminated
    m_szErrorMsg[MAX_ERR_LENGTH - 1] = '\0';
  }

  return m_szErrorMsg;
}