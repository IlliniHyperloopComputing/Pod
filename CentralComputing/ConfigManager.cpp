#include "ConfigManager.hpp"
using namespace Utils;


//derived from stack overflow and various websites
void ConfigManager::load(std::string fileName)
{
  std::ifstream input(fileName.c_str());
  if(input.is_open()){
      //until you reach the end of the file
      while(!input.eof()){
            std::string number;
            double data;
            getline(input,number); //read number

            //could possibly store the keys somewhere
            std::string delimiter = ": ";
            size_t pos = 0;
            pos = number.find(delimiter);
            std::string token = number.substr(0, pos);
            number.erase(0, pos + delimiter.length());
            data = std::atof(number.c_str()); //convert to double
            constants[token] = data;
      }
      print(LogLevel::LOG_INFO, "File successfully read \n");
  }
  else{
    print(LogLevel::LOG_ERROR, "Config Manager unable to find file: %s\n", fileName.c_str());
  }
}
