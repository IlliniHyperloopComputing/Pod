#ifndef POD_H
#define POD_H

#include "NetworkManager.hpp"
#include "Event.hpp"
#include "Pod_State.h"
#include "Configurator.hpp"
#include <string>
#include <functional>
#include <semaphore.h>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wctor-dtor-privacy"
#include "gtest/gtest.h"
#pragma GCC diagnostic pop


class Pod {
  public:
    Pod();

    void startup();

    void stop();

    std::shared_ptr<Pod_State> state_machine;
    std::atomic<bool> running;
    Event ready;
    Event processing_command;
    Event closing;

  private:
    void logic_loop();
    bool switchVal;
};

void signal_handler(int signal);


#endif
