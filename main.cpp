/*
 * Copyright (c) 2022, CATIE
 * SPDX-License-Identifier: Apache-2.0
 */
#include "mbed.h"

// Déclaration des périphériques
DigitalOut led(LED1);
InterruptIn button(BUTTON1);
Timer button_timer;
EventQueue queue(32 * EVENTS_EVENT_SIZE); // Queue pour déplacer les actions hors ISR
Thread thread;                           // Thread pour exécuter la queue d'événements

// Variables globales pour stocker l'état du bouton
bool is_button_pressed = false;

// Fonction appelée dans le thread principal pour gérer l'appui
void handle_button_press()
{
    printf("Bouton appuyé\n");
    led = 1;               // Allumer la LED
    button_timer.start();  // Démarrer le timer
}

// Fonction appelée dans le thread principal pour gérer le relâchement
void handle_button_release()
{
    button_timer.stop();                 // Arrêter le timer
    int duration = button_timer.read_ms(); // Lire la durée d'appui
    printf("Bouton relâché, durée : %d ms\n", duration);
    led = 0;                             // Éteindre la LED
    button_timer.reset();                // Réinitialiser le timer
}

// Fonction appelée dans le contexte ISR pour l'appui
void on_button_press()
{
    queue.call(handle_button_press); // Planifie l'exécution hors de l'ISR
}

// Fonction appelée dans le contexte ISR pour le relâchement
void on_button_release()
{
    queue.call(handle_button_release); // Planifie l'exécution hors de l'ISR
}

int main()
{
    // Configurer les interruptions
    button.rise(&on_button_press);   // Appelé lors de l'appui
    button.fall(&on_button_release); // Appelé lors du relâchement

    // Démarrer le thread pour exécuter la queue d'événements
    thread.start(callback(&queue, &EventQueue::dispatch_forever));

    while (true) {
        // Le thread principal reste disponible pour d'autres tâches
        ThisThread::sleep_for(100ms);
    }
}
