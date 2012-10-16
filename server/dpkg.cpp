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
  const boost::posix_time::seconds freq(1);
  const boost::posix_time::seconds zero_freq(0);
};

dpkg::dpkg(boost::asio::io_service& io_service, settings& sett) 
  : m_timer(io_service, zero_freq),
  file_to_package(),
  top_list(),
  sett(sett)
{
  load_filelists();
  add_handle();
}

void dpkg::open_file(string path)
{
  auto it = file_to_package.find(path);
  if (it == file_to_package.end()) return;

  dlnodelist<pkg_usage>* node = it->second;
  node->value.second += 1;
  packages_list.to_front(node);
}

void dpkg::current_top(const commonserver::toptype type, const size_t max, vector<string>& output)
{
  auto node = packages_list.front();;
  size_t max_packages = max;
  if (type != commonserver::toptype::top) {
    node = packages_list.back();
  }
  if (type == commonserver::toptype::all) {
    max_packages = packages_list.length();
  }
  for (auto loops = max_packages; loops > 0 && node; loops--) {
    output.push_back(node->value.first);
    if (type == commonserver::toptype::top) {
      node = node->next;
    } else {
      node = node->prev;
    }
  }
}

void dpkg::dump_top()
{
  vector<string> packages;
  current_top(commonserver::toptype::top, sett.top_packages, packages); 
  std::ofstream ofile;
  ofile.open(sett.output_file);
  for (auto s: packages) {
    ofile << s << endl;
  }
  ofile.close();
}


void dpkg::load_filelists()
{
  for (
    directory_iterator it = directory_iterator(dpkg_path);
    it != directory_iterator();
    it++) {

    if (!ends_with(it->path().string(), list_ext)) continue;

    string pkgname = it->path().stem().string();

    ifstream infile(it->path().string());
    packages_list.push_front(pkg_usage(pkgname, 0));
    auto list_node = packages_list.front();
    for (string line; getline(infile, line);) {
      file_to_package[line] = list_node;
    }
  }
}

void dpkg::timer_handle( const boost::system::error_code& ec)
{
  dump_top();
  add_handle();
}

void dpkg::add_handle()
{
  return;
  m_timer.expires_at(m_timer.expires_at() + freq);
  m_timer.async_wait(
      boost::bind(
        &dpkg::timer_handle,
        this,
        boost::asio::placeholders::error
      )
  );
}



