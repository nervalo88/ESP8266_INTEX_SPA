/*
 Name:		ESP82_IntexSPA.ino
 Created:	15/12/2019 23:35:56
 Author:	renau



        ...pour le moment c'est un exemple !...
*/

#include "topSecret.h"
#include "pinOut.h"

#include <ESP8266WebServer.h>

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

void drawGraph() {
	String out = "";
	char temp[100];
	out += "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"400\" height=\"150\">\n";
	out += "<rect width=\"400\" height=\"150\" fill=\"rgb(250, 230, 210)\" stroke-width=\"1\" stroke=\"rgb(0, 0, 0)\" />\n";
	out += "<g stroke=\"black\">\n";
	int y = rand() % 130;
	for (int x = 10; x < 390; x += 10) {
		int y2 = rand() % 130;
		sprintf(temp, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke-width=\"1\" />\n", x, 140 - y, x + 10, 140 - y2);
		out += temp;
		y = y2;
	}
	out += "</g>\n</svg>\n";

	server.send(200, "image/svg+xml", out);
}

void setup(void)
{
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

	server.on("/", handleRoot);
	server.on("/test.svg", drawGraph);
	server.on("/inline", []() {
		server.send(200, "text/plain", "this works as well");
		});
	server.onNotFound(handleNotFound);
	server.begin();
	Serial.println("HTTP server started");

}

void loop(void)
{
	// call sensors.requestTemperatures() to issue a global temperature 
	// request to all devices on the bus
		// 
	tempSensors.requestTemperatures(); // Send the command to get temperatures
	// After we got the temperatures, we can print them here.
	// We use the function ByIndex, and as an example get the temperature from the first sensor only.
	float tempC = tempSensors.getTempCByIndex(0);

	// Check if reading was successful
	if (tempC != DEVICE_DISCONNECTED_C)
	{
		Serial.printf("%3.1f \n", tempC);
	}
	else
	{
		Serial.println("Error: Could not read temperature data");
	}
}
