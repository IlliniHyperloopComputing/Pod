#ifndef SOURCE_MANAGER_BASE_HPP
#define SOURCE_MANAGER_BASE_HPP

#include "Utils.h"
#include <memory>
#include <thread>
#include <mutex>
#include <atomic>

template <long long DelayInUsecs, class Data>
class SourceManagerBase {

  public:
    std::shared_ptr<Data> Get() {
      mutex.lock();
      std::shared_ptr<Data> ret = data;
      mutex.unlock();
      return ret;
    }

    void initialize() {
      data = refresh();
      running.store(true);

      // I don't know how to start a thread using a member function, but I know how to use lambdas so suck it C++
      worker = std::thread( [&] { refresh_loop(); } );
    }
    
    void stop() {
      running.store(false);
      worker.join();
    }

  private:
    virtual std::shared_ptr<Data> refresh() = 0; //constructs a new Data object and fills it in

    void refresh_loop() {
      while(running.load()) {
        std::shared_ptr<Data> new_data = refresh();
        mutex.lock();
        data = new_data;
        mutex.unlock();
        std::this_thread::sleep_for(std::chrono::microseconds(DelayInUsecs));
      }
    }

    std::shared_ptr<Data> data;
    std::mutex mutex;
    std::atomic<bool> running;
    std::thread worker;
};
#endif
