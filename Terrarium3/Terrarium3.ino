#include <Adafruit_AM2320.h>
#include <Wire.h>
#include <DS3231.h>       // zegar
#include <LiquidCrystal.h>       // ekran
#include "DHT.h"          // czujnik

#define DHTTYPE DHT22    

const int orzesioneSensorPin = 2;
const int screen4 = 4;
const int screen5 = 5;
const int screen6 = 6;
const int screen7 = 7;
const int screen8 = 8;
const int screen9 = 9;
const int waterPin = 10;
const int orzesioneLightPin = 11;
const int redLightPin = 1;
const int x = 13;

// Select the pin used on LCD
LiquidCrystal lcd(screen8, screen9, screen4, screen5, screen6, screen7);

// define the button
int lcd_key = 0;
int adc_key_in = 0;

#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

//read the button value
int read_LCD_buttons()
{
	adc_key_in = analogRead(0);          // read analog A0 value
	// when read the 5 key values in the vicinity of the following：0,144,329,504,741
	// By setting different threshold, you can read the one button
	if (adc_key_in > 1000) return btnNONE;
	if (adc_key_in < 50)   return btnRIGHT;
	if (adc_key_in < 250)  return btnUP;
	if (adc_key_in < 450)  return btnDOWN;
	if (adc_key_in < 650)  return btnLEFT;
	if (adc_key_in < 850)  return btnSELECT;

	return btnNONE;
}

DHT orzesioneSensor(orzesioneSensorPin, DHTTYPE);

RTClib RTC;
const int orzesioneLightOnHour = 9;
const int orzesioneLightOffHour = 20;

const int waterOnTime = 16 * 1000; //czas lania wody w milisekundach
const int waterOn1 = 10;
const int waterOn2 = 13;
const int waterOn3 = 16;
const int waterOn4 = 22;

bool redLightOn = false;
bool showTemp = true;

void setup() {
	Serial.begin(9600);
	Wire.begin();

	orzesioneSensor.begin();

	pinMode(orzesioneLightPin, OUTPUT);
	pinMode(waterPin, OUTPUT);

	digitalWrite(waterPin, HIGH);

	Serial.println("Start:");

	lcd.begin(16, 2);             
	lcd.setCursor(0, 0);
}

void loop() { ///////////////// LOOP //////////////////////////

 // Odczyt temperatury i wilgotności powietrza
	float orzesioneTemp = orzesioneSensor.readTemperature();
	float orzesioneHumidity = orzesioneSensor.readHumidity();
	
	lcd.setCursor(0, 0);
  ShowTempAndHumidity(orzesioneTemp, orzesioneHumidity);
  
	if (redLightOn)
	{	
		digitalWrite(redLightPin, LOW);
	}

	//delay(1000);
	DateTime now = RTC.now();
	//ShowDateOnSerial(now);

	SwitchLight(now, orzesioneLightOnHour, orzesioneLightOffHour, orzesioneLightPin);

	Water(now);

	/////////////// LCD //////////////////////////////

  // lcd.print(millis()/1000);     // Output waiting time 
	lcd_key = read_LCD_buttons();    // read key

	switch (lcd_key)                 // display key
	{
  	case btnRIGHT:
  	{
  		break;
  	}
  	case btnLEFT:
  	{
  		break;
  	}
  	case btnUP:
  	{
  		break;
  	}
  	case btnDOWN:
  	{
  		break;
  	}
  	case btnSELECT:
  	{
  		delay(300);
  		redLightOn = !redLightOn;
  		break;
  	}
  	case btnNONE:
  	{
  		//lcd.print("NONE  ");
  		break;
  	}
	}
}

void ShowTempAndHumidity(float temp, float humidity) {
	if (isnan(temp) || isnan(humidity))
	{
		Serial.println("Blad odczytu danych z czujnika");
	}
	else
	{ 
	  	lcd.setCursor(0, 0);	    // On LCD	
			lcd.print("Temp: ");
			lcd.print(temp);
			lcd.print(" st. C");
	
      lcd.setCursor(0, 1);
			lcd.print("Wilg: ");
			lcd.print(humidity);
			lcd.print("%");
		
		Serial.print("Wilgotnosc: "); // On Port Monitor
		Serial.print(humidity);
		Serial.print(" % ");
		Serial.print("Temperatura: ");
		Serial.print(temp);
		Serial.println(" *C");
	}

}

void Water(DateTime now) {

	if (now.hour() == waterOn1 && now.minute() == 1)  // TODO: stworzyc jedna motede uruchamiania wody
	{
		digitalWrite(waterPin, LOW);
		delay(waterOnTime);
		digitalWrite(waterPin, HIGH);
		delay(70000);
	}

	if (now.hour() == waterOn2 && now.minute() == 1)
	{
		digitalWrite(waterPin, LOW);
		delay(waterOnTime);
		digitalWrite(waterPin, HIGH);
		delay(70000);
	}

	if (now.hour() == waterOn3 && now.minute() == 1)
	{
		digitalWrite(waterPin, LOW);
		delay(waterOnTime);
		digitalWrite(waterPin, HIGH);
		delay(70000);
	}

	if (now.hour() == waterOn4 && now.minute() == 1)
	{
		digitalWrite(waterPin, LOW);
		delay(waterOnTime);
		digitalWrite(waterPin, HIGH);
		delay(70000);
	}

}

void SwitchLight(DateTime now, int onHour, int offHour, int lightPin)
{
  if (now.hour()>= onHour && now.hour() < offHour)
  {
    digitalWrite(orzesioneLightPin, LOW);
  }
  else
  {
    digitalWrite(orzesioneLightPin, HIGH);
  }
}

void ShowDateOnSerial(DateTime now)
{
	Serial.print(now.year(), DEC);
	Serial.print('/');
	Serial.print(now.month(), DEC);
	Serial.print('/');
	Serial.print(now.day(), DEC);
	Serial.print(' ');
	Serial.print(now.hour(), DEC);
	Serial.print(':');
	Serial.print(now.minute(), DEC);
	Serial.print(':');
	Serial.print(now.second(), DEC);
	Serial.println();
}
