#include <systemc.h>
using namespace sc_core;

SC_MODULE(Room)
{
  sc_in<bool> enter_signal; 
  sc_in<bool> exit_signal;
  sc_out<bool> light;       
  sc_out<bool> alarm;      

  int people_count;


  //void check_alarm()
  //{
    //if(people_count < 0 || people_count > 20)
    //{
      //red_led.write(true);
    //}
  //}

  void change_ppl_count() {

    while (true)
    {
      std::cout << "checking if person entered.." << "\n";
      if (person_entered.read()) {
        people_count++;
        std::cout << "new person in room!"<<"\n";

      } else if (person_exited.read()) {
        people_count--;
        std::cout << "person left the room"<<"\n";
      }
      //check_switch_lights();
      //check_alarm();
      wait(1, SC_SEC);
    }

  }

  SC_CTOR(Room)
  {
    SC_THREAD(change_ppl_count);

  }
};

SC_MODULE(cd_sensor)
{
  sc_out<bool> enter_signal;
  sc_out<bool> exit_signal;
  bool enter_state;
  void check_motion() {

    while(true)
    {
      if (enter_state) {
        enter_signal.write(true);
        wait(1, SC_SEC);
        enter_signal.write(false);
      } else {
        //wait(5, SC_SEC);
        exit_signal.write(true);
        wait(1, SC_SEC);
        exit_signal.write(false);
      }

    }
  }
  
  SC_CTOR(cd_sensor)
  {
    enter_state = true;
    SC_THREAD(check_motion)
  }
};

SC_MODULE(sw_switch)
{

};

SC_MODULE(s_light)
{
  sc_out<bool> green_led;
  sc_out<bool> red_led;
  int people_count;

  void check_switch_lights()
  {
    if(people_count > 0)
    {
        green_led.write(true);
    }
    else
    {
        green_led.write(false);
    }

  }


};


int sc_main(int, char*[]) {
  
  Room room1("r1");
  sc_signal<bool> person_entered_sig;
  sc_signal<bool> person_exited_sig;
  //sc_signal<bool> light_sig;
  sc_signal<bool> alarm_sig;

  room1.person_entered(person_entered_sig);
  room1.person_exited(person_exited_sig);
  //room1.red_led(alarm_sig);

  cd_sensor sensor("cd1");
  sensor.enter_signal(person_entered_sig);
  sensor.exit_signal(person_exited_sig);

  sc_start(10, SC_SEC);
  //modul procesora 1,2
  ////5 pokoi 5 procesow, 6 proces na sterowanie
  //w procesie 1 n-1 procesów, n-ty proces na drugim module
  //w module procesora 1 ma być podmoduł i jjedna z funkcji wewnatrz podmodulu
  //kanal komunikacyjny budujacy moduly, czyli klasa interpretujaca zapis i odczyt i ta klasa posiad bufor w postaci kolejki fifo
  //interfejsy posiadaja metody read i write do fifo
  // 1 proces w 1 module odczytuje wartosc i zapisuje do kolejki fifo   //dzialanie w symulacji nieskonczonej 
  // przejrzyste menu, wypisanie pokoi, czy dioda jest zapalona i liczba osob w kazdym pokoju
  // osoba wchodzi do pokoju wchodzi do pokoju i swiatlo sie zapala od razu
  // slider symuluje wejscie czlowieka
  // w kazdym pokoju licznik osob 

  return 0;
}
