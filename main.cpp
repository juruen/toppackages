#include "dpkg.h"
#include "systemtap.h"
#include "dllist.h"
#include <iostream>
#include <thread>
#include <unistd.h>

int main() {
  dpkg packages;
  systemtap files;
   
  std::thread dump(
    [&] () { while (1) { sleep(1); packages.dumpTop(); } }   
  );
 
  files.start(
    [&] (std::string line) { packages.openfile(line); }   
  );
 
}
