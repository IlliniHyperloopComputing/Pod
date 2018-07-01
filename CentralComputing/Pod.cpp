#include "Pod.h"

using namespace std;
int main() {
  NetworkManager::start_server("127.0.0.1", "8800");
  thread network_thread(NetworkManager::network_loop);
  network_thread.join();

}
