#pragma once

#include <stdint.h>

namespace sandbox_utils
{
  struct Image
  {
   public:
    unsigned char *data_;
    int width_;
    int height_;
    uint8_t channels_;
  };
}