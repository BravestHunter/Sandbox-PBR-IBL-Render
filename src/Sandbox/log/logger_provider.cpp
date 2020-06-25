#include "Sandbox/log/logger_provider.h"

#include <memory>

#include "Sandbox/log/sandbox_logger.h"

namespace sandbox_log
{
  std::shared_ptr<ILogger> LoggerProvider::logger_;
  bool LoggerProvider::is_set_(false);

  sandbox_utils::OperationResult LoggerProvider::SetLogger(std::shared_ptr<ILogger> logger)
  {
    if (!is_set_)
    {
      LoggerProvider::logger_ = logger;
      is_set_ = true;
      return sandbox_utils::OperationResult::SUCCESS;
    }

    return sandbox_utils::OperationResult::FAILURE;
  }

  std::shared_ptr<ILogger> LoggerProvider::GetLogger()
  {
    if (!is_set_)
    {
      SetLogger(std::static_pointer_cast<ILogger>(std::make_shared<SandboxLogger>()));
    }

    return LoggerProvider::logger_;
  }
}
