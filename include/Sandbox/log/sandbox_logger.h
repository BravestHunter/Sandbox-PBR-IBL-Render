#pragma once

#include <string>
#include <memory>
#include <sstream>

#include "Sandbox/log/i_logger.h"

namespace sandbox_log
{
  class SandboxLogger : public ILogger
  {
   public:
    SandboxLogger(LogLevel level = LogLevel::DEBUG) : ILogger(level) {}
    virtual ~SandboxLogger() = default;

    bool GetConsoleOutputUsage() const;
    void SetConsoleOutputUsage(bool mode);
    bool GetLogfileOutputUsage() const;
    void SetLogfileOutputUsage(bool mode);

    bool GetEndlinesUsage();
    void SetEndlinesUsage(bool mode);
    std::string GetMessageFormat() const;
    void SetMessageFormat(std::string format);

    std::string GetLogfilePath() const;
    void SetLogfilePath(std::string path);
    unsigned int GetMaxBufferLength() const;
    void SetMaxBufferLength(unsigned int max);

    virtual void LogMessage(const std::string module_name, const std::string operation_name, const std::string &description, LogLevel message_level) override;
    virtual void LogRawMessage(const std::string &message, LogLevel message_level) override;

   private:
    bool use_console_output_ = true;
    bool use_logfile_output_ = false;

    bool use_endlines_ = true;
    std::string message_format_ = "[%L] - [%M]: %O - %D. [%T]";

    unsigned int max_buffer_length_ = 200000;
    std::string logfile_path_ = "sandbox.log";
    
    std::shared_ptr<std::stringstream> front_buffer_;
    std::shared_ptr<std::stringstream> back_buffer_;

    void CheckBuffer();
    void SwapBuffers();
    void DrainBackBuffer();
  };
}
