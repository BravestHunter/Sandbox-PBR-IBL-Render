#pragma once

#include "Sandbox/file/i_file_manager.h"

namespace sandbox_file
{
  class SandboxFileManager : public IFileManager
  {
   public:
    SandboxFileManager();
    virtual ~SandboxFileManager() = default;

    virtual sandbox_utils::OperationResult LoadFile(const std::string path, std::string &data) override;
    virtual sandbox_utils::OperationResult LoadFile(const std::string path, std::stringstream &data_stream) override;
    virtual sandbox_utils::OperationResult SaveFile(const std::string path, const std::string &data) override;
    virtual sandbox_utils::OperationResult SaveFile(const std::string path, const std::stringstream &data_stream) override;
    virtual sandbox_utils::OperationResult LoadImage(const std::string path, sandbox_utils::Image &image, bool flip = false) override;
    virtual sandbox_utils::OperationResult LoadFloatImage(const std::string path, sandbox_utils::FloatImage &image, bool flip = false) override;
  };
}
