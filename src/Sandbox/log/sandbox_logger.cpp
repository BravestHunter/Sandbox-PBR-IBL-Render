#include "Sandbox/log/sandbox_logger.h"

#include <iostream>
#include <regex>

#include "Sandbox/platform/platform_provider.h"
#include "Sandbox/file/file_manager_provider.h"

namespace sandbox_log
{
  bool SandboxLogger::GetConsoleOutputUsage() const
  {
    return use_console_output_;
  }

  void SandboxLogger::SetConsoleOutputUsage(bool mode)
  {
    use_console_output_ = mode;
  }

  bool SandboxLogger::GetLogfileOutputUsage() const
  {
    return use_logfile_output_;
  }

  void SandboxLogger::SetLogfileOutputUsage(bool mode)
  {
    if (mode && !use_logfile_output_)
    {
      front_buffer_ = std::make_shared<std::stringstream>();
      back_buffer_ = std::make_shared<std::stringstream>();
    }

    use_logfile_output_ = mode;
  }

  bool SandboxLogger::GetEndlinesUsage()
  {
    return use_endlines_;
  }

  void SandboxLogger::SetEndlinesUsage(bool mode)
  {
    use_endlines_ = mode;
  }

  std::string SandboxLogger::GetMessageFormat() const
  {
    return message_format_;
  }

  void SandboxLogger::SetMessageFormat(std::string format)
  {
    message_format_ = format;
  }

  std::string SandboxLogger::GetLogfilePath() const
  {
    return logfile_path_;
  }

  void SandboxLogger::SetLogfilePath(std::string path)
  {
    logfile_path_ = path;
  }

  unsigned int SandboxLogger::GetMaxBufferLength() const
  {
    return max_buffer_length_;
  }

  void SandboxLogger::SetMaxBufferLength(unsigned int max)
  {
    max_buffer_length_ = max;
  }

  void SandboxLogger::LogMessage(const std::string module_name, const std::string operation_name, const std::string &description, LogLevel message_level)
  {
    if (message_level <= log_level_)
    {
      std::string message = message_format_;
      message = std::regex_replace(message, std::regex("%L"), LogLevelToString(message_level));
      message = std::regex_replace(message, std::regex("%M"), module_name);
      message = std::regex_replace(message, std::regex("%O"), operation_name);
      message = std::regex_replace(message, std::regex("%D"), description);
      message = std::regex_replace(message, std::regex("%T"), std::to_string(sandbox_platform::PlatformProvider::GetPlatform()->GetTime()));

      if (use_console_output_)
      {
        std::cout << message;
        if (use_endlines_)
          std::cout << std::endl;
      }
      if (use_logfile_output_)
      {
        (*front_buffer_) << message;
        if (use_endlines_)
          (*front_buffer_) << std::endl;

        CheckBuffer();
      }
    }
  }

  void SandboxLogger::LogRawMessage(const std::string &message, LogLevel message_level)
  {
    if (message_level <= log_level_)
    {
      if (use_console_output_)
      {
        std::cout << message;
        if (use_endlines_)
          std::cout << std::endl;
      }
      if (use_logfile_output_)
      {
        (*front_buffer_) << message;
        if (use_endlines_)
          (*front_buffer_) << std::endl;

        CheckBuffer();
      }
    }
  }

  void SandboxLogger::CheckBuffer()
  {
    if (front_buffer_->str().length() > max_buffer_length_) // todo: use faster legth check
    {
      SwapBuffers();
      DrainBackBuffer();
    }
  }

  void SandboxLogger::SwapBuffers()
  {
    front_buffer_.swap(back_buffer_);
  }

  void SandboxLogger::DrainBackBuffer()
  {
    sandbox_file::FileManagerProvider::GetFileManager()->SaveFile(logfile_path_, *back_buffer_);
    back_buffer_->str(""); // todo: change stream clear
  }
}
