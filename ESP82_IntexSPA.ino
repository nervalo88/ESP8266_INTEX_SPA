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
	char temp[600];
	int sec = millis() / 1000;
	int min = sec / 60;
	int hr = min / 60;

	snprintf(temp, 600,

		"<html>\
			<head>\
		    <meta http-equiv='refresh' content='5'/>\
			<title>INTEX SPA</title>\
			<link href=\"https://stackpath.bootstrapcdn.com/bootstrap/4.4.1/css/bootstrap.min.css\" rel=\"stylesheet\" integrity=\"sha384-Vkoo8x4CGsO3+Hhxv8T/Q5PaXtkKtu6ug5TOeNV6gBiFeWPGFN9MuhOf23Q9Ifjh\" crossorigin=\"anonymous\">\
			</head>\
			<body>\
			<h1>INTEX SPA ESP8266 Host</h1>\
			<h3>Controls</h3>\
			<a href=\"/heat\"> Heater </a>\
			<a href=\"/power\"> Power </a>\
			<h3>Status</h3>\
			<p> Temp : %3.2f </p>\
			<p> Pump : %d </p>\
			<p> Heat : %d </p>\
			<p> Bubbles : %d </p>\
			<em>Copyright Nervalo88</em>\
			</body>\
		</html>",
		getTemperature(), digitalRead(IN_FILTERING), digitalRead(IN_HEATER), digitalRead(IN_JET_PUMP)
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

float getTemperature() {
	//get temperature
	tempSensors.requestTemperatures();
	float tempC = tempSensors.getTempCByIndex(0);
	if (tempC == DEVICE_DISCONNECTED_C)
	{
		Serial.println("ERROR: Could not read temperature data");
	}
	return tempC;
}

void sendStatus() {

	char out[400];

	
	
	int pumpState = digitalRead(IN_FILTERING);
	int heatState = digitalRead(IN_HEATER);
	int bubbleState = digitalRead(IN_JET_PUMP);

	snprintf(out, 400, 
		"{\
			\"temp\" : %3.1f\,\
			\"pump\" : %d\,\
			\"heat\" : %d\,\
			\"bubble\" : %d\
		}",getTemperature(),pumpState,heatState,bubbleState);

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
