#ifndef DPKG_H
#define DPKG_H

#include <unordered_map>
#include <string>
#include <list>
#include <mutex>
#include "dllist.h"

class dpkg {
  public:
    dpkg();
    void openfile(std::string path);
    void dumpTop();

  private:
    void _loadFileLists();

  private:
    typedef std::pair<std::string, long long> pkgUsage;
    dllist<pkgUsage> packages_list;
    typedef dlnodelist<pkgUsage>* pkg_list_node;
    std::unordered_map<std::string, pkg_list_node>  fileToPackage;
    std::list<pkg_list_node> top_list;
    std::mutex mutex;

};

#endif
