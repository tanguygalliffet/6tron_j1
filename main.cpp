#include "mbed.h"
#include "bme280/bme280.h" 

Thread thread_sensor;       
Thread thread_led;          
EventQueue event_queue;    
DigitalOut led(LED1);     
InterruptIn button(BUTTON1);  
Mutex print_mutex;

using namespace sixtron;


namespace
{
    constexpr auto SDA_PIN = I2C1_SDA;
    constexpr auto SCL_PIN = I2C1_SCL;
    constexpr auto I2C_ADDRESS = BME280::I2CAddress::Address1;
}


I2C i2c(SDA_PIN, SCL_PIN);
BME280 sensor(&i2c, I2C_ADDRESS);


volatile float temperature = 0.0f;
volatile float humidity = 0.0f;
volatile float pressure = 0.0f;


void read_sensor_data()
{
    while (true) {
        
        temperature = sensor.temperature();
        humidity = sensor.humidity();

      
        print_mutex.lock();
        printf("Température : %.2f °C\n", temperature);
        printf("Humidité : %.2f %%\n", humidity);
        print_mutex.unlock();

        ThisThread::sleep_for(2s); 
    }
}


void display_pressure()
{
    pressure = sensor.pressure();

    print_mutex.lock();
    printf("Pression atmosphérique : %.2f hPa\n", pressure / 100.0); // Conversion en hPa
    print_mutex.unlock();
}


void blink_led()
{
    while (true) {
        led = !led; 
        ThisThread::sleep_for(5s);
    }
}

// Fonction appelée par le bouton pour planifier l'affichage de la pression
void schedule_pressure()
{
    event_queue.call(display_pressure);
}

int main()
{
   
    if (sensor.initialize() != true) {
        printf("Erreur d'initialisation du capteur BME280\n");
        return 1;
    }
    printf("Capteur BME280 initialisé avec succès\n");

    // config
    sensor.set_sampling(
        BME280::SensorMode::NORMAL,
        BME280::SensorSampling::OVERSAMPLING_X16, // température
        BME280::SensorSampling::OVERSAMPLING_X16, // pression
        BME280::SensorSampling::OVERSAMPLING_X16, // humidité
        BME280::SensorFilter::OFF,                // Pas de filtrage
        BME280::StandbyDuration::MS_0_5           // Durée de veille
    );

    // Configuration du bouton pour planifier l'affichage de la pression
    button.fall(schedule_pressure);

    // Démarrage des threads
    thread_sensor.start(read_sensor_data);
    thread_led.start(blink_led);

    // Démarrage de la file d'événements
    event_queue.dispatch_forever();
}
