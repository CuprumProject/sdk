// Copyright (c) 2021, CuprumProject author.
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.

#include <string>
#include <sstream>
#include <memory>

#include "core/syntax/syntax.h"
#include "core/virtual/register.h"

#include "base/commandline/cmd_line.h"
#include "base/file/file_access.h"

using CommandLine::HasOption;
using CommandLine::GetValueFromOption;

using fsys::CreateSingleFileTrace;
using fsys::File;

int TargetTargetAndCompile(const char*);

int main(int argc, char* argv[]) {
  std::unique_ptr<const char*> compileObject = (
    HasOption(argc, argv, "-t") ? 
      std::make_unique<const char*>(GetValueFromOption(argc, argv, "-t")) :
      nullptr
  );

  if (compileObject == nullptr || !*compileObject) {
    fprintf(stderr, "An error occurred loading file\n");
    if (compileObject == nullptr)
      fprintf(stderr, "Hint: Use -t option to set the file to compile.");

    return 1;
  }

  return DetermineTargetAndCompile(*compileObject);
}

int TargetTargetAndCompile(const char* compileObjectFileName) {
  File compileObjectFile = CreateSingleFileTrace(compileObjectFileName);
  File textout = CreateSingleFileTrace("program.e");
  
  const std::string contents = reinterpret_cast<char*>(*(compileObjectFile.Read()));

  std::string line;
  std::string modified;
  std::string f;

  std::stringstream StringStream(contents);

  int l = 0;

  while (std::getline(StringStream, line, '\n')) {
    if (IsSystemReservedSyntaxType(line.c_str())) {
      if (line.length() <= 3) {
        char reason[1024] = {0};
        sprintf(reason, "%s%d%c%s", "Line ", l, ':', " invalid syntax");
        ErrorExit(reason);
      }
      
      modified += GetOperator(line.c_str());

      switch(GetOperator(line.c_str())) {
      // mov
       case 1:
        f = TookFirstArgument(line.c_str());
        
        if (f == kMsa)
          modified += 1;
        else
          if (f == kRsa)
            modified += 2;
          else
            if (f == kAsa)
              modified += 3;
            else
              ErrorExit("Error: Unknown register");

        modified += GetSecondArgumentLength(line.c_str());
        modified += TookSecondArgument(line.c_str());
        break;

      //int
       case 6:
        modified += TookFirstArgument(line.c_str());
        break;
      
       default:
        ErrorExit("Error: Unknown operation");
      }

      textout.Write(modified.c_str());
      modified = "";
    }
    l++;
  }

  printf("compile success");
  return 0;
}
