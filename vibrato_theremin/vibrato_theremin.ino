/*  D'après Vibrato / Tim Barrass 2013, CC by-nc-sa.
 *   arduino 1.8.5 + lib. Mozzi 2015-05-11
 *   
 *   On peut changer le type de la forme d'onde des oscillateurs parmi les suivants
 *     cos2048, saw2048, sin2048, smoothsquare8192, square_no_alias_2048,
 *     triangle2048, triangle_dist_cubed_2048, triangle_dist_squared_2048
 *     triangle_hermes_2048, triangle_valve_2048, triangle_valve_2_2048, triangle_warm8192
 *
 *   Dans ce cas, il faut inclure la table d'échantillon choisie (ligne 15) 
 *   et les définitions des oscillateurs aSin0, aSin1, aSin2, aSin3 (lignes 24 et 25) 
*/

#include <MozziGuts.h>
#include <Oscil.h>
#include <tables/cos2048_int8.h>      // table des échantillons de l'oscillateur
#include <mozzi_midi.h>               // pour mtof
#include <mozzi_fixmath.h>

#define BROCHE_PHOTORESISTANCE_1         0   
#define BROCHE_PHOTORESISTANCE_2         1  

#define CONTROL_RATE 64               // Toujours une puissance de 2 (4, 8, 16, 32, 64, etc.)

Oscil<COS2048_NUM_CELLS, AUDIO_RATE> aCos(COS2048_DATA);
Oscil<COS2048_NUM_CELLS, AUDIO_RATE> aVibrato(COS2048_DATA);

const byte intensity = 255;

void setup(){
  startMozzi(CONTROL_RATE);
  aCos.setFreq(mtof(84.f));
  aVibrato.setFreq(15.f);
}


void updateControl(){
  
  // Définir le vibrato d'après la photoresistance
  float bumpy_input = mozziAnalogRead(BROCHE_PHOTORESISTANCE_1) >> 5; // Vibrato entre 0 et 32
  aVibrato.setFreq(bumpy_input);

  // Définir la note d'après la seconde photorésistance
  float note = mozziAnalogRead(BROCHE_PHOTORESISTANCE_2) / 16 + 30; // Note entre 64 et 94 (en MIDI)
  aCos.setFreq(mtof(note));
}


int updateAudio(){
    Q15n16 vibrato = (Q15n16) intensity * aVibrato.next();
    return aCos.phMod(vibrato); // Modulation de la fréquence par modulation de phase
}

void loop(){
  audioHook();
}
