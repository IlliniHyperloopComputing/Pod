#include "I2CManager.hpp"

bool I2CManager::initialize_source(){
  return true;
}

void I2CManager::stop_source(){

}

std::shared_ptr<I2CData> I2CManager::refresh() {
  std::shared_ptr<I2CData> new_data = std::make_shared<I2CData>();
  new_data->dummy_data = i;
  i++;
  return new_data;
}
