// Copyright (c) 2021, CuprumProject author.
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.

#ifndef _FILE_ACCESS_H_
#define _FILE_ACCESS_H_

#include <fstream>
#include <functional>
#include <ostream>
#include <optional>

namespace fsys {

class File {
 public:
  enum class Attributes {
    NORMAL_ATTRIBUTES = 0,
    DELETE_ON_CLOSE = 1
  };

  static constexpr int UntilEnd = 0;

  File() : fileStream_() {};
  File(File& file);

  ~File() = default;

  void ChangeStream(std::fstream&& newStream) noexcept { 
    fileStream_ = std::move(newStream); 
    }

  bool Close();

  constexpr std::fstream* fileStream() { return &fileStream_; }

  void SetAttributes(File::Attributes newAttributes) {
    attributes_ = newAttributes;
  }

  int Size();

  std::optional<unsigned char*> Read();
  std::optional<unsigned char*> Read(int s, int e);

  void Write(const char* ptr);
  void Write(char* ptr);
  void Write(int d);

  File* operator=(const File& other);

 private:
  std::fstream fileStream_; 

  Attributes attributes_;
};

// Create single file trace.
// this function returns independent file.
File CreateSingleFileTrace(const char* path);

} // namespace filesystem

#endif // _FILE_ACCESS_H_