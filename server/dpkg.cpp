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
  m_sett(sett),
  m_packages_list(),
  m_file_to_package()
{
  load_filelists();
  add_handle();
}

void dpkg::open_file(string path)
{
  auto it = m_file_to_package.find(path);
  if (it == m_file_to_package.end()) return;

  packages_list_iter node = it->second;
  node->second += 1;
  m_packages_list.splice(m_packages_list.begin(), m_packages_list, node);
  it->second = m_packages_list.begin();
}

void dpkg::top_n(size_t n, vector<string>& output)
{
  for(auto node = m_packages_list.begin();
      n > 0 && node != m_packages_list.end();
      ++node, --n)
  {
    output.push_back(node->first);
  }
}

void dpkg::bottom_n(size_t n, vector<string>& output)
{
  for(auto node = m_packages_list.rbegin();
      n > 0 && node != m_packages_list.rend();
      ++node, --n)
  {
    output.push_back(node->first);
  }
}

void dpkg::current_top(const commonserver::toptype type, const size_t max, vector<string>& output)
{
  size_t max_packages = max;
  if (type == commonserver::toptype::all) {
    max_packages = m_packages_list.size();
  }
  if (type == commonserver::toptype::top) {
    top_n(max_packages, output);
  } else {
    bottom_n(max_packages, output);
  }
}

void dpkg::dump_top()
{
  vector<string> packages;
  current_top(commonserver::toptype::top, m_sett.top_packages, packages); 
  std::ofstream ofile;
  ofile.open(m_sett.output_file);
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
    m_packages_list.push_front(pkg_usage(pkgname, 0));
    packages_list_iter list_node = m_packages_list.begin();
    for (string line; getline(infile, line);) {
      m_file_to_package[line] = list_node;
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
