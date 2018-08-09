#include "Filter.h"

double Filter::LowPass(double t_old, double t_new){
  return t_old * LOWPASS_PERCENT + t_new * (1.0 - LOWPASS_PERCENT);
}
