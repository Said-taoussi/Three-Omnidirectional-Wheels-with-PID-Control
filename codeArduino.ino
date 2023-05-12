
//#include <Simpletimer.h>

//sens positif pour 1er moteur est : 0
//sens positif pour 2eme moteur est : 0
//sens positif pour 3eme moteur est : 0

//Simpletimer timer;
float vx = 0.0;
float vy = 0.0;
float w = 0; // w en rad/s et 1rad = 0.159155 tours

char Incoming_value = 0;
float theta = 0;
unsigned long lastTime = 0;

const float d = 0.18;
const float R = 0.05;
const float prop = 0.15;
const float correspondant = 0.500;//0.215;//PI/10;
const float conversion = 0.159155;

float consigne_moteur = 0; //20
float consigne_moteur2 = 0; //-10
float consigne_moteur3= 0; //-10



int PWM = 8;
int DIR = 7;
int PWM2 = 6;
int DIR2 = 5;
int PWM3 = 10;
int DIR3 = 9;
const byte interruptPin = 19;
const byte interruptPin2 = 18;
const byte interruptPin3 = 2;
double Nfmd = 0;
double Nfmd2 = 0;
double Nfmd3 = 0;

unsigned long myTime;
double nbr_tours;
unsigned long myTime2;
double nbr_tours2;
unsigned long myTime3;
double nbr_tours3;

int vitMoteur = 0; 
unsigned int tick_codeuse = 0;
unsigned int tick_codeuse_ = 0;
float vit_roue_tour_sec = 0.0;
int vitMoteur2 = 0; 
unsigned int tick_codeuse2 = 0;
unsigned int tick_codeuse2_ = 0;
float vit_roue_tour_sec2 = 0.0;
int vitMoteur3 = 0; 
unsigned int tick_codeuse3 = 0;
unsigned int tick_codeuse3_ = 0;
float vit_roue_tour_sec3 = 0.0;

const int frequence_echantillonnage = 100;
const int sendFrequency = 20; //700 ms
 
float erreur_precedente = abs(consigne_moteur); // (en tour/s)
float somme_erreur = 0;
float erreur_precedente2 = abs(consigne_moteur2); // (en tour/s)
float somme_erreur2 = 0;
float erreur_precedente3 = abs(consigne_moteur3); // (en tour/s)
float somme_erreur3 = 0;

const float kp = 150; // Coefficient proportionnel (choisis par essais successifs)
const float ki = 0.001; // Coefficient intégrateur
const float kd = 0.0001; // Coefficient dérivateur
const int rapport_reducteur = 131; 
const int tick_par_tour_codeuse = 32;
unsigned long prv = 0; 
unsigned long prv2 = 0;  



  void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600); 
  pinMode(PWM, OUTPUT); 
  pinMode(DIR, OUTPUT);
  analogWrite(PWM,0); 
  digitalWrite(DIR, 0);
  delay(300);
  pinMode(interruptPin, INPUT_PULLUP);

  pinMode(PWM2, OUTPUT); 
  pinMode(DIR2, OUTPUT);
  analogWrite(PWM2,0); 
  digitalWrite(DIR2, 0);
  delay(300);
  pinMode(interruptPin2, INPUT_PULLUP);

  pinMode(PWM3, OUTPUT); 
  pinMode(DIR3, OUTPUT);
  analogWrite(PWM3,0); 
  digitalWrite(DIR3, 0);
  delay(300);
  pinMode(interruptPin3, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(interruptPin), increment, CHANGE);
  attachInterrupt(digitalPinToInterrupt(interruptPin2), increment2, CHANGE);
  attachInterrupt(digitalPinToInterrupt(interruptPin3), increment3, CHANGE);
  //timer.setInterval(1000 / frequence_echantillonnage, asservissement);

}

