#include "dpkg.h"
#include "systemtap.h"
#include "dllist.h"
#include "settings.h"
#include <iostream>
#include <thread>
#include <unistd.h>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

bool parse_command_line(int argc, char* argv[], settings* sett) {
  po::options_description desc("Allowed options");
  bool ret = true;

  desc.add_options()
    ("help", "produce help message")
    ("output_file", po::value<std::string>(), "filename full path")
    ("top_packages", po::value<unsigned int>(), "number of top packages");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if(vm.count("help")) {
    std::cout << desc << std::endl;
    ret = false;
  }

  if(vm.count("output_file")) {
    sett->output_file = vm["output_file"].as<std::string>();
  }

  if(vm.count("top_packages")) {
    sett->top_packages = vm["top_packages"].as<unsigned int>();
  }

  return ret;
}

int main(int argc, char* argv[]) {
  settings sett;

  if(!parse_command_line(argc, argv, &sett))
  {
    exit(EXIT_SUCCESS);
  }

  boost::asio::io_service io_service;

  dpkg packages(io_service, sett);

  systemtap files(
      io_service,
      [&] (std::string line) { packages.openfile(line); } 
   );

  if (!files.start()) {
   exit(-1);
  }

  io_service.run();
}
