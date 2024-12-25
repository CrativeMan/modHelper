#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include <GLFW/glfw3.h>
#include <string>
typedef struct {
  GLFWwindow *id;
  int width;
  int height;
  std::string title;
} Window;

#endif // GLOBALS_HPP
