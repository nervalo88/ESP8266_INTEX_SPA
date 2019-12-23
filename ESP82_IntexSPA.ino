/*
 Name:		ESP82_IntexSPA.ino
 Created:	15/12/2019 23:35:56
 Author:	renau



        ...pour le moment c'est un exemple !...
*/

#include "topSecret.h"
#include "pinOut.h"

#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include <OneWire.h>
#include <DallasTemperature.h>

ESP8266WebServer server(80);


OneWire oneWire(DS18B20_BUS);
DallasTemperature tempSensors(&oneWire);

void handleRoot() {
	char temp[400];
	int sec = millis() / 1000;
	int min = sec / 60;
	int hr = min / 60;

	snprintf(temp, 400,

		"<html>\
  <head>\
    <meta http-equiv='refresh' content='5'/>\
    <title>ESP8266 Demo</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <h1>Hello from ESP8266!</h1>\
    <p>Uptime: %02d:%02d:%02d</p>\
    <img src=\"/test.svg\" />\
  </body>\
</html>",

hr, min % 60, sec % 60
);
	server.send(200, "text/html", temp);
}

void handleNotFound() {

	String message = "File Not Found\n\n";
	message += "URI: ";
	message += server.uri();
	message += "\nMethod: ";
	message += (server.method() == HTTP_GET) ? "GET" : "POST";
	message += "\nArguments: ";
	message += server.args();
	message += "\n";

	for (uint8_t i = 0; i < server.args(); i++) {
		message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
	}

	server.send(404, "text/plain", message);
}

void switchPower() {

	digitalWrite(OUT_POWER, LOW);
	delay(500);
	digitalWrite(OUT_POWER, HIGH);

	server.send(200, "text/html", "Power switched !");

}

void switchHeat() {

	digitalWrite(OUT_HEAT, LOW);
	delay(500);
	digitalWrite(OUT_HEAT, HIGH);

	server.send(200, "text/html", "Heater switched !");

}

void sendStatus() {

	char out[400];

	//get temperature
	tempSensors.requestTemperatures(); 
	float tempC = tempSensors.getTempCByIndex(0);
	if (tempC == DEVICE_DISCONNECTED_C)
	{
		server.send(200,"text/html","Error: Could not read temperature data");
		return;
	}
	
	int pumpState = digitalRead(IN_FILTERING);
	int heatState = digitalRead(IN_HEATER);
	int bubbleState = digitalRead(IN_JET_PUMP);

	snprintf(out, 400, 
		"{\
			\"temp\" : %3.1f\,\
			\"pump\" : %d\,\
			\"heat\" : %d\,\
			\"bubble\" : %d\
		}",tempC,pumpState,heatState,bubbleState);

	server.send(200, "application/json", out);
}

void setup(void)
{
	pinMode(OUT_POWER, OUTPUT);
	digitalWrite(OUT_POWER, HIGH);
	pinMode(OUT_HEAT, OUTPUT);
	digitalWrite(OUT_HEAT, HIGH);

	// start serial port
	Serial.begin(115200);

	WiFi.mode(WIFI_STA);
	WiFi.begin(ssid, password);
	Serial.println("");

	// Wait for connection
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}

	Serial.println("");
	Serial.print("Connected to ");
	Serial.println(ssid);
	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());

	Serial.println(ssid);

	tempSensors.begin();
	tempSensors.setResolution(12);
	
	if (MDNS.begin("esp8266")) {
		Serial.println("MDNS responder started");
	}

	server.on("/", handleRoot);
	server.on("/power", switchPower);
	server.on("/heat", switchHeat);
	server.on("/status", sendStatus);
	server.onNotFound(handleNotFound);

	server.begin();

	Serial.println("HTTP server started");

}

void loop(void)
{


	server.handleClient();
}
