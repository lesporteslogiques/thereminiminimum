
/*  D'après Knob_LDR_x2_WavePacket / Tim Barrass 2013, CC by-nc-sa.
 *   arduino 1.8.5 + lib. Mozzi 2015-05-11
 *   
 *   https://sensorium.github.io/Mozzi/doc/html/class_wave_packet.html
*/

#include <WavePacket.h>
#include <RollingAverage.h>
#include <AutoMap.h>
 
#define BROCHE_PHOTORESISTANCE_1        0  
#define BROCHE_PHOTORESISTANCE_2        1  

// Valeurs minimales et maximales des paramètres
const int MIN_F = 5;       // Fréquence fondamentale, min
const int MAX_F = 100;

const int MIN_BW = 10;      // Largeur de bande de fréquence
const int MAX_BW = 100;

const int MIN_CF = 60;     // Fréquence du filtre
const int MAX_CF = 2000;


// Moyennes glissantes pour lisser les données de scontrôleurs
// usage : RollingAverage <type de données, nombre de données à moyenner> myThing
RollingAverage <int, 16> kAverageF;
RollingAverage <int, 16> kAverageBw;
RollingAverage <int, 16> kAverageCf;
AutoMap kMapF(0,1023,MIN_F,MAX_F);
AutoMap kMapBw(0,1023,MIN_BW,MAX_BW);
AutoMap kMapCf(0,1023,MIN_CF,MAX_CF);

WavePacket <DOUBLE> wavey; // DOUBLE selects 2 overlapping streams


void setup(){

  // wait before starting Mozzi to receive analog reads, so AutoRange will not get 0
  delay(200); 
  startMozzi();
}


void updateControl(){

  // Captation des valeurs brutes de photorésistance 
  int v1 = mozziAnalogRead(BROCHE_PHOTORESISTANCE_1);
  int v2 = mozziAnalogRead(BROCHE_PHOTORESISTANCE_2);
  int v3 = v1*v2 >> 8;

  // Définition des paramètres en fonction des photorésistances
  int fundamental = v1;              // Fréquence fondamentale
  fundamental = kMapF(fundamental);  // 
  
  int bandwidth = v3; 
  bandwidth = kMapBw(bandwidth);
  
  int centre_freq = v2; 
  centre_freq = kMapCf(centre_freq);

  wavey.set(fundamental, bandwidth, centre_freq);
}


int updateAudio(){
  return wavey.next()>>8; // >>8 for AUDIO_MODE STANDARD
}


void loop(){
  audioHook(); 
}
