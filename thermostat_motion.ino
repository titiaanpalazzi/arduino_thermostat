#include <LiquidCrystal.h>

const int temperaturePin = 0;
const int sensorPin = 1;
const int ledPin = 13; 
const int temperatureSet = 24; // define the threshold value of the temperature sensor
LiquidCrystal lcd(12,11,5,4,3,2);

int lightLevel, high = 0, low = 1023;
int lightBinary = LOW;
int expectancy = 4;
int count = 0;

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  lcd.begin(16,2);
  lcd.clear();
  lcd.print("Temp.");
  lcd.setCursor(0,1);
  lcd.print("Setpoint");
}

void loop() {
  float voltage, degreesC; 
  voltage = getVoltage(temperaturePin);
  degreesC = (voltage - 0.5) * 100.0;
  
  lightLevel = analogRead(sensorPin);
  autoTune();  // have the Arduino do the work for us!
  
  if(lightBinary == HIGH && expectancy < 20){
    expectancy ++;
  } else if(lightBinary == LOW && expectancy > 0){
    expectancy = expectancy -3;
  }
  Serial.print("Too cold?: ");
  Serial.println(degreesC);
  Serial.print("Movement detected?: ");
  Serial.println(1-lightBinary);
  Serial.print("No one at home (<4)?: ");
  Serial.println(20-expectancy);
  lcd.setCursor(10,0);
  lcd.print(degreesC);
  lcd.setCursor(10,1);
  lcd.print(temperatureSet);
  
  // if the voltage varies between 0 and 5 volts, the maximum temperature measured
  // equals 450 C
  if((degreesC < temperatureSet) && (expectancy < 17)) {
    digitalWrite(ledPin,HIGH);
  } else {
    digitalWrite(ledPin,LOW);
  }
  delay(3000);
}

float getVoltage(int pin)
{
  return (analogRead(pin) * 0.004882814);
}

void autoTune()
{  
  // Find lowest and highest values by putting your finger in front of the photoresistor sensing surface.
  if (lightLevel < low)
  {
    low = lightLevel;
  }

  if (lightLevel > high)
  {
    high = lightLevel;
  }
  
  //TP: we are mapping the values between 0 and 1 here, so we can later make them binary. 
  //'Bumming' this could happen by mapping the values between 0 and 1 with no decimals. 
  //In that case, the latter if-loop is unnecessary
  
  lightLevel = map(lightLevel, low+30, high-30, 0, 1);
  lightLevel = constrain(lightLevel, 0, 1);
  
  if(lightLevel <= 0.5) {
    lightBinary = LOW;
  } else {
    lightBinary = HIGH;
  }
}
