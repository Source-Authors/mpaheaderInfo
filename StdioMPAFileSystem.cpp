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

#include "StdioMPAFileSystem.h"

#include <cstdio>

namespace {

[[nodiscard]] int MapMPAFileSeekType(MPAFileSeekType seek_type) {
  switch (seek_type) {
    case MPAFileSeekType::kCurrent:
      return SEEK_CUR;
    case MPAFileSeekType::kEnd:
      return SEEK_END;
    case MPAFileSeekType::kSet:
      return SEEK_SET;
    default:
      return SEEK_CUR;
  }
}

}  // namespace

StdioMpaFile::StdioMpaFile(const char *path, const char *mode)
    : m_file{::fopen(path, mode)} {}

StdioMpaFile::~StdioMpaFile() {
  if (m_file) ::fclose(m_file);
}

size_t StdioMpaFile::Read(void *buffer, size_t size) {
  return ::fread(buffer, 1, size, m_file);
}

int StdioMpaFile::Seek(long offset, MPAFileSeekType seek_type) {
  return ::fseek(m_file, offset, MapMPAFileSeekType(seek_type));
}
long StdioMpaFile::Tell() { return ::ftell(m_file); }

int StdioMpaFile::Error() { return ::ferror(m_file); }

std::unique_ptr<IMPAFile> StdioMPAFileSystem::Open(const char *path,
                                                   const char *mode) {
  return std::make_unique<StdioMpaFile>(path, mode);
}
