#ifndef CMD_H
#define CMD_H

#include <iostream>
#include <vector>

namespace cmd {
  enum class cmdtype  { request, response };
  enum class cmdid  { dumptop, dumpbottom, stop };
  struct cmd {
    cmdtype type;
    cmdid id;
    unsigned int data;
  };

static  void dump_cmd(const struct cmd cmd) {
    switch (cmd.type) {
      case cmdtype::request:
        std::cout << "cmdtype request" << std::endl;
        break;
      case cmdtype::response:
        std::cout << "cmdtype response" << std::endl;
        break;
      default:
        std::cout << "Unkown" << std::endl;
        break;
    }
    switch (cmd.id) {
      case cmdid::dumptop:
        std::cout << "cmdid dumptop" << std::endl;
        break;
      case cmdid::dumpbottom:
        std::cout << "cmdid dumpbottom" << std::endl;
        break;
      default:
        std::cout << "Unkown" << std::endl;
        break;
    }
    std::cout << "data: " << cmd.data << std::endl;
  }

static   bool buffer_to_cmd(const std::vector<char>& buffer, cmd& cmd) {
    char* walk = reinterpret_cast<char*>(&cmd);
    for (size_t i = 0; i < sizeof(cmd); i++) {
      *walk = buffer[i];
      walk++;
    }
    dump_cmd(cmd);
    return true;
  }

}

#endif
