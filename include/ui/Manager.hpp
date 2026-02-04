/*

 PHI
 2026/01/01

 Phi C++ Project
 include/ui/Manager.hpp

 Zevi Berlin

*/

#ifndef MANAGER_HPP
#define MANAGER_HPP

#include <string>
#include <memory>
#include <vector>
#include <thread>
#include <chrono>
#include <tuple>

#include <sys/ioctl.h>
#include <unistd.h>

#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/color.hpp>

#include "database/Database.hpp"
#include "database/structs.hpp"
#include "ui/color_defs.hpp"
#include "ui/utils.hpp"
#include "ui/constants.hpp"
#include "utils/str_utils.hpp"

//---------> [ Config. Separator ] <---------\\ 

namespace phi::ui {

constexpr int COLS = 150;
constexpr int ROWS = 35;

// winsize.ws_row and winsize.ws_col
inline struct winsize getTerminalSize() {
  struct winsize size{};
  ioctl(STDOUT_FILENO, TIOCGSIZE, &size);  // NOLINT Don't call C vararg funcs: cppcoreguidelines
  return size;
}

//================={ Header Item Separator }=================\\ 

struct Components {
    ftxui::Component login_input;

    ftxui::Component home_button_layout;

    ftxui::Component contacts_menu;
    ftxui::Component contact_page;

    ftxui::Component self_edit;

    ftxui::Component contact_request_menu;
    ftxui::Component error_menu;
};

//================={ Header Item Separator }=================\\ 

enum class Page : unsigned char {
  Login,
  Home,
  EncryptMessage,
  DecryptMessage,
  ContactsMenu,
  EditContact,
  AddContact,
  DeleteContact,
  ContactDoesNotExist,
  EditSelf,
  EditSelfRSA2048,
  EditSelfRSA4096,
  EditSelfKyber512,
  EditSelfKyber768,
  EditSelfAES,
  EditSelfChaCha,
  Screensaver
};

//================={ Header Item Separator }=================\\ 

struct Notification {
    bool show;

    std::string title;
    std::string description;

    ftxui::Color color;

    std::chrono::time_point<std::chrono::steady_clock> expires;
};

//================={ Header Item Separator }=================\\ 

struct State {
    Page page;

    Notification noti;
};

//================={ Header Item Separator }=================\\ 

class Manager {
  private:
    std::shared_ptr<phi::database::Database> DATABASE;

    ftxui::ScreenInteractive screen = ftxui::ScreenInteractive::TerminalOutput();

    State state{Page::Home, {false, "", "", phi::ui::colors::PURPLE_HAZE, {}}};
    Components components;

    //=====[ Declaration Separator ]=====\\ 

    void getContacts();
    //
    std::vector<std::string> contacts;
    std::vector<int> contact_ids;
    //

    void rebuildRoot(ftxui::Component& root) const;

    void loadComponents();

    void addNoti(const std::string& title, const std::string& description,
                 const ftxui::Color& color, double lifespan);

    //=====[ Declaration Separator ]=====\\ 

    phi::database::contact_t selected_contact_t{};
    int selected_contact_id = 0;
    std::string displayable_rsa2048;
    std::string displayable_rsa4096;
    std::string displayable_kyber512;
    std::string displayable_kyber768;

    //

    bool should_exit = false;
    ftxui::Component root = ftxui::Container::Vertical({});

    //

    std::string password;

    //

    phi::database::self_t editable_self;
    std::string self_displayable_rsa2048_pub;
    std::string self_displayable_rsa2048_priv;
    std::string self_displayable_rsa4096_pub;
    std::string self_displayable_rsa4096_priv;
    std::string self_displayable_kyber512_pub;
    std::string self_displayable_kyber512_priv;
    std::string self_displayable_kyber768_pub;
    std::string self_displayable_kyber768_priv;
    std::string self_displayable_aes128;
    std::string self_displayable_aes192;
    std::string self_displayable_aes256;
    std::string self_displayable_chacha20_poly1305;

  public:
    Manager(std::shared_ptr<phi::database::Database> database);

    //=====[ Declaration Separator ]=====\\ 

    void eventLoop();

    // functions below are inside of `Manager_render_functions.cpp`
    ftxui::Element renderLoginUI() const;
    ftxui::Element renderHomeUI() const;
    ftxui::Element renderContactsMenuUI() const;
    ftxui::Element renderContactPageUI(int contact_id) const;
    ftxui::Element contactDoesNotExist() const;
    ftxui::Element renderSelfEditPageUI() const;
    ftxui::Element renderScreensaver() const;  // not this one, `Manager_screensaver.cpp`
    ftxui::Element renderNotification() const;

    // functions below are inside of `Manager_components.cpp`
    void createLoginInput();
    void createHomePageButtons(const ftxui::ButtonOption& bopt);
    void createContactsMenu();
    void createContactEditPage(const ftxui::ButtonOption& bopt);
    void createSelfEditPage(const ftxui::ButtonOption& bopt);
};

}  // namespace phi::ui

#endif /* MANAGER_HPP */