void loop() {
  // put your main code here, to run repeatedly:
  
   //timer.run(); //on fait tourner l'horloge
   //delay(10); 
   unsigned long tmp = millis();

if (Serial.available() > 0) { // check if there's data in the serial buffer
  String inputString = Serial.readStringUntil('\n'); // read the incoming data until a newline character is received
  inputString.trim(); // remove any leading or trailing whitespace
  int spaceIndex1 = inputString.indexOf(' '); // find the index of the first space character that separates the values
  int spaceIndex2 = inputString.indexOf(' ', spaceIndex1+1); // find the index of the second space character that separates the values
  if (spaceIndex1 != -1 && spaceIndex2 != -1) { // check if both space characters were found
    String value1String = inputString.substring(0, spaceIndex1); // extract the first value as a string
    String value2String = inputString.substring(spaceIndex1 + 1, spaceIndex2); // extract the second value as a string
    String value3String = inputString.substring(spaceIndex2 + 1); // extract the third value as a string
    float value1 = value1String.toFloat(); // convert the first value to a float and store it in the value1 variable
    float value2 = value2String.toFloat(); // convert the second value to a float and store it in the value2 variable
    float value3 = value3String.toFloat(); // convert the third value to a float and store it in the value3 variable
      vx = value1String.toFloat(); // convert the first value to a float and store it in the value1 variable
      vy = value2String.toFloat(); // convert the second value to a float and store it in the value2 variable
      w = value3String.toFloat(); // convert the second value to a float and store it in the value2 variable
  }
    consigne_moteur = 1*(vx-d*w)/R; 
    consigne_moteur2 = 1*(-0.5*vx - 0.866*vy-d*w)/R; //-10
    consigne_moteur3 = 1*(-0.5*vx + 0.866*vy-d*w)/R; //-10
    }
  
unsigned long currentTime = millis();
unsigned long elapsedTime = currentTime - lastTime;
unsigned long elapsedTime2 = currentTime - prv;

//if(elapsedTime2 >= 10) {
//    asservissement();
//    asservissement2();
//    asservissement3();
//    prv = currentTime;
//}
//if (consigne_moteur == 0 && consigne_moteur2 == 0 && consigne_moteur3 == 0) {
//  Tourner(0);
//  Tourner2(0);
//  Tourner3(0);
//}else if(consigne_moteur == 0 && consigne_moteur2 == 0 ) {
//  Tourner(0);
//  Tourner2(0);
//  asservissement3();
//  }else if (consigne_moteur == 0 && consigne_moteur3 == 0 ) {
//    Tourner(0);
//    Tourner3(0);
//    asservissement2();
//  }else  if (consigne_moteur2 == 0 && consigne_moteur3 == 0 ) {
//    Tourner2(0);
//    Tourner3(0);
//    asservissement();
//  }else  if (consigne_moteur == 0 ) {
//    Tourner(0);
//    asservissement2();
//    asservissement3();
//  }else  if (consigne_moteur2 == 0 ) {
//    Tourner2(0);
//    asservissement();
//    asservissement3();
//  }else  if (consigne_moteur3 == 0 ) {
//    Tourner3(0);
//    asservissement();
//    asservissement2();
//  }else  {
//    asservissement();
//    asservissement2();
//    asservissement3();
//  }
    asservissement();
    asservissement2();
    asservissement3();



//    Tourner2(0);
//    int frequence_codeuse = frequence_echantillonnage * tick_codeuse2; //100*tick_codeuse
// vit_roue_tour_sec2 = (float)frequence_codeuse / (float)tick_par_tour_codeuse / 
//(float)rapport_reducteur; //(100*tick_codeuse)/32/131
// // Réinitialisation du nombre de tick de la codeuse
// tick_codeuse2 = 0;
// Serial.println(vit_roue_tour_sec2);
// delay(10);


    if(elapsedTime >= 50) {
//  if (vit_roue_tour_sec < -3*(PI/10) || vit_roue_tour_sec > 3*(PI/10)) {
//    vit_roue_tour_sec = 0; // set the value to 0 instead of printing an error message
//  }
  if (consigne_moteur < 0) {
    Serial.print(-vit_roue_tour_sec/(correspondant)); // print the negative value to serial
  } else {
    Serial.print(vit_roue_tour_sec/(correspondant)); // print the positive value to serial
  }
  Serial.print(", "); // print a comma and a space to separate the values
  // print the other two values similarly, with appropriate sign changes if needed
//  if (vit_roue_tour_sec2 < -3*(PI/10) || vit_roue_tour_sec2 > 3*(PI/10)) {
//    vit_roue_tour_sec2 = 0; // set the value to 0 instead of printing an error message
//  }
  if (consigne_moteur2 < 0) {
    Serial.print(-vit_roue_tour_sec2/(correspondant)); // print the negative value to serial
  } else {
    Serial.print(vit_roue_tour_sec2/(correspondant)); // print the positive value to serial
  }
  Serial.print(", "); // print a comma and a space to separate the values
//  if (vit_roue_tour_sec3 < -3*(PI/10) || vit_roue_tour_sec3 > 3*(PI/10)) {
//    vit_roue_tour_sec3 = 0; // set the value to 0 instead of printing an error message
//  }
  if (consigne_moteur3 < 0) {
    Serial.println(-vit_roue_tour_sec3/(correspondant)); // print the negative value to serial with a newline character
  } else {
    Serial.println(vit_roue_tour_sec3/(correspondant)); // print the positive value to serial with a newline character
  }
    lastTime = currentTime;
}


 
}
void increment() {
  tick_codeuse++;
  
}
void increment2() {
  tick_codeuse2++;
}
void increment3() {
  tick_codeuse3++;
  
}

