#include "dpkg.h"
#include "systemtap.h"
#include "dllist.h"
#include <iostream>
#include <thread>
#include <unistd.h>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

int main() {
  boost::asio::io_service io_service;

  dpkg packages(io_service);

  systemtap files(
      io_service,
      [&] (std::string line) { packages.openfile(line); } 
   );
   
  if (!files.start()) {
   exit(-1);
  }

  io_service.run();
}

