#ifndef SOURCE_MANAGER_BASE_HPP
#define SOURCE_MANAGER_BASE_HPP

#include "Utils.h"
#include "Event.hpp"
#include <memory>
#include <thread>
#include <mutex>
#include <atomic>

using namespace Utils;

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

      // Initialize the source manager
      initialized_correctly = initialize_source();
      if(initialized_correctly){
        
        // If the source manager initialized correcly, setup the worker
        data = refresh();
        running.store(true);

        // I don't know how to start a thread using a member function, but I know how to use lambdas so suck it C++
        worker = std::thread( [&] { refresh_loop(); } );
      }
      else{

        // Did not setup correctly. Print error and set garbage data
        print(LogLevel::LOG_ERROR, "Failed to initialize. Not running worker thread\n");

        // Set garbage data
        data = std::make_shared<Data>();
        memset(data.get(), (uint8_t)0, sizeof(Data));

      }
    }

    
    void stop() {
      if(initialized_correctly){
        running.store(false);
        closing.invoke();
        worker.join();
        stop_source();
      }
    }

  private:
    // Init and Stop functions can setup devices/ file I/O
    // Stop will only be called if init returns true
    virtual bool initialize_source() = 0;
    virtual void stop_source() = 0;
    
    virtual std::shared_ptr<Data> refresh() = 0; //constructs a new Data object and fills it in

    void refresh_loop() {
      while(running.load()) {
        std::shared_ptr<Data> new_data = refresh();
        mutex.lock();
        data = new_data;
        mutex.unlock();
        print(LogLevel::LOG_DEBUG, "running through wait: %lu\n", DelayInUsecs);
        closing.wait_for(DelayInUsecs);
      }
    }

    std::shared_ptr<Data> data;
    std::mutex mutex;
    std::atomic<bool> running;
    Event closing;
    std::thread worker;
    bool initialized_correctly;
};
#endif