void asservissement()
{
 int frequence_codeuse = frequence_echantillonnage * tick_codeuse; //100*tick_codeuse
 vit_roue_tour_sec = (float)frequence_codeuse / (float)tick_par_tour_codeuse / 
(float)rapport_reducteur ; //(100*tick_codeuse)/32/131

 float erreur = abs(consigne_moteur) - vit_roue_tour_sec; // pour le proportionnel
 somme_erreur += erreur; // pour l'intégrateur
 float delta_erreur = erreur - erreur_precedente; // pour le dérivateur
 erreur_precedente = erreur;
 // Réinitialisation du nombre de tick de la codeuse
 tick_codeuse = 0;
 // P : calcul de la commande
 vitMoteur = kp*erreur + ki*somme_erreur + kd*delta_erreur; //somme des trois erreurs
 // Normalisation et contrôle du moteur
 if ( vitMoteur > 255 )
 {
 vitMoteur = 255; // sachant que l'on est branché sur un pont en H L293D
 }
 else if ( vitMoteur < 0 )
 {
 vitMoteur = 0;
 }
 Tourner (vitMoteur);
 // DEBUG
 /*Serial.print(vit_roue_tour_sec, 8); // affiche à gauche la vitesse et à droite l'erreur
 Serial.print(" : ");
 Serial.print(erreur, 4);
 Serial.print(" : ");
 Serial.print(vitMoteur);
 Serial.println();*/
// Serial.print(vitMoteur);
 //Serial.println(vit_roue_tour_sec*100, 8); // affiche à gauche la vitesse et à droite l'erreur
// Serial.print(" ; ");
// Serial.print(erreur, 4);
// Serial.print(" ; ");
 delay(10);
// Serial.println();
}

void asservissement2()
{
 // Calcul de l'erreur
 int frequence_codeuse = frequence_echantillonnage * tick_codeuse2; //100*tick_codeuse
vit_roue_tour_sec2 = (float)frequence_codeuse / (float)tick_par_tour_codeuse / 
(float)rapport_reducteur; //(100*tick_codeuse)/32/131
 float erreur2 = abs(consigne_moteur2) - vit_roue_tour_sec2; // pour le proportionnel
 somme_erreur2 += erreur2; // pour l'intégrateur
 float delta_erreur2 = erreur2 - erreur_precedente2; // pour le dérivateur
 erreur_precedente2 = erreur2;
 // Réinitialisation du nombre de tick de la codeuse
 tick_codeuse2 = 0;
 // P : calcul de la commande
 vitMoteur2 = kp*erreur2 + ki*somme_erreur2 + kd*delta_erreur2; //somme des trois erreurs
 // Normalisation et contrôle du moteur
 if ( vitMoteur2 > 255 )
 {
 vitMoteur2 = 255; // sachant que l'on est branché sur un pont en H L293D
 }
 else if ( vitMoteur2 < 0 )
 {
 vitMoteur2 = 0;
 }
 Tourner2 (vitMoteur2);
// Serial.println(vitMoteur2);
delay(10);
}

void asservissement3()
{
 // Calcul de l'erreur
 int frequence_codeuse = frequence_echantillonnage * tick_codeuse3; //100*tick_codeuse
 vit_roue_tour_sec3 = (float)frequence_codeuse / (float)tick_par_tour_codeuse / 
(float)rapport_reducteur ; //(100*tick_codeuse)/32/131

 float erreur3 = abs(consigne_moteur3) - vit_roue_tour_sec3; // pour le proportionnel
 somme_erreur3 += erreur3; // pour l'intégrateur
 float delta_erreur3 = erreur3 - erreur_precedente3; // pour le dérivateur
 erreur_precedente3 = erreur3;
 // Réinitialisation du nombre de tick de la codeuse
 tick_codeuse3 = 0;
 // P : calcul de la commande
 vitMoteur3 = kp*erreur3 + ki*somme_erreur3 + kd*delta_erreur3; //somme des trois erreurs
 // Normalisation et contrôle du moteur
 if ( vitMoteur3 > 255 )
 {
 vitMoteur3 = 255; // sachant que l'on est branché sur un pont en H L293D
 }
 else if ( vitMoteur3 < 0 )
 {
 vitMoteur3 = 0;
 }
 Tourner3 (vitMoteur3);
//  Serial.print(vit_roue_tour_sec, 8); // affiche à gauche la vitesse et à droite l'erreur
// Serial.print(" : ");
// Serial.print(erreur3, 4);
// Serial.print(" : ");
// Serial.println(vitMoteur3);
// Serial.println();
delay(10);
}


void Tourner( int powerRate )
{
 if (consigne_moteur<0){
  digitalWrite(DIR, 0);
 }
 else{
  digitalWrite(DIR, 1);
 }
 
 analogWrite(PWM, powerRate);
 /*digitalWrite(DIR, 0);*/
}

void Tourner2( int powerRate )
{
 if (consigne_moteur2<0){
  digitalWrite(DIR2, 1);
 }
 else{
  digitalWrite(DIR2, 0);
 }
 
 analogWrite(PWM2, powerRate);
 /*digitalWrite(DIR2, 0);*/
}

void Tourner3( int powerRate )
{
 if (consigne_moteur3<0){
  digitalWrite(DIR3, 1);
 }
 else{
  digitalWrite(DIR3, 0);
 }
 
 analogWrite(PWM3, powerRate);
 /*digitalWrite(DIR3, 0);*/
}
