#include "Null.h"
Arbitrary_Data no_calculation(Arbitrary_Data raw){
  Arbitrary_Data calculated;
  calculated.size = 0;
  calculated.data = NULL;
  return calculated;
}
