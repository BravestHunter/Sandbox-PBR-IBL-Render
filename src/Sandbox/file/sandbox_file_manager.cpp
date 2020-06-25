#include "Sandbox/file/sandbox_file_manager.h"

#include <fstream>

#include <SOIL.h>

namespace sandbox_file
{
  std::string module_name = "SandboxFileManager";

  SandboxFileManager::SandboxFileManager()
  {
    // some constructor work
  }

  sandbox_utils::OperationResult SandboxFileManager::LoadFile(const std::string path, std::string &data)
  {
    std::ifstream input_stream;
    std::stringstream string_stream;

    try
    {
      input_stream.open(path.c_str());
      string_stream << input_stream.rdbuf();
      input_stream.close();

      data = string_stream.str();
    }
    catch (std::ifstream::failure& e)
    {
      // log error

      return sandbox_utils::OperationResult::FAILURE;
    }

    return sandbox_utils::OperationResult::SUCCESS;
  }

  sandbox_utils::OperationResult SandboxFileManager::LoadFile(const std::string path, std::stringstream &data_stream)
  {
    std::ifstream input_stream;

    try
    {
      input_stream.open(path.c_str());
      data_stream << input_stream.rdbuf();
      input_stream.close();
    }
    catch (std::ifstream::failure& e)
    {
      // log error

      return sandbox_utils::OperationResult::FAILURE;
    }

    return sandbox_utils::OperationResult::SUCCESS;
  }

  sandbox_utils::OperationResult SandboxFileManager::SaveFile(const std::string path, const std::string &data)
  {
    std::ofstream output_stream;

    try
    {
      output_stream.open(path.c_str());
      output_stream << data;
      output_stream.close();
    }
    catch (std::ifstream::failure& e)
    {
      // log error

      return sandbox_utils::OperationResult::FAILURE;
    }

    return sandbox_utils::OperationResult::SUCCESS;
  }

  sandbox_utils::OperationResult SandboxFileManager::SaveFile(const std::string path, const std::stringstream &data_stream)
  {
    std::ofstream output_stream;

    try
    {
      output_stream.open(path.c_str());
      output_stream << data_stream.rdbuf();
      output_stream.close();
    }
    catch (std::ifstream::failure& e)
    {
      // log error

      return sandbox_utils::OperationResult::FAILURE;
    }

    return sandbox_utils::OperationResult::SUCCESS;
  }

  sandbox_utils::OperationResult SandboxFileManager::LoadImage(const std::string path, sandbox_utils::Image &image)
  {
    try
    {
      int channels;
      image.data_ = SOIL_load_image(path.c_str(), &image.width_, &image.height_, &channels, SOIL_LOAD_AUTO);
      image.channels_ = channels;
    }
    catch (...)
    {
      // log error

      SOIL_free_image_data(image.data_);

      return sandbox_utils::OperationResult::FAILURE;
    }

    return sandbox_utils::OperationResult::SUCCESS;
  }

  sandbox_utils::OperationResult SandboxFileManager::FreeImage(sandbox_utils::Image &image)
  {
    try
    {
      // log error

      SOIL_free_image_data(image.data_);
    }
    catch (...)
    {
      return sandbox_utils::OperationResult::FAILURE;
    }
    
    return sandbox_utils::OperationResult::SUCCESS;
  }
}
