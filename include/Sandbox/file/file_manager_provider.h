#pragma once

#include <memory>

#include "Sandbox/file/i_file_manager.h"
#include "Sandbox/utils/operation_result.h"

namespace sandbox_file
{
  class FileManagerProvider
  {
   public:
    static sandbox_utils::OperationResult SetFileManager(std::shared_ptr<IFileManager> file_manager);
    static std::shared_ptr<IFileManager> GetFileManager();

   private:
    static std::shared_ptr<IFileManager> file_manager_;
    static bool is_set_;
  };
}
