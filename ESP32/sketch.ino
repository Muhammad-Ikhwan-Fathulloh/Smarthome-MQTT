#include <WiFi.h>
#include <MQTT.h>
#include <DHTesp.h>

DHTesp dhtSensor;

const char ssid[] = "Wokwi-GUEST";
const char pass[] = "";

WiFiClient net;
MQTTClient client;

const int kontak1 = 4;
const int kontak2 = 2;
const int kontak3 = 5;
const int kontak4 = 18;

const int echo = 12;
const int trigger = 13;

#include <LiquidCrystal_I2C.h>    //Library LCD I2C
LiquidCrystal_I2C lcd(0x27,16,2);   //Alamat I2C

unsigned long lastMillis = 0;

float suhu;
float kelembaban;
String lama_waktu;
String temp_after;
String hum_after;

void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nconnecting...");
  while (!client.connect("arduino", "public", "public")) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");

  client.subscribe("koalawan/iot/message");
  client.subscribe("koalawan/iot/kontak1");
  client.subscribe("koalawan/iot/kontak2");
  client.subscribe("koalawan/iot/kontak3");
  client.subscribe("koalawan/iot/kontak4");
}

void messageReceived(String &topic, String &payload) {
  Serial.println(topic + ": " + payload);

  if (String(topic) == "koalawan/iot/kontak1") {
    Serial.print("Changing output to ");
    if(payload == "1"){
      Serial.println(payload);
      digitalWrite(kontak1, HIGH);
    }
    else if(payload == "0"){
      Serial.println(payload);
      digitalWrite(kontak1, LOW);
    }
  }

  if (String(topic) == "koalawan/iot/kontak2") {
    Serial.print("Changing output to ");
    if(payload == "1"){
      Serial.println(payload);
      digitalWrite(kontak2, HIGH);
    }
    else if(payload == "0"){
      Serial.println(payload);
      digitalWrite(kontak2, LOW);
    }
  }

  if (String(topic) == "koalawan/iot/kontak3") {
    Serial.print("Changing output to ");
    if(payload == "1"){
      Serial.println(payload);
      digitalWrite(kontak3, HIGH);
    }
    else if(payload == "0"){
      Serial.println(payload);
      digitalWrite(kontak3, LOW);
    }
  }

  if (String(topic) == "koalawan/iot/kontak4") {
    Serial.print("Changing output to ");
    if(payload == "1"){
      Serial.println(payload);
      digitalWrite(kontak4, HIGH);
    }
    else if(payload == "0"){
      Serial.println(payload);
      digitalWrite(kontak4, LOW);
    }
  }

  if (String(topic) == "koalawan/iot/message") {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Smarthome");
    lcd.setCursor(0,1);
    lcd.print(payload);
  }
}

float fuzzy_cold(){
  float cold;

  if (suhu <= 0){
    cold = 1;
  } else if (suhu >= 0 && suhu <= 3){
    cold = (3 - suhu) / 3;
  } else if (suhu >= 3){
    cold = 0;
  }

  return cold;
}

float fuzzy_cool(){
  float cool;

  if(suhu <= 0 || suhu >= 15){
    cool = 0;
  } else if (suhu >= 0 && suhu <= 7.5){
    cool = (suhu - 0) / 7.5;
  } else if (suhu >= 7.5 && suhu <= 15){
    cool = (15 - suhu) / 7.5;
  }
  return cool;
}

float fuzzy_normal(){
  float normal;

  if (suhu <= 12 || suhu >= 27){
    normal = 0;
  } else if (suhu >= 12 && suhu <= 19.5){
    normal = (suhu - 12) / 7.5;
  } else if (suhu >= 19.5 && suhu <= 27){
    normal = (27 - suhu) / 7.5;
  }
  return normal;
}

float fuzzy_warm(){
  float warm;

  if (suhu <= 24 || suhu >= 39){
    warm = 0;
  } else if (suhu >= 24 && suhu <= 31.5){
    warm = (suhu - 24) / 7.5;
  } else if (suhu >= 31.5 && suhu <= 39){
    warm = (39 - suhu) / 7.5;
  }
  return warm;
}

