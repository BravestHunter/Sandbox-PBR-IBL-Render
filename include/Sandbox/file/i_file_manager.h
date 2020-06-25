#pragma once

#include <string>
#include <sstream>

#include "Sandbox/utils/operation_result.h"
#include "Sandbox/utils/image.h"

namespace sandbox_file
{
  class IFileManager
  {
   public:
    IFileManager() = default;
    virtual ~IFileManager() = default;

    virtual sandbox_utils::OperationResult LoadFile(const std::string path, std::string &data) = 0;
    virtual sandbox_utils::OperationResult LoadFile(const std::string path, std::stringstream &data_stream) = 0;
    virtual sandbox_utils::OperationResult SaveFile(const std::string path, const std::string &data) = 0;
    virtual sandbox_utils::OperationResult SaveFile(const std::string path, const std::stringstream &data_stream) = 0;
    virtual sandbox_utils::OperationResult LoadImage(const std::string path, sandbox_utils::Image &image) = 0;
    virtual sandbox_utils::OperationResult FreeImage(sandbox_utils::Image &image) = 0;
  };
}
