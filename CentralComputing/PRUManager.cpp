#include "PRUManager.hpp"

std::shared_ptr<PRUData> PRUManager::refresh() {
  std::shared_ptr<PRUData> new_data = std::make_shared<PRUData>();
  new_data->dummy_data = i;
  i++;
  return new_data;
}
