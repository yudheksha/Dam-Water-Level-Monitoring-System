#include "ThingSpeak.h"
#include <ESP8266WiFi.h>
#include <Servo.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
Servo servo;
const int trigPin1 = D1;
const int echoPin1 = D2;
unsigned long ch_no = 1525190;
const char * write_api = "6P80S1Y5IAP6FEU9";
char auth[] = "bhumika00";
unsigned long startMillis;
unsigned long currentMillis;
const unsigned long period = 10000;
WiFiClient client1;
long duration1;
int distance1;
// Initialize Telegram BOT
#define BOTtoken "5016988989:AAEJuqz5JCzivAxfmJ3Lce5pDUNl9_WENg0" // your Bot Token(Get from Botfather)
#define CHAT_ID "1390131791"
X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);
void setup()
{
pinMode(trigPin1, OUTPUT);
pinMode(echoPin1, INPUT);
Serial.begin(9600);
configTime(0, 0, "pool.ntp.org"); // get UTC time via NTP
client.setTrustAnchors(&cert); // Add root certificate for api.telegram.org
WiFi.begin("Abcd","jacd0605");
while (WiFi.status() != WL_CONNECTED)
{
delay(500);
Serial.print(".");
}
Serial.println("WiFi connected");
Serial.println(WiFi.localIP());
ThingSpeak.begin(client1);
bot.sendMessage(CHAT_ID, "Bot started up", "");
startMillis = millis(); //initial start time
servo.attach(2); //D4
servo.write(0);
delay(2000);
}
void loop()
{
digitalWrite(trigPin1, LOW);
delayMicroseconds(2);
digitalWrite(trigPin1, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin1, LOW);
duration1 = pulseIn(echoPin1, HIGH);
distance1 = duration1 * 0.034 / 2;
Serial.println(distance1);
if (distance1 <= 4)
{
servo.write(180);
delay(100);
}
else
{
servo.write(0);
delay(1000);
}
currentMillis = millis();
if (currentMillis - startMillis >= period)
{
ThingSpeak.setField(1, distance1);
ThingSpeak.writeFields(ch_no, write_api);
startMillis = currentMillis;
}
if (distance1 <= 4)
bot.sendMessage(CHAT_ID, "Flood Alert!!", "");
}
