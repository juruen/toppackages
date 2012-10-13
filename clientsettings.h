#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>
#include <boost/optional/optional.hpp>

struct settings
{
  boost::optional<size_t> top;
  boost::optional<size_t> bottom;

  settings() {}
};

#endif
