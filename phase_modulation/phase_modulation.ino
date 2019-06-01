/*  D'après Knob_LightLevel_FMSynth / Tim Barrass 2013, CC by-nc-sa.
 *   arduino 1.8.5 + lib. Mozzi 2015-05-11
 *   
 *   On peut changer le type de la forme d'onde des oscillateurs parmi les suivants
 *     cos2048, saw2048, sin2048, smoothsquare8192, square_no_alias_2048,
 *     triangle2048, triangle_dist_cubed_2048, triangle_dist_squared_2048
 *     triangle_hermes_2048, triangle_valve_2048, triangle_valve_2_2048, triangle_warm8192
 *
 *   Dans ce cas, il faut inclure la table d'échantillon choisie (ligne 15) 
 *   et les définitions des oscillateurs aSin0, aSin1, aSin2, aSin3 (lignes 39 à 41) 
*/

#include <MozziGuts.h>
#include <Oscil.h> 
#include <tables/cos2048_int8.h>      // Table d'échantillon de l'oscillateur
#include <Smooth.h>
#include <AutoMap.h>              

#define BROCHE_PHOTORESISTANCE_1         0   
#define BROCHE_PHOTORESISTANCE_2         1   

// Intervalle de la fréquence de porteuse 
const int MIN_CARRIER_FREQ = 22;
const int MAX_CARRIER_FREQ = 220;

// Intervalle d'intensité 
const int MIN_INTENSITY = 700;
const int MAX_INTENSITY = 10;

// Intervalle de vitesse 
const int MIN_MOD_SPEED = 100;
const int MAX_MOD_SPEED = 1;

AutoMap kMapCarrierFreq(0,1023,MIN_CARRIER_FREQ,MAX_CARRIER_FREQ);
AutoMap kMapIntensity(0,1023,MIN_INTENSITY,MAX_INTENSITY);
AutoMap kMapModSpeed(0,1023,MIN_MOD_SPEED,MAX_MOD_SPEED);


Oscil<COS2048_NUM_CELLS, AUDIO_RATE> aCarrier(COS2048_DATA);
Oscil<COS2048_NUM_CELLS, AUDIO_RATE> aModulator(COS2048_DATA);
Oscil<COS2048_NUM_CELLS, CONTROL_RATE> kIntensityMod(COS2048_DATA);

int mod_ratio = 3; // Brillance (harmoniques) 
long fm_intensity; // carries control info from updateControl to updateAudio

// Lissage pour supprimer les clicks sur les transitions
float smoothness = 0.95f;
Smooth <long> aSmoothIntensity(smoothness);


void setup(){
  startMozzi(); // :))
}


void updateControl(){

  // Capture des valeurs brutes des photorésistances
  int v1 = mozziAnalogRead(BROCHE_PHOTORESISTANCE_1);
  int v2 = mozziAnalogRead(BROCHE_PHOTORESISTANCE_2);
  // Extrapolation d'une 3e valeur
  int v3 = v1*v2 >> 9;
  
  int knob_value = v1;
  
  // Définir la fréquence de porteuse 
  int carrier_freq = kMapCarrierFreq(knob_value);
  
  // Calculer la fréquence de modulation
  int mod_freq = carrier_freq * mod_ratio;
  
  // Définir les fréquences des oscillateurs
  aCarrier.setFreq(carrier_freq); 
  aModulator.setFreq(mod_freq);
  
  // Calculer l'intensité de la modulation de fréquence
  int LDR1_calibrated = kMapIntensity(v2);
  fm_intensity = ((long)LDR1_calibrated * (kIntensityMod.next()+128))>>8; // shift back to range after 8 bit multiply

  float mod_speed = (float)kMapModSpeed(v3)/1000;
  kIntensityMod.setFreq(mod_speed);
}


int updateAudio(){
  long modulation = aSmoothIntensity.next(fm_intensity) * aModulator.next();
  return aCarrier.phMod(modulation);
}


void loop(){
  audioHook();
}





