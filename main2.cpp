#include <systemc.h>
#include <ctime>
#include <cstdlib>

using namespace sc_core;
using std::cout;

// Define the Room module with light and alarm
SC_MODULE(Room) {
  sc_in<bool> enter_signal; 
  sc_in<bool> exit_signal;
  sc_out<bool> light;       
  sc_out<bool> alarm;      

  int people_count;

  void check_alarm() {
    if (people_count < 0) {
      alarm.write(true); // Trigger alarm for incorrect count
    } else {
      alarm.write(false);
    }
  }

  void change_ppl_count() {
    while (true) {
      if (enter_signal.read()) {
        people_count++;
      } else if (exit_signal.read()) {
        people_count--;
      }

      // Control light state
      if (people_count > 0) {
        light.write(true); // Light ON if there are people
      } else {
        light.write(false); // Light OFF if no people
      }

      // Check alarm state
      check_alarm();
      
      wait(1, SC_SEC); // Simulation delay
    }
  }

  SC_CTOR(Room) : people_count(0) {
    SC_THREAD(change_ppl_count);
  }
};


SC_MODULE(cd_sensor) {
  sc_out<bool> enter_signal;
  sc_out<bool> exit_signal;
  int num_people; // Number of people to simulate

  void check_motion() {
    srand(time(0)); // Seed for random generation

    while (true) {
      for (int i = 0; i < num_people; ++i) {
        int action = rand() % 2; // Randomly decide to enter (0) or exit (1)
        int random_delay = rand() % 5 + 1; // Random delay between 1 and 5 seconds

        if (action == 0) { // Simulate entry
          enter_signal.write(true);
          wait(1, SC_SEC); // Hold signal for 1 second
          enter_signal.write(false);
        } else { // Simulate exit
          exit_signal.write(true);
          wait(1, SC_SEC); // Hold signal for 1 second
          exit_signal.write(false);
        }

        wait(random_delay, SC_SEC); // Random delay between actions
      }
    }
  }

  SC_CTOR(cd_sensor) : num_people(3) { // Default: 3 people
    SC_THREAD(check_motion);
  }
};


// Define the light system that reacts to room state
SC_MODULE(s_light) {
  sc_out<bool> green_led;
  sc_out<bool> red_led;
  int people_count;

  void check_switch_lights() {
    if (people_count > 0) {
      green_led.write(true); // Green light if people are inside
      red_led.write(false);
    } else {
      green_led.write(false); // Red light if no one is inside
      red_led.write(true); // Red LED on if alarm triggered
    }
  }

  SC_CTOR(s_light) : people_count(0) {
    SC_THREAD(check_switch_lights);
  }
};

// Function to display room status
void display_menu(Room& room, int room_id) {
  cout << "Room " << room_id << " Status:\n";
  cout << "People Count: " << room.people_count << "\n";
  cout << "Light: " << (room.light.read() ? "ON" : "OFF") << "\n";
  cout << "Alarm: " << (room.alarm.read() ? "ACTIVE" : "INACTIVE") << "\n";
}

int sc_main(int, char*[]) {
  sc_signal<bool> person_entered_sig1, person_exited_sig1;
  sc_signal<bool> person_entered_sig2, person_exited_sig2;
  sc_signal<bool> person_entered_sig3, person_exited_sig3;
  sc_signal<bool> light_sig, alarm_sig;

  // Create Room 1
  Room room1("Room1");
  room1.enter_signal(person_entered_sig1);
  room1.exit_signal(person_exited_sig1);
  room1.light(light_sig);
  room1.alarm(alarm_sig);

  // Create Room 2
  Room room2("Room2");
  room2.enter_signal(person_entered_sig2);
  room2.exit_signal(person_exited_sig2);
  sc_signal<bool> light_sig2, alarm_sig2;
  room2.light(light_sig2);
  room2.alarm(alarm_sig2);

  // Create Sensors for Room 1 and Room 2
  cd_sensor sensor_cd1("cd1");
  sensor_cd1.enter_signal(person_entered_sig1);
  sensor_cd1.exit_signal(person_exited_sig1);

  cd_sensor sensor_cd4("cd4");
  sensor_cd4.enter_signal(person_entered_sig2);
  sensor_cd4.exit_signal(person_exited_sig2);

  cd_sensor sensor_cd5("cd5");
  sensor_cd5.enter_signal(person_entered_sig3);
  sensor_cd5.exit_signal(person_exited_sig3);

  // Create Light System for Room 1
  sc_signal<bool> green_led_sig1, red_led_sig1;
  s_light light1("light1");
  light1.green_led(green_led_sig1);
  light1.red_led(red_led_sig1);

  // Create Light System for Room 2
  sc_signal<bool> green_led_sig2, red_led_sig2;
  s_light light2("light2");
  light2.green_led(green_led_sig2);
  light2.red_led(red_led_sig2);

  // Run the simulation for 20 seconds
  sc_start(20, SC_SEC); 

  // Display room status
  display_menu(room1, 1);
  display_menu(room2, 2);

  return 0;
}

