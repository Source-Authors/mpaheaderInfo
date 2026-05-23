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

#include "MPAFileStream.h"

#include "Platform.h"
#include "MPAException.h"
#include "MPAEndOfFileException.h"

// 1KB is initial buffersize
const unsigned CMPAFileStream::INIT_BUFFERSIZE = 1024U;

CMPAFileStream::CMPAFileStream(LPCTSTR file_name, IMPAFileSystem *file_system)
    : CMPAStream(file_name), m_dwOffset(0) {
  // open with CreateFile (no limitation of 128byte filename length, like in
  // mmioOpen)
  m_hFile = file_system->Open(file_name, "rb");

  if (!m_hFile) {
    throw CMPAException{CMPAException::Error::ErrOpenFile, file_name,
                        _T("CMPAFileStream ctor (fopen)"), GetLastSystemError(),
                        _T("Unable to open file in read mode.")};
  }

  Init();
}

CMPAFileStream::CMPAFileStream(LPCTSTR file_name, std::unique_ptr<IMPAFile> &&hFile)
    : CMPAStream(file_name), m_hFile(std::move(hFile)), m_dwOffset(0) {
  Init();
}

void CMPAFileStream::Init() {
  m_dwBufferSize = INIT_BUFFERSIZE;
  // fill buffer for first time
  m_pBuffer = new unsigned char[m_dwBufferSize];

  FillBuffer(m_dwOffset, m_dwBufferSize, false);
}

CMPAFileStream::~CMPAFileStream() {
  delete[] m_pBuffer;
}

// set file position
void CMPAFileStream::SetPosition(unsigned offset) const {
  const int result = m_hFile->Seek(offset, MPAFileSeekType::kSet);

  if (result != 0) {
    throw CMPAException{CMPAException::Error::ErrSetPosition, m_szFile,
                        _T("CMPAFileStream::SetPosition (fseek)"),
                        GetLastSystemError()};
  }
}

unsigned char* CMPAFileStream::ReadBytes(unsigned size, unsigned& offset,
                                         bool move_offset,
                                         bool should_reverse) const {
  // enough bytes in buffer, otherwise read from file
  if (offset < m_dwOffset || m_dwOffset + m_dwBufferSize < offset + size) {
    if (!FillBuffer(offset, size, should_reverse)) {
      throw CMPAEndOfFileException{m_szFile};
    }

    if (move_offset) offset += size;

    return m_pBuffer;
  }

  unsigned char* buffer{m_pBuffer + (offset - m_dwOffset)};

  if (move_offset) offset += size;

  return buffer;
}

unsigned CMPAFileStream::GetSize() const {
  const long start_pos{m_hFile->Tell()};
  if (start_pos == -1L) {
    throw CMPAException{CMPAException::Error::ErrReadFile, m_szFile,
                        _T("CMPAFileStream::GetSize (ftell)"),
                        GetLastSystemError()};
  }

  if (m_hFile->Seek(0L, MPAFileSeekType::kEnd)) {
    (void)m_hFile->Seek(start_pos, MPAFileSeekType::kSet);
    throw CMPAException{CMPAException::Error::ErrReadFile, m_szFile,
                        _T("CMPAFileStream::GetSize (fseek)"),
                        GetLastSystemError()};
  }

  // ftell and _ftelli64 return the current file position.
  // The value returned by ftell and _ftelli64 may not reflect the physical byte
  // offset for streams opened in text mode, because text mode causes carriage
  // return-line feed translation.
  const long size{m_hFile->Tell()};

  if (size == -1L) {
    throw CMPAException{CMPAException::Error::ErrReadFile, m_szFile,
                        _T("CMPAFileStream::GetSize (ftell)"),
                        GetLastSystemError()};
  }

  if (m_hFile->Seek(start_pos, MPAFileSeekType::kSet)) {
    throw CMPAException{CMPAException::Error::ErrReadFile, m_szFile,
                        _T("CMPAFileStream::GetSize (fseek)"), GetLastSystemError()};
  }

  return size;
}

// fills internal buffer, returns false if EOF is reached, otherwise true.
// Throws exceptions
bool CMPAFileStream::FillBuffer(unsigned offset, unsigned size,
                                bool should_reverse) const {
  // calc new buffer size
  if (size > m_dwBufferSize) {
    m_dwBufferSize = size;

    // release old buffer
    delete[] m_pBuffer;

    // reserve new buffer
    m_pBuffer = new unsigned char[m_dwBufferSize];
  }

  if (should_reverse) {
    if (offset + size < m_dwBufferSize)
      offset = 0;
    else
      offset = offset + size - m_dwBufferSize;
  }

  // read <m_dwBufferSize> bytes from offset <offset>
  m_dwBufferSize = Read(m_pBuffer, offset, m_dwBufferSize);

  // set new offset
  m_dwOffset = offset;

  // false if eof.
  return m_dwBufferSize >= size;
}

// read from file, return number of bytes read
unsigned CMPAFileStream::Read(void* data, unsigned offset,
                              unsigned size) const {
  // set position first
  SetPosition(offset);

  const size_t bytes_read = m_hFile->Read(data, size);
  if (bytes_read < size && m_hFile->Error())
    throw CMPAException{CMPAException::Error::ErrReadFile, m_szFile,
                        _T("CMPAFileStream::Read (fread)"), GetLastSystemError()};

  // Safe as size is unsigned.
  return static_cast<unsigned>(bytes_read);
}