#include "TMPManager.hpp"

bool TMPManager::initialize_source(){
  // Try to open a file
  std::ifstream in(prefix + devices[idx] + suffix);

  if(in){
    print(LogLevel::LOG_DEBUG, "TMP Manger setup successful\n");
    return true;
  }
  else{
    print(LogLevel::LOG_ERROR, "TMP Manger setup failed\n");
    return false;
  }

}

void TMPManager::stop_source(){
  print(LogLevel::LOG_DEBUG, "TMP Manger stopped\n");
}

std::shared_ptr<TMPData> TMPManager::refresh() {
  //create the data struct that we will return
  std::shared_ptr<TMPData> new_data = std::make_shared<TMPData>();

  //open the file
  std::ifstream in(prefix + devices[idx] + suffix);

  //If valid input
  if (in){
    //File will have two lines. we want the second line
    std::string line;
    std::getline(in, line);//ignore this line
    std::getline(in, line);//we want this line

    //parse the temperature value
    int rawTmp = std::stoi(line.substr(line.find('t')+2));

    //Set the new value
    old_data.tmp_data[idx] = rawTmp/1000.0;
  }
  else{
    print(LogLevel::LOG_ERROR, "TMP Manager: File[%d], device: %s not opened correctly\n", idx, devices[idx].c_str());
  }
  
  //Copy the values into the new_data struct
  memcpy(new_data.get(), &old_data, sizeof(TMPData));

  //increment the index
  idx = (idx+1) % NUM_TMP;
  
  return new_data;
}

std::shared_ptr<TMPData> TMPManager::refresh_sim() {
  return empty_data();
}
