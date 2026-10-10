#include <Adafruit_NeoPixel.h> //lese med farge/høyde bar

#include <TM1637Display.h> //lese tall 


//husk at pinMode er for DIGITALE SENSORER 
//#define er for analoge sensorer


#define Startmotor_pin A0  //sensor startmotor
#define Pumpe_pin A1    //sensor pumpe
#define Gloedeplugg_pin A2  //sensor glødeplugg
#define Referansespenning 5.0 //står på webside 
#define ADC_ 1024.0 //Arduino Uno sin ADC har maks 10 bit resolution 
// aka 2^10 = 1024 (output)
#define Spenning_uten_stroem 2.485 //er 2.5 V -> sjekket i databladet
// for sensorer som bruker hall effekt pleier det å være !/" av Referansespenning
#define Sensitivitet 12.5 // -> må dele på 1000 senere 

//TM1637
//vi bruker 4 kabler - CLK og DIO for å kontrollere display  

#define CLK 2
#define DIO 3

//lager en ny instanse av klassen
TM1637Display display(CLK, DIO);

#define PIN_NEO_PIXEL 4 //pin som connects til NeoPixel
#define NUM_PIXELS 16 // det er 16 piksler 

#define amtMsrAvrg 10

float measurements[amtMsrAvrg] = {};
int i = 0;

Adafruit_NeoPixel NeoPixel(NUM_PIXELS, PIN_NEO_PIXEL, NEO_GRB + NEO_KHZ800);

float Lese_Stroem(int pin){ //lese strøm fra snesn0r
    int sensor_Verdi = analogRead(pin);
    float spenning = ((float)sensor_Verdi/ADC_)*Referansespenning;
    //float spenning = (sensor_Verdi * (R2/R1+R2))
    //float spenning = Referansespenning + ((Sensitivitet/1000)*sensor_Verdi);
    //float stroem = (spenning - Spenning_uten_stroem)/(Sensitivitet/1000.0);
    //float stroem = (2.485 - ((sensor_Verdi/ADC_)*Referansespenning))/0.0125;
    //(2.485 - (V_in*(R2/R1+R2)))/0.012 = I 
    float stroem = (2.485 - spenning)/0.0125;
    return sensor_Verdi;

}

void vis_paa_display(float stroem){
    //int viseVerdi = int(stroem*10); 
    //lager det til heltallverdier som er enklere å vise 
    display.showNumberDec(int(stroem), false);
     //from what i understand this is supposed to work 
  // vi vil ikke vse decimal point så vi sier false
    
}

void opdater_LED(float stroem){
    int antallLEDpaa = map(stroem, 0, 100, 0, NUM_PIXELS); //lager skala 0-10 A
    NeoPixel.clear();
    for (int i = 0; i < antallLEDpaa; i++) {
  
      if (i - 1 < NUM_PIXELS / 3) {  
        NeoPixel.setPixelColor(i, NeoPixel.Color(0, 30, 0)); // Green (low range) 
    } else if (i - 1 < 2 * NUM_PIXELS / 3) {
        NeoPixel.setPixelColor(i, NeoPixel.Color(15, 15, 0)); // Yellow (mid range)
    } else {
        NeoPixel.setPixelColor(i, NeoPixel.Color(30, 0, 0)); // Red (high range)
    }
    
    //NeoPixel.show(); // viser på stripen 
    //NeoPixel.fill(); //fyller med en farge 
    //NeoPixel.rainbow(); //gir regnbue effekt 
    //NeoPixel.setPixelColor(pixelIndex, r, g, b); // gi spesifikk farge??? im nor sure

    NeoPixel.show(); //oppdaterer LED stripen 
    }

}

void setup(){
  Serial.begin(9600); //baud rate er 9600 bps
  //display.init();            // Initialize TM1637 display
  NeoPixel.begin();          // Initialize NeoPixel strip
  NeoPixel.clear();          // Clear the LEDs 
  display.setBrightness(7); //brightest settiing for the brightest bitchessss

}

void loop(){
    float stroemStartmotor = Lese_Stroem(Startmotor_pin);

    measurements[i] = stroemStartmotor;
    i++;
    i %= amtMsrAvrg;
    float s = 0;
    for (int j = 0; j < amtMsrAvrg; j++) {
      s += measurements[j];
    }
    s /= amtMsrAvrg;

    //float stroemPumpe = Lese_Stroem(Pumpe_pin);
    //float stroemGloedeplugg = Lese_Stroem(Gloedeplugg_pin);

    //Serial.print("Startmotor strøm (i A):");
    //Serial.print(stroemStartmotor);

    //Serial.print("Strøm Pumpe (i A):");
    //Serial.print(stroemPumpe);

    //Serial.print("Strøm Glødeplugg ( i A):");
    //Serial.print(stroemGloedeplugg);
    //Serial.print("~~~~~~~~~~~~~~~~~~~~~~");

    vis_paa_display(s);

    opdater_LED(s);

     //oppdaterer seg hvert halvt sekund

}

//(2.485 - Uout)/0.012 = I

// (2.485 - (V_in*(R2/R1+R2)))/0.012 = I 

//Vout = Vin * (R2/R1+R2)
//(2.485 - ((sensor_Verdi/ACD_)*Referansespenning))/0.0125 = I 
