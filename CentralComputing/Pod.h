#ifndef POD_H
#define POD_H

#include "NetworkManager.hpp"
#include "ParameterManager.hpp"
#include "Pod_State.h"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wctor-dtor-privacy"
#include "gtest/gtest.h"
#pragma GCC diagnostic pop


class Pod {
  public:
    Pod();

    void startup();

    shared_ptr<Pod_State> state_machine;
    atomic<bool> running;
  private:
    void logic_loop();
};


#endif
