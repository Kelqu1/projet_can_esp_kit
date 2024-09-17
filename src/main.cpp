#include <Arduino.h>
#include <ACAN2515.h>
#include <Wire.h>
#include <U8g2lib.h>

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

// Utilisation du MCP2515 relié par bus SPI
// Voir dans doc les pins à utiliser en fonction du type de cible (ESP32, arduino...)

#define cs_2515 5         // réglage broche CS
#define int_2515 4        // réglage broche Interuption 
#define f_2515 16000000    // réglage Fréquence du quartz de la carte MCP2515 
#define f_bus_can 250000        // réglage Vitesse du bus CAN

int RegT;
int RegH;
int RegL;
int val_aff;
int bariere =8191;

ACAN2515 can2515(cs_2515, SPI, int_2515); // Déclaration de l'objet CAN utilisant le réglages des broches définis avant


// Declaration d'un masque (param 1) qui utilise les 11 bits d'identification de message
// et qui n'utilise pas les 2 premiers octets de data (param 2 et 3) optionnels
const ACAN2515Mask masque = standard2515Mask(0b11111111111, 0, 0);

// Declaration procedure de traitement  filtre
void message_1octet(const CANMessage & inMessage);


//réglage identifiant trame voulue
const ACAN2515AcceptanceFilter filtres[] = {
   { standard2515Filter(0x3E1, 0, 0), message_1octet }, 

};

CANMessage messageCANReception;

void setup()
{

    {
  u8g2.setI2CAddress(0x78);
  u8g2.begin();
  u8g2.enableUTF8Print(); //nécessaire pour écrire des caractères accentués
}

	Serial.begin(9600);
	Serial.println("Init CAN ok");
	delay(500);
	SPI.begin();
	ACAN2515Settings can_vit(f_2515, f_bus_can);
    
	// Demarrage CAN (le 2eme param s'appelle "une closure" il permet de relier
	// le code à la fonction qui traite l'intéruption dans la lib ACAN2515)
	const uint16_t v_err = can2515.begin(can_vit, [] { can2515.isr(); },masque, filtres, 2 );   
	
	if (v_err == 0) 
    { 
        Serial.println("Recepteur: Config ok");
    } 
	else 
    {
        Serial.println("Recepteur: Config HS"); 
        while (1); 
    }
	
	Serial.println("Attente de message !");
	delay(500);
}

//Traitement message par fonction
void message_1octet(const CANMessage & inMessage)
{

}

void loop()
{
	RegT=(RegH<<8)+RegL;
  val_aff=RegT/8;
    if (can2515.receive(messageCANReception))  // test si un message est arrivé
    {
    // Un message CAN est arrive 
    
    can2515.receive(messageCANReception) ;
  
    Serial.println("Messages obtenus: ") ;
    Serial.println(messageCANReception.data[0]);
    Serial.println(messageCANReception.data[1]);
    RegH=messageCANReception.data[0];
    RegL=messageCANReception.data[1];
    }  
    
    delay(100);

  /********************* Dessiner des contours de formes géométriques ******************/

/******************* Écrire du texte *******************************/
  u8g2.setFont(u8g2_font_ncenB10_tf); // choix de la police
  u8g2.clearBuffer();
  u8g2.setCursor(0, 20); // position du début du texte
  u8g2.print(1);  // on écrit le texte
  u8g2.setCursor(15, 20); // position du début du texte
  u8g2.print(2);  // on écrit le texte
  u8g2.setCursor(30, 20); // position du début du texte
  u8g2.print(3);  // on écrit le texte
  u8g2.setCursor(45, 20); // position du début du texte
  u8g2.print(4);  // on écrit le texte
  u8g2.setCursor(60, 20); // position du début du texte
  u8g2.print(5);  // on écrit le texte
  u8g2.setCursor(75, 20); // position du début du texte
  u8g2.print(6);  // on écrit le texte
  u8g2.setCursor(90, 20); // position du début du texte
  u8g2.print(7);  // on écrit le texte
  u8g2.setCursor(105, 20); // position du début du texte
  u8g2.print(8);  // on écrit le texte
  u8g2.sendBuffer();
  delay(300);
}