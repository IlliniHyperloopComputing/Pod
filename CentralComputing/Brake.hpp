#ifndef BRAKE_HPP
#define BRAKE_HPP

class Brake {
  public:

    Brake();

    void enable_brakes();

    void disable_brakes();

    void is_enabled();

    void calculate_brakeForce();

    void set_brakeForce();

    void debug_set_pwm_pin();

  private:

    void set_enable();
};

#endif
