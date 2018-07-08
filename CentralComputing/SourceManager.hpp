#ifndef SOURCE_MANAGER_HPP
#define SOURCE_MANAGER_HPP

#include "NetworkManager.hpp"
#include "Utils.h"


class SourceManagerBase {

  public:

    SourceManagerBase(double delay_in_seconds):
      delay_in_usecs(delay_in_seconds*1E6)
    {}

    void work(){
      usleep(delay_in_usecs);
      refresh();
    }

  private:

    void refresh(){};

    double delay_in_usecs;

};
#endif
