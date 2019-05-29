/* thereminiminimum
 *  THSF#10, mai 2019
 *  test photorésistance
 *  Ouvrir le traceur série de l'IDE arduino pour voir les valeurs
 *  photorésistances en diviseur de tension, avec une résistance de 2.2k
 */
 
#define BROCHE_PHOTORESISTANCE_1     A0
#define BROCHE_PHOTORESISTANCE_2     A1

int signal_photoresistance_1;
int signal_photoresistance_2;

void setup() {
  Serial.begin(9600);
}

void loop() {

  // Récupérer les valeurs sur les broches
  signal_photoresistance_1 = analogRead(BROCHE_PHOTORESISTANCE_1);
  signal_photoresistance_2 = analogRead(BROCHE_PHOTORESISTANCE_2);

  // Envoyer les valeurs pour qu'elles soient affichables dans le traceur série
  Serial.print(signal_photoresistance_1);
  Serial.print(",");
  Serial.println(signal_photoresistance_2);
  
  delay(50); // 1 valeur toutes les 50 ms = 20 valeurs par secondes
}
