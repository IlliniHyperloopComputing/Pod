#ifndef PRU_MANAGER_HPP
#define PRU_MANAGER_HPP

#include "SourceManager.hpp"

class PRUManager : public SourceManagerBase {

  public:

    PRUManager(double delay_in_seconds):
      SourceManagerBase(delay_in_seconds)
    {}

    int getPRUData(){
      return 7;
    }

  private:

    void refresh(){
      //Do work here to update data
      //this is running in its own thread
      
    }


};

#endif

