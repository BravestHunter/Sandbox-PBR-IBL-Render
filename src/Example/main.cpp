#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include <fstream>

#include "Sandbox/utils/delegate.h"

#include "Example/application.h"

#include "Sandbox/graphics/model_loader.h"
#include "Sandbox/common/graphics/opengl/opengl_binder.h"


int main(int argc, char** argv)
{
  std::string width_str = "1920";
  std::string height_str = "1080";
  if (argc > 1)
  {
    width_str = std::string(argv[1]);
    height_str = std::string(argv[2]);
  }
  int width = std::stoi(width_str);
  int height = std::stoi(height_str);

  std::cout << "Select scene(1-12):" << std::endl;
  std::string number_str;
  std::cin >> number_str;
  std::cout << std::endl;

  system("cls");

  std::cout << "Use wasd-keys, mouse movement and mouse buttons and to control camera." << std::endl;
  std::cout << "Use Key1 to change polygon mode (scenes 3-12)." << std::endl;
  std::cout << "Use Key2 to on/off point lights (scenes 3-12)." << std::endl;
  std::cout << "Use num-keys(7-9, 4-6, 8-2) to wrap model (scenes 3-12)." << std::endl;
  std::cout << std::endl << "Press escape to close window." << std::endl;

  int result = -1;
  try
  {
    switch (std::stoi(number_str))
    {
    case 1:
      result = GraphicsApplication::RunExample1(width, height);
      break;
    case 2:
      result = GraphicsApplication::RunExample2(width, height);
      break;
    case 3:
      result = GraphicsApplication::RunExample3(width, height);
      break;
    case 4:
      result = GraphicsApplication::RunExample4(width, height);
      break;
    case 5:
      result = GraphicsApplication::RunExample5(width, height);
      break;
    case 6:
      result = GraphicsApplication::RunExample6(width, height);
      break;
    case 7:
      result = GraphicsApplication::RunExample7(width, height);
      break;
    case 8:
      result = GraphicsApplication::RunExample8(width, height);
      break;
    case 9:
      result = GraphicsApplication::RunExample9(width, height);
      break;
    case 10:
      result = GraphicsApplication::RunExample10(width, height);
      break;
    case 11:
      result = GraphicsApplication::RunExample11(width, height);
      break;
    case 12:
      result = GraphicsApplication::RunExample12(width, height);
      break;
    }
  }
  catch (std::exception &e)
  {
    std::cout << e.what() << std::endl;
    std::cin >> number_str;
  }

  return 0;
}