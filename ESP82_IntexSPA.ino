/*
 Name:		ESP82_IntexSPA.ino
 Created:	15/12/2019 23:35:56
 Author:	renau

                GPIO
Output	power	D6	12	
        heat	D5	14	
        
                    Couleur	LevConvPin	    GPIO
input	Heater	    gris	HV1	            D0	16
        Filtering	noir	HV2	            D3	0
        JetPump	    Blanc	HV3	            D4	2
        DS18B20                             D7  13

        ...pour le moment c'est un exemple !...
*/

// the setup function runs once when you press reset or power the board
// Include the libraries we need
#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 13

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

/*
 * The setup function. We only start the sensors here
 */
void setup(void)
{
    // start serial port
    Serial.begin(115200);
    Serial.println("Dallas Temperature IC Control Library Demo");

    // Start up the library
    sensors.begin();
    sensors.setResolution(12);

    //Serial.print(sensors.getResolution(0), DEC);
}

/*
 * Main function, get and show the temperature
 */
void loop(void)
{
    // call sensors.requestTemperatures() to issue a global temperature 
    // request to all devices on the bus
    sensors.requestTemperatures(); // Send the command to get temperatures
    // After we got the temperatures, we can print them here.
    // We use the function ByIndex, and as an example get the temperature from the first sensor only.
    float tempC = sensors.getTempCByIndex(0);

    // Check if reading was successful
    if (tempC != DEVICE_DISCONNECTED_C)
    {
        Serial.printf("%3.1f \n",tempC);
    }
    else
    {
        Serial.println("Error: Could not read temperature data");
    }
}