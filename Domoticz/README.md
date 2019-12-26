# DOMOTICZ Integration

### Hardware configuration
1. Create 5x virtual switches :
    * Power button (press)
    * Heater button (bress)
    * Output states : Water pump, heater, bubbles
2. Create 1x sensor
    * Temperature sensor

### Implement the script below to your eventSystem :

```lua
--
-- Domoticz passes information to scripts through a number of global tables
--
-- otherdevices, otherdevices_lastupdate and otherdevices_svalues are arrays for all devices: 
--   otherdevices['yourotherdevicename'] = "On"
--   otherdevices_lastupdate['yourotherdevicename'] = "2015-12-27 14:26:40"
--   otherdevices_svalues['yourotherthermometer'] = string of svalues
--
-- uservariables and uservariables_lastupdate are arrays for all user variables: 
--   uservariables['yourvariablename'] = 'Test Value'
--   uservariables_lastupdate['yourvariablename'] = '2015-12-27 11:19:22'
--
-- other useful details are contained in the timeofday table
--   timeofday['Nighttime'] = true or false
--   timeofday['SunriseInMinutes'] = number
--   timeofday['Daytime'] = true or false
--   timeofday['SunsetInMinutes'] = number
--   globalvariables['Security'] = 'Disarmed', 'Armed Home' or 'Armed Away'
--
-- To see examples of commands see: http://www.domoticz.com/wiki/LUA_commands#General
-- To get a list of available values see: http://www.domoticz.com/wiki/LUA_commands#Function_to_dump_all_variables_supplied_to_the_script
--
-- Based on your logic, fill the commandArray with device commands. Device name is case sensitive. 
--
local http = require("socket.http");
local json = require("JSON");
commandArray = {}

body,c,l,h = http.request("http://192.168.0.43/status");

local status = json:decode(body);
print(status.temp);

local function update(idx, value1, value2)
    local cmd = string.format("%d|%.2f|%.2f", idx, value1, value2)
    table.insert (commandArray, { ['UpdateDevice'] = cmd } )
end

update (44, 0, status.temp)
update (45, status.pump, 0)
update (46, status.heat, 0)
update (47, status.bubble, 0)

-- loop through all the devices
for deviceName,deviceValue in pairs(otherdevices) do
--    if (deviceName=='myDevice') then
--        if deviceValue == "On" then
--            print("Device is On")
--        elseif deviceValue == "Off" then
--            commandArray['a device name'] = "On"
--            commandArray['Scene:MyScene'] = "Off"
--        end
--    end
end

-- loop through all the variables
for variableName,variableValue in pairs(uservariables) do
--    if (variableName=='myVariable') then
--        if variableValue == 1 then
--            commandArray['a device name'] = "On"
--            commandArray['Group:My Group'] = "Off AFTER 30"
--        end
--    end
end

return commandArray
```