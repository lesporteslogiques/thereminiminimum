/*  D'après Control_Echo_Theremin / Tim Barrass 2013, CC by-nc-sa.
 *   arduino 1.8.5 + lib. Mozzi 2015-05-11
 *   
 *   On peut changer le type de la forme d'onde des oscillateurs parmi les suivants
 *     cos2048, saw2048, sin2048, smoothsquare8192, square_no_alias_2048,
 *     triangle2048, triangle_dist_cubed_2048, triangle_dist_squared_2048
 *     triangle_hermes_2048, triangle_valve_2048, triangle_valve_2_2048, triangle_warm8192
 *
 *   Dans ce cas, il faut inclure la table d'échantillon choisie (ligne 14) 
 *   et les définitions des oscillateurs aSin0, aSin1, aSin2, aSin3 (lignes 29 à 32) 
*/

#include <MozziGuts.h>
#include <Oscil.h>                            // oscillator template
#include <tables/triangle_hermes_2048_int8.h> // table d'échantillon pour l'oscillateur
#include <RollingAverage.h>
#include <ControlDelay.h>

#define BROCHE_PHOTORESISTANCE      0         // La photorésistance est reliée à cette broche

unsigned int echo_cells_1 = 32;
unsigned int echo_cells_2 = 60;
unsigned int echo_cells_3 = 127;

#define CONTROL_RATE 64
ControlDelay <128, int> kDelay;               // 2 secondes

// Oscillateurs
Oscil <TRIANGLE_HERMES_2048_NUM_CELLS, AUDIO_RATE> aSin0(TRIANGLE_HERMES_2048_DATA);
Oscil <TRIANGLE_HERMES_2048_NUM_CELLS, AUDIO_RATE> aSin1(TRIANGLE_HERMES_2048_DATA);
Oscil <TRIANGLE_HERMES_2048_NUM_CELLS, AUDIO_RATE> aSin2(TRIANGLE_HERMES_2048_DATA);
Oscil <TRIANGLE_HERMES_2048_NUM_CELLS, AUDIO_RATE> aSin3(TRIANGLE_HERMES_2048_DATA);

// Moyenne glissante sur x valeurs
// usage : RollingAverage <type de données, nombre de valeurs à moyenner> maVariable
RollingAverage <int, 32> kAverage; // le nombre de valeurs à moyenner doit être une puissance de 2 : 2, 4, 8, 16, 32, etc.
int averaged;

void setup(){
  kDelay.set(echo_cells_1);
  startMozzi();
}

void updateControl(){
  
  // Récupérer le signal brut de la photorésistance
  // Pour définir la fréquence
  int bumpy_input = mozziAnalogRead(BROCHE_PHOTORESISTANCE) * 3 + 100; // fréquence entre 100 et 3169 Hertz

  // Calcul de la moyenne glissante 
  averaged = kAverage.next(bumpy_input);

  // Modification de la fréquence des différents oscillateurs
  aSin0.setFreq(averaged);
  aSin1.setFreq(kDelay.next(averaged));
  aSin2.setFreq(kDelay.read(echo_cells_2));
  aSin3.setFreq(kDelay.read(echo_cells_3));
}

int updateAudio(){
  // Calcul de l'échantillon à jouer par déclage de bit ( ~= division)
  return 3*((int)aSin0.next()+aSin1.next()+(aSin2.next()>>1)
    +(aSin3.next()>>2)) >>3;
}

void loop(){
  audioHook();
}


