#include "dpkg.h"

#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/bind.hpp>

using namespace boost::filesystem;
using namespace boost::algorithm;
using namespace std;

namespace {
  const path dpkg_path("/var/lib/dpkg/info");
  const string list_ext(".list");
  const unsigned int topPkgs = 50;
  const string outputfile ("topdeb.out");
  const boost::posix_time::seconds freq(1);
  const boost::posix_time::seconds zero_freq(0);
};

dpkg::dpkg(boost::asio::io_service& io_service) : m_timer(io_service, zero_freq)
{
  _loadFileLists();
  add_handle();
}

void dpkg::openfile(string path)
{
  auto it = fileToPackage.find(path);
  if (it == fileToPackage.end()) return;

  dlnodelist<pkgUsage>* node = (*it).second;
  node->value.second += 1;
  packages_list.to_front(node);
}

void dpkg::dumpTop()
{
  auto loops = topPkgs;
  auto node = packages_list.front();
  std::ofstream ofile;
  ofile.open(outputfile);
  while (loops > 0 && node->next) {
    ofile << node->value.first << endl;
    node = node->next;
    loops--;
  }
  ofile.close();
}


void dpkg::_loadFileLists()
{
  for (
    directory_iterator it = directory_iterator(dpkg_path);
    it != directory_iterator();
    it++) {

    if (!ends_with((*it).path().string(), list_ext)) continue;

    string pkgname = (*it).path().filename().string();
    pkgname = pkgname.erase(pkgname.size() - list_ext.size(), list_ext.size());

    ifstream infile((*it).path().string());
    packages_list.push_front(pkgUsage(pkgname, 0));
    auto list_node = packages_list.front();
    for (string line; getline(infile, line);) {
      fileToPackage[line] = list_node;
    }
  }
}

void dpkg::timer_handle( const boost::system::error_code& ec)
{
  dumpTop();
  add_handle();
}

void dpkg::add_handle()
{
  m_timer.expires_at(m_timer.expires_at() + freq);
  m_timer.async_wait(
      boost::bind(
        &dpkg::timer_handle,
        this,
        boost::asio::placeholders::error
      )
  );
}



