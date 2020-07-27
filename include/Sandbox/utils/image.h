#pragma once

#include <vector>

namespace sandbox_utils
{
  struct Image
  {
    std::vector<unsigned char> data;
    int width;
    int height;
    int channels;
    int channel_bits;
  };

  struct FloatImage
  {
    std::vector<float> data;
    int width;
    int height;
    int channels;
    int channel_bits;
  };
}
