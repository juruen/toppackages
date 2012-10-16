#include "cmdclient.h"
#include "settings.h"
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
    ("top,t", po::value<unsigned int>(), "dump top 'arg' of used packages")
    ("bottom,b", po::value<unsigned int>(), "dump bottom 'arg' of used packages")
    ("all,a", "dump all packages");

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

  if(vm.count("top")) {
    sett.top = vm["top"].as<unsigned int>();
  }

  if(vm.count("bottom")) {
    sett.bottom = vm["bottom"].as<unsigned int>();
  }

  return true;
}

int main(int argc, char* argv[]) {
  settings sett;

  if(!parse_command_line(argc, argv, sett))
  {
    exit(EXIT_SUCCESS);
  }

  cmdclient cmdclient(sett);
  return cmdclient.dump_top();
}
