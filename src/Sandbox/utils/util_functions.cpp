#include "Sandbox/utils/util_functions.h"

namespace sandbox_utils
{
  std::string GetFileExtension(const std::string &filename) 
  {
    int last_dot = filename.find_last_of(".");
    if (last_dot != std::string::npos)
    {
      return filename.substr(last_dot + 1);
    }

    return "";
  }
}
