#include <systemc.h>
using namespace sc_core;

SC_MODULE(Room)
{
  sc_in<bool> light;

  int people_count;

  void switch_lights()
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


}

SC_MODULE(sensor)
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
