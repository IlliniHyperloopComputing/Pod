#include "CANManager.hpp"

std::shared_ptr<CANData> CANManager::refresh() {
  std::shared_ptr<CANData> new_data = std::make_shared<CANData>();
  new_data->dummy_data = i;
  i++;
  return new_data;
}
