// Copyright (c) 2021, CuprumProject author.
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.

#include <assert.h>
#include <limits.h>
#include <string.h>

#include <iostream>
#include <fstream>

#include "file_access.h"

using namespace fsys;

File::File(File& file) {
  fileStream_ = std::move(file.fileStream_);
}

bool File::Close() {
  fileStream_.close();
  return 0;
}

File fsys::CreateSingleFileTrace(const char* path) {
  // Create temporary file to create fstream fstream struct.
  // fstream structure will fail if the file does not exist when it is created.
  std::ofstream outStream(path, std::ios::app);
  outStream.close();

  assert(!outStream.fail());

  std::fstream fstream_file(path, std::ios::binary | std::ios::out | std::ios::in);
  File file;

  file.ChangeStream(std::move(fstream_file));
  return file;
}

int File::Size() {
  std::fstream* FileStartPosition = fileStream();

  FileStartPosition->seekg(0, std::ios::end);

  int filesize = FileStartPosition->tellg();
  FileStartPosition->seekg(0, std::ios::beg);

  return filesize;
}

void File::Write(const char* ptr) {
  *fileStream() << ptr;
}

void File::Write(char* ptr) {
  *fileStream() << ptr;
}

void File::Write(int d) {
  *fileStream() << d;
}

std::optional<unsigned char*> File::Read() {
  std::optional<unsigned char*> buf = Read(0, UntilEnd);
  return buf;
}

std::optional<unsigned char*> File::Read(int s, int e) {

  unsigned char* buf = reinterpret_cast<unsigned char*>(calloc(Size() + 1, sizeof(unsigned char)));
  std::fstream* FileStartPosition = fileStream();
  int d = 0;

  if (buf == nullptr)
    return std::nullopt;

  for (int i = s; !FileStartPosition->eof(); ++i) {
    FileStartPosition->read((char*) &d, sizeof(char));
    buf[i] = d;
    d = 0;
    if (i == e && e != 0)
      break; 
  }
  return std::make_optional<unsigned char*>(std::move(buf));
}

File* File::operator=(const File& other) {
  File file = const_cast<File&>(other);
  fileStream_ = std::move(file.fileStream_);
  return this;
}