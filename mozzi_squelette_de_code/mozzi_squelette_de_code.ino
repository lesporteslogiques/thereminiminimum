
/* Structure pour l 'utilisation de la lib. Mozzi 
 *  arduino 1.8.5 + lib. Mozzi 2015-05-11
 */

#include <MozziGuts.h>        // à inclure en haut du sketch
#define CONTROL_RATE 64       // taux de rafraichissement des controleurs (toujours une puissance de 2 : 64, 128, 256, etc.)

void setup() {
  startMozzi(CONTROL_RATE);
}

void updateControl() {
  // code pour les controleurs
}

int updateAudio() {
  // code pour l'audio, renvoit un échantillon entre -244 et 243 ( ~= 9 bits)
  // en pratique, un type char fonctionne 
  
  return 0;
}

void loop() {
  audioHook();  // garnir le buffer audio
}

