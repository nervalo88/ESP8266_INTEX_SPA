# DOMOTICZ Integration

## UPDATE : Does not work anymore since domoticz 2020.1 moved to Lua 5.3, compatibility with lua-sockets is currently broken.

### Hardware configuration
1. **Create 5x virtual switches** :
    * Power button (press)
    * Heater button (bress)
    * Output states : Water pump, heater, bubbles
2. **Create 1x sensor**
    * Temperature sensor

### Event Lua script configuration

* Install lua-sockets [(how-to)](https://www.domoticz.com/wiki/Smappee#Installing_Lua_Libraries)

* Implement the Lua script below to your eventSystem (time based) :
```lua
local http = require("socket.http");
local json = require("JSON");
commandArray = {}

body,c,l,h = http.request("http://<YOUR ESP IP>/status");

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

return commandArray
```

* Adapt IDXs to your domoticz setup in ```update(IDX,nValue,sValue)``` calls.
* Set your ESP8266 IP  to ```http.request("http://<YOUR ESP IP>/status")```

### Lua dependancies
JSON and luaSocket are not available into Domoticz by default (https://install.domoticz.com/ on raspbian). 

1. Download packages [here](https://www.domoticz.com/forum/viewtopic.php?f=21&t=7279)
2. Enter the following commands :
```bash 
sudo mkdir -p /usr/local/share/lua/5.2/ #Only needed if this directory does not exist
cd /usr/local/share/lua/5.2/
sudo tar -xvf ~/temp/usrlocalsharelua52.tar.gz
sudo mkdir -p /usr/local/lib/lua/5.2/ #Only needed if this directory does not exist
cd /usr/local/lib/lua/5.2/
sudo tar -xvf ~/temp/usrlocalliblua52.tar.gz
```

[Source](https://www.domoticz.com/wiki/Remote_Control_of_Domoticz_by_Telegram_Bot)