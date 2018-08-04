#ifndef PRU_MANAGER_HPP
#define PRU_MANAGER_HPP

#include "SourceManagerBase.hpp"

struct PRUData {
  //replace with actual data structure
  int dummy_data;
};

class PRUManager : public SourceManagerBase<(long long) (1.0 * 1E6), PRUData> {
  private:
    std::shared_ptr<PRUData> refresh();
      

    //iterator for testing purposes, remove
    int i = 5;
};

#endif

