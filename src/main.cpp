#include <GL/glew.h>

#include "globals.hpp"
#include "logger.hpp"
#include "userInterface.hpp"

LOG_LEVEL Logger::level = L_VERBOSE;
Window window;

void frameBufferSizeCallback(GLFWwindow *window, int width, int height) {
  (void)window;
  glViewport(0, 0, width, height);
}

void errorCallback(int error, const char *description) {
  (void)error;
  Logger::error("GLFW Error: %s", description);
}

int main() {
  Logger::testLogger();
  if (glfwInit()) {
    Logger::info("MH", "Initialized GLFW");
  } else {
    Logger::critical("MH", "Failed to init GLFW");
    exit(EXIT_FAILURE);
  }

  glfwSetErrorCallback(errorCallback);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  window.width = 800;
  window.height = 600;
  window.title = "Modding helper";
  window.id = glfwCreateWindow(window.width, window.height,
                               window.title.c_str(), NULL, NULL);
  if (window.id == NULL) {
    Logger::critical("MH", "Failed to init glfw window");
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  Logger::info("MH", "Initialized window '%d'", window.id);

  glfwMakeContextCurrent(window.id);
  glfwSetFramebufferSizeCallback(window.id, frameBufferSizeCallback);
  glViewport(0, 0, window.width, window.height);

  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK) {
    Logger::critical("MH", "Failed to init GLEW");
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  UserInterface ui(window.id);

  bool cont = true;

  while (!glfwWindowShouldClose(window.id)) {
    glfwPollEvents();
    if (glfwGetWindowAttrib(window.id, GLFW_ICONIFIED) != 0) {
      ImGui_ImplGlfw_Sleep(10);
      continue;
    }

    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    cont = ui.render();
    if (cont == false)
      break;

    glfwSwapBuffers(window.id);
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  glfwTerminate();
  Logger::info("MH", "Terminated GLFW");
  return 0;
}