float fuzzy_hot(){
  float hot;

  if (suhu <= 36){
    hot = 0;
  } else if (suhu >= 36 && suhu <= 39){
    hot = (36 - suhu) / 3;
    hot = -hot;
  } else if (suhu >= 39){
    hot = 1;
  }
  return hot;
}

float fuzzy_dry(){
  float dry;

  if (kelembaban <= 10){
    dry = 1;
  } else if (kelembaban >= 10 && kelembaban <= 20){
    dry = (20 - kelembaban) / 10;
  } else if (kelembaban >= 20){
    dry = 0;
  }
  return dry;
}

float fuzzy_moist(){
  float moist;

  if (kelembaban <= 10 || kelembaban >= 50){
    moist = 0;
  } else if (kelembaban >= 10 && kelembaban <= 30){
    moist = (kelembaban - 10) / 20;
  } else if (kelembaban >= 30 && kelembaban <= 50){
    moist = (50 - kelembaban) / 20;
  }
  return moist;
}

float fuzzy_wet(){
  float wet;

  if (kelembaban <= 40){
    wet = 0;
  } else if (kelembaban >= 40 && kelembaban <= 50){
    wet = (kelembaban - 40) / 10;
  } else if (kelembaban >= 50){
    wet = 1;
  }
  return wet;
}

void setup() {
  Serial.begin(115200);
  pinMode(kontak1, OUTPUT);
  pinMode(kontak2, OUTPUT);
  pinMode(kontak3, OUTPUT);
  pinMode(kontak4, OUTPUT);

  pinMode(echo, INPUT);
  pinMode(trigger, OUTPUT);

  dhtSensor.setup(15, DHTesp::DHT22);
  lcd.init();

  // start wifi and mqtt
  WiFi.begin(ssid, pass);
  client.begin("public.cloud.shiftr.io", net);
  client.onMessage(messageReceived);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Smarthome");
  lcd.setCursor(0,1);
  lcd.print("Koalawan");

  connect();
}

void loop() {
  client.loop();
  delay(10);

  // check if connected
  if (!client.connected()) {
    connect();
  }

  if (millis() - lastMillis > 3000) {
    lastMillis = millis();

    digitalWrite(trigger, LOW);
    delayMicroseconds(2);
    digitalWrite(trigger, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigger, LOW);
    int duration = pulseIn(echo, HIGH);
    float distance =  (duration / 2) / 29;
    String jarak = String(distance);

    TempAndHumidity data = dhtSensor.getTempAndHumidity();
    Serial.println("Temp " + String(data.temperature));

    String temp_value = String(data.temperature);

    Serial.println("Humd " + String(data.humidity));

    String humd_value = String(data.humidity);

    suhu = data.temperature;
    kelembaban = data.humidity;

    float cold = fuzzy_cold();
    float cool = fuzzy_cool();
    float normal = fuzzy_normal();
    float warm = fuzzy_warm();
    float hot = fuzzy_hot();
    float dry = fuzzy_dry();
    float moist = fuzzy_moist();
    float wet = fuzzy_wet();

    //Menentukan label kelembaban
    if (dry > moist && dry > wet){
      hum_after = "Dry";
    } else if (moist > dry && moist > wet){
      hum_after = "Moist";
    } else if (wet > dry && wet > moist){
      hum_after = "Wet";
    }

    //Menentukan label suhu
    if (cold > cool && cold > normal && cold > warm && cold > hot){
      temp_after = "Cold";
    } else if (cool > cold && cool > normal && cool > warm && cool > hot){
      temp_after = "Cool";
    } else if (normal > cold && normal > cool && normal > warm && normal > hot){
      temp_after = "Normal";
    } else if (warm > cold && warm > cool && warm > normal && warm > hot){
      temp_after = "Warm";
    } else if (hot > cold && hot > cool && hot > normal && hot && warm){
      temp_after = "Hot";
    }

    client.publish("koalawan/iot/distance", jarak);
    client.publish("koalawan/iot/temperature", temp_value);
    client.publish("koalawan/iot/humidity", humd_value);
    client.publish("koalawan/iot/status_temp", temp_after);
    client.publish("koalawan/iot/status_humd", hum_after);
  }
}