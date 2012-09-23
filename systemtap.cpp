#include <string>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include "systemtap.h"

using namespace std;

namespace {
  const string cmd("stap  opencall.stp");
  const unsigned int line_size = 2048;
};

systemtap::systemtap()
{

}

void systemtap::start(function<void(std::string)> processLine)
{
  FILE* pipe = popen(cmd.c_str(), "r");
  if (!pipe) {
    std::cerr << "Couldn't open pipe for " << cmd << endl;
    return;
  }

  char line[line_size];
  while (fgets(line, line_size, pipe)) {
    line[strlen(line) - 1] = '\0'; // Chomp
    processLine(line);
  }
  pclose(pipe);
}

void systemtap::stop()
{
}
