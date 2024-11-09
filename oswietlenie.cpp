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
