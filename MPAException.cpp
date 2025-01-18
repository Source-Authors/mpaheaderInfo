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

namespace {

// AddressSanitizer expects non-nullptr in strdup, so wrap it.
char* mpa_strdup(const char* source) {
  return source ? strdup(source) : nullptr;
}

}  // namespace

CMPAException::CMPAException(Error id, LPCTSTR file, LPCTSTR function,
                             int last_error, LPCTSTR context)
    : m_id(id), m_last_error(last_error), m_error_msg(nullptr) {
  m_file = mpa_strdup(file);
  m_function = mpa_strdup(function);
  m_context = mpa_strdup(context);
}

// copy constructor (necessary for exception throwing without pointers)
CMPAException::CMPAException(const CMPAException& s) {
  m_id = s.m_id;
  m_last_error = s.m_last_error;
  m_file = mpa_strdup(s.m_file);
  m_function = mpa_strdup(s.m_function);
  m_context = mpa_strdup(s.m_context);

  if (s.m_error_msg) {
    const size_t maxSize = strlen(s.m_error_msg) + 1;
    m_error_msg = new char[maxSize];
    strcpy(m_error_msg, s.m_error_msg);
  } else {
    m_error_msg = nullptr;
  }
}

CMPAException& CMPAException::operator=(CMPAException s) {
  std::swap(m_id, s.m_id);
  std::swap(m_last_error, s.m_last_error);
  std::swap(m_function, s.m_function);
  std::swap(m_file, s.m_file);
  std::swap(m_error_msg, s.m_error_msg);
  std::swap(m_context, s.m_context);

  return *this;
}

// destructor
CMPAException::~CMPAException() {
  delete[] m_error_msg;
  free(m_context);
  free(m_function);
  free(m_file);
}

// should be in resource file for multi language applications
constexpr LPCTSTR CMPAException::m_error_id_2_msg[static_cast<int>(
    CMPAException::Error::MaxError)] = {
    _T("Can't open the file."),
    _T("Can't set file position."),
    _T("Can't read from file."),
    _T("No VBR Header found."),
    _T("Incomplete VBR Header."),
    _T("No frame found within tolerance range."),
    _T("No frame found before end of file was reached."),
    _T("Header is corrupt."),
    _T("Free Bitrate currently not supported."),
    _T("Incompatible Header."),
    _T("Corrupt Lyrics3 Tag.")};

LPCTSTR CMPAException::GetErrorDescription() const {
  constexpr unsigned MAX_ERR_LENGTH{256};

  if (!m_error_msg) {
    m_error_msg = new char[MAX_ERR_LENGTH];
    snprintf(m_error_msg, MAX_ERR_LENGTH,
             "At %s for '%s'\n\n%s\n%s\n\nSystem error: %s",
             m_function ? m_function : "N/A", m_file ? m_file : "N/A",
             m_error_id_2_msg[static_cast<int>(m_id)],
             m_context ? m_context : "N/A",
             m_last_error ? std::system_category().message(m_last_error).c_str()
                          : "N/A");
    // make sure string is null-terminated
    m_error_msg[MAX_ERR_LENGTH - 1] = '\0';
  }

  return m_error_msg;
}