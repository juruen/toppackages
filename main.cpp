#include "dpkg.h"
#include "systemtap.h"
#include "dllist.h"
#include "settings.h"
#include "logger.h"

#include <iostream>
#include <unistd.h>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

bool parse_command_line(int argc, char* argv[], settings& sett) {
  po::options_description desc("Allowed options");

  desc.add_options()
    ("help,h", "produce help message")
    ("outfile,o", po::value<std::string>(), "filename full path")
    ("top-pkgs,t", po::value<unsigned int>(), "number of top packages");

  po::variables_map vm;

  try {
    po::store(po::parse_command_line(argc, argv, desc), vm);
  } catch (const boost::program_options::unknown_option& ex) {
    std::cout << ex.what() << std::endl;
    std::cout << desc << std::endl;
    exit(EXIT_FAILURE);
  }

  po::notify(vm);

  if(vm.count("help")) {
    std::cout << desc << std::endl;
    return false;;
  }

  if(vm.count("outfile")) {
    sett.output_file = vm["outfile"].as<std::string>();
  }

  if(vm.count("top-pkgs")) {
    sett.top_packages = vm["top-pkgs"].as<unsigned int>();
  }

  return true;
}

int main(int argc, char* argv[]) {
  logging::logger mylog("topdeb", logging::facility::daemon);

  std::clog << "topdeb launched" << std::endl;

  settings sett;

  if(!parse_command_line(argc, argv, sett))
  {
    exit(EXIT_SUCCESS);
  }

  boost::asio::io_service io_service;

  dpkg packages(io_service, sett);

  systemtap files(
      io_service,
      [&] (std::string line) { packages.open_file(line); } 
   );

  if (!files.start()) {
   exit(-1);
  }

  io_service.run();
}
