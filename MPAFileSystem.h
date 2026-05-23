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

#ifndef MPA_HEADER_INFO_MPA_FILE_SYSTEM_H_
#define MPA_HEADER_INFO_MPA_FILE_SYSTEM_H_

#include <memory>
#include <cstdio>

enum class MPAFileSeekType { kUnknown = 0, kCurrent = 1, kEnd = 2, kSet = 3 };

struct IMPAFile {
  virtual ~IMPAFile() = 0;

  [[nodiscard]] virtual size_t Read(void *buffer, size_t size) = 0;
  [[nodiscard]] virtual int Seek(long offset, MPAFileSeekType seek_type) = 0;
  [[nodiscard]] virtual long Tell() = 0;

  [[nodiscard]] virtual int Error() = 0;
};

struct IMPAFileSystem {
  [[nodiscard]] virtual std::unique_ptr<IMPAFile> Open(const char *path,
                                                       const char *mode) = 0;
};

#endif  // !MPA_HEADER_INFO_MPA_FILE_SYSTEM_H_
