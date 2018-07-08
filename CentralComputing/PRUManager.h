#ifndef PRU_MANAGER_HPP
#define PRU_MANAGER_HPP

#include "SourceManagerBase.hpp"

struct PRUData {
  //replace with actual data structure
  int dummy_data;
};

class PRUManager : public SourceManagerBase<1, PRUData> {
  private:
    shared_ptr<PRUData> refresh() {
      //this is where you would query the PRU and get new data
      
      shared_ptr<PRUData> new_data = make_shared<PRUData>();
      new_data->dummy_data = i;
      i++;
      return new_data;
    }

    //iterator for testing purposes, remove
    int i = 5;
};

#endif

