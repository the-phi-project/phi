/*

  PHI
  2025/09/20

  Phi C++ Project
  src/main.cpp

  Zevi Berlin

*/

#include <iostream>
#include <string>
#include <memory>
#include <cstring>

#include "termcolor/termcolor.hpp"

#include "database/Database.hpp"
#include "ui/Manager.hpp"
#include "parser.hpp"
#include "utils/misc_utils.hpp"
#include "utils/str_utils.hpp"

#define RESET_TERM "\x1b[2J\x1b[H"

namespace tmc = termcolor;

int main(int argc, char** argv) {
  bool cli_mode = (argc > 1);

  /**** GLOBAL CONFIG ****/

  int erc = 0;

  // TODO: Add features for updates (~/.phi/updates/) and a backup database (~/.phi/__backup.db)
  if (!createDataFiles(erc)) {
    std::cout << tmc::bright_red;
    if (erc == 1) {
      std::cout << "⛔️ Failed to create data file directory (~/.phi/) ⛔️\n";
    }
    if (erc == 2) {
      std::cout << "⛔️ Failed to create database file (~/.phi/main.db) ⛔️\n";
    }
    std::cout << tmc::reset;
    return erc;
  }  // exit if not valid permissions

  /**/

  const std::shared_ptr<phi::database::Database> DATABASE =
    std::make_shared<phi::database::Database>();

  /**** ****/

  if (cli_mode) {
    return phi::parse_commands(argc, argv, DATABASE);
  }

  auto size = phi::ui::getTerminalSize();
  if (size.ws_col < phi::ui::COLS || size.ws_row < phi::ui::ROWS) {
    std::cout << tmc::bright_red << "⛔️ Phi requires a terminal size of at least " << phi::ui::COLS
              << "x" << phi::ui::ROWS << " to properly render the TUI ⛔️\n"
              << tmc::reset;
    return 1;
  }

  const std::shared_ptr<phi::ui::Manager> MANAGER = std::make_shared<phi::ui::Manager>(DATABASE);

  std::cout << RESET_TERM << std::flush;
  MANAGER->eventLoop();
  std::cout << RESET_TERM << std::flush;

  return 0;
}