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

#ifndef MPA_HEADER_INFO_MPA_EXCEPTION_H_
#define MPA_HEADER_INFO_MPA_EXCEPTION_H_

// exception class
class CMPAException {
 public:
  enum class Error {
    ErrOpenFile,
    ErrSetPosition,
    ErrReadFile,
    NoVBRHeader,
    IncompleteVBRHeader,
    NoFrameInTolerance,
    EndOfFile,
    HeaderCorrupt,
    FreeBitrate,
    IncompatibleHeader,
    CorruptLyricsTag,
    // this specifies the total number of possible IDs
    MaxError
  };

  explicit CMPAException(Error id, LPCTSTR file = nullptr,
                         LPCTSTR function = nullptr, int last_error = 0,
                         LPCTSTR context = nullptr);
  // copy constructor (necessary because of LPSTR members)
  CMPAException(const CMPAException& s);
  CMPAException& operator=(CMPAException s);

  virtual ~CMPAException();

  Error GetError() const { return m_id; };
  LPCTSTR GetErrorDescription() const;

 private:
  Error m_id;
  int m_last_error;
  LPTSTR m_function;
  LPTSTR m_file;
  mutable LPTSTR m_error_msg;
  LPTSTR m_context;

  static const LPCTSTR
      m_error_id_2_msg[static_cast<int>(CMPAException::Error::MaxError)];
};

#endif  // !MPA_HEADER_INFO_MPA_EXCEPTION_H_