#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>

struct settings
{
  std::string output_file;
  unsigned int top_packages;

  settings() : output_file("/tmp/topdeb.out"), top_packages(50) {}
};

#endif
