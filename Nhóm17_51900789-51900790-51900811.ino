#include <FirebaseArduino.h>
#include <ESP8266WiFi.h>
#include <DHT.h>
#include <SFE_BMP180.h>
#include <Wire.h>

//#define ALTITUDE 20 (Viet Nam) 
#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN,DHTTYPE);
SFE_BMP180 pressure;

float temp;
float humi;
int sensorPin = A0;
int sensorValue2 = 0;
char status;
double T,P,p0,a;

//WiFi
const String SSID = "";
const String Password = "";

void setup() {

  Serial.begin(115200);
  Serial.println("");
  Serial.print("Connect to WiFi ");
  Serial.println(SSID);
  WiFi.begin(SSID, Password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  Firebase.begin("esp8266-weatherstation-bac30-default-rtdb.firebaseio.com", "s1VcNFb1rjagUb1HS5S3bP3UfLAWasuTh4zuDBQX");
  
  //setup bmp180
  if (pressure.begin())
    Serial.println("BMP180 init success");
  else
  {
    Serial.println("BMP180 init fail\n\n");
    while(1); // Pause forever.
  }
  
  //setup dht11
  dht.begin();

}

void loop() {
	
  //dht11
  humi = dht.readHumidity();
  temp = dht.readTemperature();
  
  if (isnan(humi) || isnan(temp))
  {
  Serial.println("Failed to read from DHT sensor!");
  return;
  }
  
  //dht11-humidity
  Serial.print("Humidity: ");
  Serial.print(humi);
  Serial.print(" %\t");
  
  
  //dht11-temperature
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.print(" °C ");
  
  
	
  //rain sensor
  sensorValue2 = analogRead(sensorPin);
  Serial.print("Rain value:       ");
  Serial.println(sensorValue2);
  

  //bmp180
  
  //read ALTITUDE
  //Serial.print("provided altitude: ");
  //Serial.print(ALTITUDE,0);
  //Serial.print(" meters, ");
  //Serial.print(ALTITUDE*3.28084,0);
  //Serial.println(" feet");

  //status = pressure.startTemperature();
  
  //check status
  //if (status != 0)
  //{
    //delay(status);
    //status = pressure.getTemperature(T);
	
    //if (status != 0)
    //{
	  //read temperature
	  //Serial.print("temperature: ");
      //Serial.print(T,2);
      //Serial.print(" deg C, ");
      //Serial.print((9.0/5.0)*T+32.0,2);
      //Serial.println(" deg F");
	  
    status = pressure.startPressure(3);
	  
    if (status != 0)
    {
      delay(status);
      status = pressure.getPressure(P,T);
		
      if (status != 0)
      {
        // read absolute pressure
		    Serial.print("absolute pressure: ");
        Serial.print(P,2);
        Serial.print(" mb ");

          //read sea-level pressure
          //p0 = pressure.sealevel(P,ALTITUDE);
          //Serial.print("relative (sea-level) pressure: ");
          //Serial.print(p0,2);
          //Serial.print(" mb, ");
          //Serial.print(p0*0.0295333727,2);
          //Serial.println(" inHg");
		  
		  //read computed altitude
          //a = pressure.altitude(P,p0);
          //Serial.print("computed altitude: ");
          //Serial.print(a,0);
          //Serial.print(" meters, ");
          //Serial.print(a*3.28084,0);
          //Serial.println(" feet");
      }
      else 
		    Serial.println("error retrieving pressure measurement\n");
    }
    else{
	    Serial.println("error starting pressure measurement\n");
    }
    
    Firebase.setInt("rain",sensorValue2);
    Firebase.setFloat("temp", temp);
    Firebase.setFloat("humidity", humi);
    Firebase.setFloat("pressure",P);
    //}
    //else Serial.println("error retrieving temperature measurement\n");
  //}
  //else Serial.println("error starting temperature measurement\n");
  Serial.println("");
  delay(5000);
}
