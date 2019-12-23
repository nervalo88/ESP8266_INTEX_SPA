#pragma once
/*
                GPIO
Output	power	D6	12	
        heat	D5	14	
        
                    Couleur	LevConvPin	    GPIO
input	Heater	    gris	HV1	            D0	16
        Filtering	noir	HV2	            D2	4
        JetPump	    Blanc	HV3	            D1  5
        DS18B20                             D7  13

D4 & D3 used as inputs, but was not working
*/

#define DS18B20_BUS 13

#define OUT_POWER		12
#define OUT_HEAT		14

#define IN_HEATER		16
#define IN_FILTERING	4
#define IN_JET_PUMP		5
