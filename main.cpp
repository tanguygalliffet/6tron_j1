#include "mbed.h"

Thread thread_sensor;       
Thread thread_led;          
EventQueue event_queue;    
DigitalOut led(LED1);     
InterruptIn button(BUTTON1);  

// SIMUS
float get_temperature() { return 25.0f + rand() % 10; } 
float get_humidity() { return 50.0f + rand() % 20; } 
float get_pressure() { return 1013.25f + rand() % 10; }


Mutex print_mutex;


void read_sensor_data()
{
    while (true) {
        float temperature = get_temperature();
        float humidity = get_humidity();

        print_mutex.lock();
        printf("Temperature : %.2f degres celsius\n", temperature);
        printf("Humidite : %.2f %%\n", humidity);
        print_mutex.unlock();

        ThisThread::sleep_for(2s); 
    }
}


void display_pressure()
{
    float pressure = get_pressure();

    print_mutex.lock();
    printf("Pression atmospherique : %.2f hPa\n", pressure);
    print_mutex.unlock();
}


void blink_led()
{
    while (true) {
        led = !led; 
        ThisThread::sleep_for(5s);
    }
}


void schedule_pressure()
{
    event_queue.call(display_pressure);
}

int main()
{
    
    button.fall(schedule_pressure);

    
    thread_sensor.start(read_sensor_data);
    thread_led.start(blink_led);

   
    event_queue.dispatch_forever();
}
