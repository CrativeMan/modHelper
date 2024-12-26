#ifndef USERINTERFACE_HPP
#define USERINTERFACE_HPP

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <string>
#include <vector>

#include "logger.hpp"
#include "utils.hpp"

typedef struct {
  unsigned int id;
  std::string title;
  std::string date;
  std::string author;
} Mod;

typedef enum {
  W_MAIN,
  W_MOD,
  W_EXIT,
} W_STATE;

class UserInterface {
public:
  std::vector<Mod> mod_list;
  W_STATE state = W_MAIN;

  UserInterface(GLFWwindow *window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui::StyleColorsClassic();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    Logger::info("MH", "ImGui Initialized");
  }

  bool input() {
    ImGuiIO &io = ImGui::GetIO();
    if (io.KeysDown[GLFW_KEY_ESCAPE]) {
      return false;
    }
    return true;
  }

  bool render() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();

    ImGuiIO &io = ImGui::GetIO();
    ImGui::NewFrame();

    bool cont = true;
    switch (state) {
    case W_MAIN:
      cont = w_MainWindow(io);
      break;
    case W_MOD:
      break;
    case W_EXIT:
      return false;
    default:
      break;
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    return cont;
  }

private:
  ImVec2 menu_button_size = ImVec2(360, 40);

  void mainWindow_PopUps() {
    ImGuiIO &io = ImGui::GetIO();
    ImVec2 center = ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f);
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    if (ImGui::BeginPopupModal("New Project", NULL,
                               ImGuiWindowFlags_AlwaysAutoResize)) {
      ImGui::Text("Create a new project.");

      static char title[128] = "";
      ImGui::InputText("Title", title, IM_ARRAYSIZE(title));

      static char author[128] = "";
      ImGui::InputText("Author", author, IM_ARRAYSIZE(author));

      if (ImGui::Button("Create")) {
        if (!isModInfoValid(title, author)) {
          Logger::warn("MH", "Invalid mod info");
        } else {
          Logger::info("MH", "Creating new mod project: %s - %s", title,
                       author);
          Mod mod;
          mod.id = mod_list.size();
          mod.title = title;
          mod.author = author;
          mod.date = util::getDateAsString();
          mod_list.push_back(mod);
          ImGui::CloseCurrentPopup();
        }
      }
      ImGui::SameLine();
      if (ImGui::Button("Cancel")) {
        ImGui::CloseCurrentPopup();
      }

      ImGui::EndPopup();
    }
  }

  bool w_MainWindow(ImGuiIO &io) {
    bool cont = true;
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y));
    ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_MenuBar;

    ImGui::Begin("Full Screen Window", NULL, flags);

    mainWindow_PopUps();
    // cont = w_MainWindow_MenuBar();

    // Column 1
    ImGui::Columns(2, NULL, false);
    if (ImGui::Button("Create new mod project", menu_button_size)) {
      ImGui::OpenPopup("New Project");
    }

    // Column 2
    ImGui::NextColumn();
    for (long unsigned int i = 0; i < mod_list.size(); i++) {
      std::string mod_info = mod_list[i].title + " - " + mod_list[i].date +
                             " - " + mod_list[i].author;
      if (ImGui::Button(mod_info.c_str(), menu_button_size)) {
        Logger::info("MH", mod_info.c_str());
      }
    }

    ImGui::End();
    return cont;
  };

  bool isModInfoValid(std::string title, std::string author) {
    if (title.empty() || author.empty()) {
      return false;
    }
    if (title.length() > 128 || author.length() > 128) {
      return false;
    }
    for (long unsigned int i = 0; i < mod_list.size(); i++) {
      if (mod_list[i].title == title) {
        return false;
      }
    }
    return true;
  }
};
#endif // USERINTERFACE_HPP
