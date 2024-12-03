#include "mbed.h"


Thread thread_ping;
Thread thread_pong;
DigitalOut led(LED1);
Mutex print_mutex; // Mutex pour protéger l'accès aux sorties console


void ping()
{
    for (int i = 0; i < 100; i++) {
        print_mutex.lock(); 
        printf("Ping\n");
        print_mutex.unlock(); 
        ThisThread::sleep_for(100ms);
    }
}


void pong()
{
    for (int i = 0; i < 100; i++) {
        print_mutex.lock(); 
        printf("Pong\n");
        print_mutex.unlock(); 
        ThisThread::sleep_for(100ms);
    }
}

int main()
{
    
    thread_ping.start(ping);
    thread_pong.start(pong);

    
    while (true) {
        led = !led; 
        print_mutex.lock(); 
        printf("Alive!\n");
        print_mutex.unlock(); 
        ThisThread::sleep_for(500ms);
    }
}
