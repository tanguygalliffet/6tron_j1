#include "mbed.h"
#include "bme280.h"

using namespace sixtron;

// Définition des constantes
namespace
{
    constexpr auto SDA_PIN = I2C1_SDA;
    constexpr auto SCL_PIN = I2C1_SCL;
    constexpr auto I2C_ADDRESS = BME280::I2CAddress::Address1;
}

// Initialisation des objets nécessaires
I2C i2c(SDA_PIN, SCL_PIN);
BME280 sensor(&i2c, I2C_ADDRESS);

// Variables partagées pour stocker les données
volatile float temperature = 0.0f;
volatile float humidity = 0.0f;
volatile float pressure = 0.0f;
volatile bool running = true; // Indicateur pour arrêter le thread

// Fonction pour configurer et lire les données du capteur
void read_sensor()
{
    if (sensor.initialize() == 0) {
        printf("Erreur : Initialisation du capteur BME280 échouee\n");
        running = false;
        return;
    }

    // Configuration du capteur
    sensor.set_sampling(
        BME280::SensorMode::NORMAL,
        BME280::SensorSampling::OVERSAMPLING_X16, // Échantillonnage pour la température
        BME280::SensorSampling::OVERSAMPLING_X16, // Échantillonnage pour la pression
        BME280::SensorSampling::OVERSAMPLING_X16, // Échantillonnage pour l'humidité
        BME280::SensorFilter::OFF,                // Pas de filtrage
        BME280::StandbyDuration::MS_0_5           // Durée de veille
    );

    printf("Capteur BME280 configure avec succes \n");

    while (running) {
        temperature = sensor.temperature();
        humidity = sensor.humidity();
        pressure = sensor.pressure();

        ThisThread::sleep_for(1s); // Lecture toutes les secondes
    }
}

int main()
{
    // Création et démarrage du thread
    Thread thread;
    thread.start(read_sensor);

    // Boucle principale pour afficher les données
    while (running) {
        printf("Temperature : %.2f deg Celsius \n", temperature);
        printf("Humidite : %.2f %% \n", humidity);
        printf("Pression : %.2f hPa \n", pressure / 100.0);

        ThisThread::sleep_for(1s); // Affichage toutes les secondes
    }

    // Attendre que le thread se termine proprement
    thread.join();

    printf("Fin de l'application.\n");
}
