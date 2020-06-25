#pragma once

#include <memory>

#include "Sandbox/log/i_logger.h"
#include "Sandbox/utils/operation_result.h"

namespace sandbox_log
{
  class LoggerProvider
  {
   public:
    static sandbox_utils::OperationResult SetLogger(std::shared_ptr<ILogger> logger);
    static std::shared_ptr<ILogger> GetLogger();

   private:
    static std::shared_ptr<ILogger> logger_;
    static bool is_set_;
  };
}
