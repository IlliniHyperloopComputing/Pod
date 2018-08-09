#ifndef FILTER_H
#define FILTER_H

#define LOWPASS_PERCENT 0.9

namespace Filter{
  double LowPass(double t_old, double t_new);
}

#endif
