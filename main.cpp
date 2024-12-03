#include "mbed.h"
#include "bme280.h"

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
volatile bool running = true; // Indicateur pour arrêter le thread


void read_sensor()
{
    if (sensor.initialize() == 0) {
        printf("Erreur : Initialisation du capteur BME280 échouee\n");
        running = false;
        return;
    }

    
    sensor.set_sampling(
        BME280::SensorMode::NORMAL,
        BME280::SensorSampling::OVERSAMPLING_X16, 
        BME280::SensorSampling::OVERSAMPLING_X16, 
        BME280::SensorSampling::OVERSAMPLING_X16,
        BME280::SensorFilter::OFF,              
        BME280::StandbyDuration::MS_0_5      
    );

    printf("Capteur BME280 configure avec succes \n");

    while (running) {
        temperature = sensor.temperature();
        humidity = sensor.humidity();
        pressure = sensor.pressure();

        ThisThread::sleep_for(1s); 
    }
}

int main()
{
   
    Thread thread;
    thread.start(read_sensor);

  
    while (running) {
        printf("Temperature : %.2f deg Celsius \n", temperature);
        printf("Humidite : %.2f %% \n", humidity);
        printf("Pression : %.2f hPa \n", pressure / 100.0);

        ThisThread::sleep_for(1s); 
    }

    // Attendre que le thread se termine proprement / si dans while= true pas besoin 
    thread.join();

}
