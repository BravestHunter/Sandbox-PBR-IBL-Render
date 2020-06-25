#pragma once

#include <string>

namespace sandbox_log
{
  enum class LogLevel
  {
    NONE,
    CRITICAL,
    ERROR,
    WARNING,
    INFO,
    DEBUG
  };

  std::string LogLevelToString(LogLevel level);

  class ILogger
  {
   public:
    ILogger(LogLevel level) : log_level_(level) {}
    virtual ~ILogger() = default;

    void SetLogLevel(LogLevel level);
    LogLevel GetLogLevel() const;

    virtual void LogMessage(const std::string module_name, const std::string operation_name, const std::string &description, LogLevel message_level) = 0;
    virtual void LogRawMessage(const std::string &message, LogLevel message_level) = 0;

   protected:
    LogLevel log_level_;
  };
}
