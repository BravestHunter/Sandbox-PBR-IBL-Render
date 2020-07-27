#include "Sandbox/file/sandbox_file_manager.h"

#include <fstream>

//#include <SOIL.h>
#include "stb_image.h"

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

  sandbox_utils::OperationResult SandboxFileManager::LoadImage(const std::string path, sandbox_utils::Image &image, bool flip)
  {
    unsigned char *image_data;
    try
    {
      stbi_set_flip_vertically_on_load(flip);

      int channels;
      image_data = stbi_load(path.c_str(), &image.width, &image.height, &channels, 0);
      if (!image_data)
      {
        return sandbox_utils::OperationResult::FAILURE;
      }

      int size = image.width * image.height * channels;
      image.data = std::vector<unsigned char>(size);
      memcpy(&image.data.at(0), image_data, size);
      image.channels = channels;

      stbi_image_free(image_data);
      stbi_set_flip_vertically_on_load(false);
    }
    catch (...)
    {
      // log error

      //stbi_image_free(image_data);

      return sandbox_utils::OperationResult::FAILURE;
    }

    return sandbox_utils::OperationResult::SUCCESS;
  }

  sandbox_utils::OperationResult SandboxFileManager::LoadFloatImage(const std::string path, sandbox_utils::FloatImage &image, bool flip)
  {
    float *image_data;
    try
    {
      stbi_set_flip_vertically_on_load(flip);

      int channels;
      image_data = stbi_loadf(path.c_str(), &image.width, &image.height, &channels, 0);
      if (!image_data)
      {
        return sandbox_utils::OperationResult::FAILURE;
      }

      int size = image.width * image.height * channels * sizeof(float);
      image.data = std::vector<float>(size);
      memcpy(&image.data.at(0), image_data, size);
      image.channels = channels;

      stbi_image_free(image_data);
      stbi_set_flip_vertically_on_load(false);
    }
    catch (...)
    {
      // log error

      //stbi_image_free(image_data);

      return sandbox_utils::OperationResult::FAILURE;
    }

    return sandbox_utils::OperationResult::SUCCESS;
  }
}
