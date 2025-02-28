#define BLYNK_TEMPLATE_ID "user7"
#define BLYNK_TEMPLATE_NAME "user7@server.wyns.it"
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>

// Blynk-authenticatie (vervang door jouw token uit de Blynk-app)
char auth[] = "cprYrmoKFJSwvZxmdoOrCAkz7fWUBI1E";
char ssid[] = "telenet-63E5C";         
char pass[] = "MaRiAm25";   

// Pin-definities
#define DHTPIN 4        
#define DHTTYPE DHT11   
#define HEATER_LED 2    

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;
float setTemp = 22.0;  

// Lees de gewenste temperatuur uit de Blynk-slider (V0)
BLYNK_WRITE(V0) {  
  setTemp = param.asFloat();  
}

void sendSensorData() {
  float temp = dht.readTemperature();  

  if (isnan(temp)) {
    Serial.println("Fout: DHT11 kan geen temperatuur lezen!");
    return;
  }

  // Stuur de gemeten temperatuur naar de Blynk Gauge (V1)
  Blynk.virtualWrite(V1, temp);
  
  // Controleer of de verwarming aan moet
  if (temp < setTemp) {
    digitalWrite(HEATER_LED, HIGH);  
  } else {
    digitalWrite(HEATER_LED, LOW);   
  }

  // Print naar de Serial Monitor
  Serial.print("Gemeten temperatuur: ");
  Serial.print(temp);
  Serial.print("°C | Gewenst: ");
  Serial.print(setTemp);
  Serial.print("°C | Ketel: ");
  Serial.println(temp < setTemp ? "AAN" : "UIT");
}

void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass, "server.wyns.it", 8081);
  dht.begin();
  pinMode(HEATER_LED, OUTPUT);
  timer.setInterval(2000L, sendSensorData);  
}

void loop() {
  Blynk.run();
  timer.run();
}
