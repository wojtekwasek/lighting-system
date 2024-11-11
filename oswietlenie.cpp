#include <systemc.h>
using namespace sc_core;

SC_MODULE(Room)
{
  sc_in<bool> enter_signal; 
  sc_in<bool> exit_signal;
  sc_out<bool> light;       
  sc_out<bool> alarm;      

  int people_count;

  void check_switch_lights()
  {
    if(people_count > 0)
    {
        light.write(true);
    }
    else
    {
      light.write(false);
    }

  }

  void check_alarm()
  {
    if(people_count < 0 || people_count > 20)
    {
      alarm.write(true);
    }
  }

  void check_ppl_count() {

    if (enter_signal.read()) {
      occupancy++;
    } else if (exit_signal.read()) {
      occupancy--;
    }
    check_switch_lights();
    check_alarm();
  }

}


SC_MODULE(cd_sensor)
{
  sc_out<bool> enter_signal;
  sc_out<bool> exit_signal;

  void generate_pulse(bool enter) {
    if (enter) {
      enter_signal.write(true);
      wait(1, SC_NS);
      enter_signal.write(false);
    } else {
      exit_signal.write(true);
      wait(1, SC_NS);
      exit_signal.write(false);
    }
  }

}

SC_MODULE(switch)
{

}

SC_MODULE(lights)
{

}


int sc_main(int, char*[]) {

  return 0;
}
