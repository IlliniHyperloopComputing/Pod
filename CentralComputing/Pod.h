#ifndef POD_H
#define POD_H

#include "NetworkManager.hpp"
#include "ParameterManager.hpp"
#include "Pod_State.h"
#pragma GCC diagnostic ignored "-Wctor-dtor-privacy"
#include "gtest/gtest.h"


class Pod {
  public:
    void startup();
  private:
    void logic_loop();

    shared_ptr<Pod_State> state_machine;
    bool running = true;
};


#endif
