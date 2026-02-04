/*

 PHI
 2026/01/23

 Phi C++ Project
 src/Manager_components.cpp

 Zevi Berlin

*/

#include "ui/Manager.hpp"

//---------> [ Config. Separator ] <---------\\ 

void phi::ui::Manager::createLoginInput() {
  ftxui::InputOption popt;
  popt.password = true;

  this->components.login_input = ftxui::Input(&(this->password), "", popt);
  this->components.login_input |= ftxui::CatchEvent([&](const ftxui::Event& e) {
    if (e == ftxui::Event::Return) {
      /*if (this->DATABASE->login(this->password)) {
        this->DATABASE->createTables();*/
      this->state.page = phi::ui::Page::Home;
      this->rebuildRoot(this->root); /*
     } else {
       this->should_exit = true;
     }*/
      return true;
    }

    return false;
  });
}

//------------[ Func. Implementation Separator ]------------\\ 

void phi::ui::Manager::createHomePageButtons(const ftxui::ButtonOption& bopt) {
  auto encrypt_button = ftxui::Button(
    "Encrypt a Message",
    [&] {
      this->state.page = phi::ui::Page::ContactsMenu;
      this->getContacts();
      this->rebuildRoot(this->root);
    },
    bopt);

  auto decrypt_button = ftxui::Button(
    "Decrypt a Message", [&] { this->state.page = phi::ui::Page::DecryptMessage; }, bopt);

  auto edit_contact = ftxui::Button(
    "Edit a Contact",
    [&] {
      this->state.page = phi::ui::Page::ContactsMenu;
      this->getContacts();
      this->rebuildRoot(this->root);
    },
    bopt);

  auto add_contact =
    ftxui::Button("Add a Contact", [&] { this->state.page = phi::ui::Page::AddContact; }, bopt);

  auto delete_contact = ftxui::Button(
    "Delete a Contact", [&] { this->state.page = phi::ui::Page::DeleteContact; }, bopt);

  auto edit_self = ftxui::Button(
    "Edit My Keys",
    [&] {
      this->state.page = phi::ui::Page::EditSelf;
      this->rebuildRoot(this->root);
    },
    bopt);

  auto screensaver_button =
    ftxui::Button("Screensaver", [&] { this->state.page = phi::ui::Page::Screensaver; }, bopt);

  auto exit_button = ftxui::Button("Exit", [&] { should_exit = true; }, bopt);

  this->components.home_button_layout =
    ftxui::Container::Vertical({encrypt_button, decrypt_button, edit_contact, add_contact,
                                delete_contact, edit_self, screensaver_button, exit_button});
}

//------------[ Func. Implementation Separator ]------------\\ 

void phi::ui::Manager::createContactsMenu() {
  this->components.contacts_menu =
    ftxui::Menu(&contacts, &this->selected_contact_id) | ftxui::CatchEvent([&](ftxui::Event e) {
      if (e == ftxui::Event::Return) {
        if (!this->DATABASE->getContact(contact_ids.at(this->selected_contact_id),
                                        this->selected_contact_t)) {
          this->state.page = phi::ui::Page::ContactDoesNotExist;
        } else {
          this->state.page = phi::ui::Page::EditContact;
        }
        this->rebuildRoot(root);
        return true;
      }
      return false;
    });
}

//------------[ Func. Implementation Separator ]------------\\ 

void phi::ui::Manager::createContactEditPage(const ftxui::ButtonOption& bopt) {
  ftxui::InputOption ropt;
  ropt.transform = [](ftxui::InputState s) {
    return s.element | ftxui::color(phi::ui::colors::BLUE_BABY) | ftxui::borderRounded |
           ftxui::color(phi::ui::colors::GOLD);
  };

  ftxui::InputOption dopt;
  dopt.transform = [](ftxui::InputState s) {
    return s.element | ftxui::color(phi::ui::colors::BLUE_BABY) | ftxui::borderRounded |
           ftxui::color(phi::ui::colors::GOLD) | ftxui::dim;
  };

#define ENTER_CATCHER \
  ftxui::CatchEvent([&](const ftxui::Event& e) { return e == ftxui::Event::Return; })

#undef ENTER_CATCHER

  auto save_changes = ftxui::Button(
    "Save Changes",
    [&] {
      phi::database::contact_t current;
      if (!this->DATABASE->getContact(this->contact_ids.at(this->selected_contact_id), current)) {
        this->state.page = phi::ui::Page::ContactDoesNotExist;
        this->rebuildRoot(this->root);
      }

      this->addNoti("Changes Saved",
                    "The profile changes made to the contact with ID " +
                      std::to_string(current.id) + " were saved to the database successfully.",
                    phi::ui::colors::GREEN_MINT, 3.0);
    },
    bopt);


  this->components.contact_page = ftxui::Container::Vertical({save_changes});
}

//------------[ Func. Implementation Separator ]------------\\ 

void phi::ui::Manager::createSelfEditPage(const ftxui::ButtonOption& bopt) {
  ftxui::InputOption ropt;
  ropt.transform = [](ftxui::InputState s) {
    return s.element | ftxui::color(phi::ui::colors::BLUE_BABY) | ftxui::borderRounded |
           ftxui::color(phi::ui::colors::GOLD);
  };

  ftxui::InputOption dopt;
  dopt.transform = [](ftxui::InputState s) {
    return s.element | ftxui::color(phi::ui::colors::BLUE_BABY) | ftxui::borderRounded |
           ftxui::color(phi::ui::colors::GOLD) | ftxui::dim;
  };


#define ENTER_CATCHER \
  ftxui::CatchEvent([&](const ftxui::Event& e) { return e == ftxui::Event::Return; })

#undef ENTER_CATCHER

  auto save_changes = ftxui::Button(
    "Save Changes",
    [&] {
      this->DATABASE->updateSelf(this->editable_self);

      this->addNoti("Changes Saved",
                    "The changes made to your profile were saved to the database successfully.",
                    phi::ui::colors::GREEN_MINT, 2.9);
    },
    bopt);


  this->components.self_edit = ftxui::Container::Vertical({save_changes});
}