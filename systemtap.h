#ifndef SYSTEMTAP_H
#define SYSTEMTAP_H

#include <string>
#include <functional>

class systemtap {
  public:
    systemtap();
    void start(std::function<void(std::string)> processLine);
    void stop();
};

#endif
