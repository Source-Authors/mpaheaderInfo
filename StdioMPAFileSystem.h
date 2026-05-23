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

#ifndef MPA_HEADER_INFO_STDIO_MPA_FILE_SYSTEM_H_
#define MPA_HEADER_INFO_STDIO_MPA_FILE_SYSTEM_H_

#include "MPAFileSystem.h"

#include <cstdio>

class StdioMpaFile : public IMPAFile {
 public:
  StdioMpaFile(const char *path, const char *mode);
  ~StdioMpaFile();

  StdioMpaFile(StdioMpaFile &) = delete;
  StdioMpaFile(StdioMpaFile &&f) noexcept : m_file{std::move(f.m_file)} {
    f.m_file = nullptr;
  }
  StdioMpaFile &operator=(StdioMpaFile &) = delete;
  StdioMpaFile &operator=(StdioMpaFile &&f) noexcept {
    std::swap(m_file, f.m_file);
    return *this;
  }

  [[nodiscard]] size_t Read(void *buffer, size_t size) override;
  [[nodiscard]] int Seek(long offset, MPAFileSeekType seek_type) override;
  [[nodiscard]] long Tell() override;
  [[nodiscard]] int Error() override;

 private:
  FILE *m_file;
};

class StdioMPAFileSystem : public IMPAFileSystem {
 public:
  [[nodiscard]] std::unique_ptr<IMPAFile> Open(const char *path,
                                               const char *mode) override;
};

#endif  // !MPA_HEADER_INFO_STDIO_MPA_FILE_SYSTEM_H_
