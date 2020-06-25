#include "Sandbox/file/file_manager_provider.h"

#include "Sandbox/file/sandbox_file_manager.h"

namespace sandbox_file
{
  std::shared_ptr<IFileManager> FileManagerProvider::file_manager_;
  bool FileManagerProvider::is_set_(false);

  sandbox_utils::OperationResult FileManagerProvider::SetFileManager(std::shared_ptr<IFileManager> file_manager)
  {
    if (is_set_ == false)
    {
      FileManagerProvider::file_manager_ = file_manager;
      is_set_ = true;
      return sandbox_utils::OperationResult::SUCCESS;
    }

    return sandbox_utils::OperationResult::FAILURE;
  }

  std::shared_ptr<IFileManager> FileManagerProvider::GetFileManager()
  {
    if (!is_set_)
    {
      SetFileManager(std::static_pointer_cast<IFileManager>(std::make_shared<SandboxFileManager>()));
    }

    return FileManagerProvider::file_manager_;
  }
}
