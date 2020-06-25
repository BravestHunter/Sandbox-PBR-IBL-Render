#include "Sandbox/log/i_logger.h"

namespace sandbox_log
{
  std::string LogLevelToString(LogLevel level)
  {
    switch (level)
    {
    case LogLevel::NONE:
      return "NONE";
    case LogLevel::CRITICAL:
      return "CRITICAL";
    case LogLevel::ERROR:
      return "ERROR";
    case LogLevel::WARNING:
      return "WARNING";
    case LogLevel::INFO:
      return "INFO";
    case LogLevel::DEBUG:
      return "DEBUG";
    default:
      return "UNKNOWN";
    }
  }

  void ILogger::SetLogLevel(LogLevel level)
  {
    log_level_ = level;
  }

  LogLevel ILogger::GetLogLevel() const
  {
    return log_level_;
  }
}
