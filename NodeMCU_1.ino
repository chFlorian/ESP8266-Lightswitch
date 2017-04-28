#include <ESP8266WiFi.h>
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>
#include <Servo.h>

#define WLAN_SSID ""
#define WLAN_PASS ""

#define MQTT_SERVER "192.168.2.118"
#define MQTT_PORT 1883
#define MQTT_USER ""
#define MQTT_PASS ""
#define MQTT_TOPIC ""

#define SERVO_PIN 0

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, MQTT_PORT, MQTT_USER, MQTT_PASS);
Adafruit_MQTT_Subscribe topic = Adafruit_MQTT_Subscribe(&mqtt, MQTT_TOPIC);

Servo myservo;
bool turnedUp = false;

void MQTT_connect();

void setup() {
  Serial.begin(115200);
  delay(10);

  myservo.attach(SERVO_PIN);
  myservo.write(90);

  Serial.println(F("MQTT Test Code"));

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());

  mqtt.subscribe(&topic);
}

void loop() {
  MQTT_connect();

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &topic) {
      Serial.print(F("Got: "));
      String msg = (char *)topic.lastread;
      Serial.println(msg);
      if(msg == "ON") {
        up();
        myservo.write(90);
      } else if (msg == "OFF") {
        down();
        myservo.write(90);
      }
    }
  }

  if(! mqtt.ping()) {
    mqtt.disconnect();
  }
}

void down() {
  rotateToPosition(140, 5);
}

void up() {
  rotateToPosition(20, 5);
}

void rotateToPosition(int target, int stepLength) {
  int i = 90;
  if(target < 90) {
    for(i; i>=target; i-=stepLength) {
      myservo.write(i);
      delay(100);
    }
  } else {
    for(i; i<=target; i+=stepLength) {
      myservo.write(i);
      delay(100);
    }
  }
}

void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
