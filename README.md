# EspWatch with third party applications

<br />

Dear developers.. please write helpful commit messages.

Hardware folder is for esp code.

Simulation folder is for custom code language testing.

ThirdpartyApps folder is for custom apps

<br />

# Custom language rules / format

<br />

Hardcoded commands
| Command  | Argument0  | Argument1  | Argument2  | Description  |
| ------------- | ------------- | ------------- | ------------- | ------------- |
| io  | write | IO_PIN | 0 - 255 | Update io output |
| io  | read | IO_PIN | Returns io value to 0-1023 |
<br />

IO constants (outputs)
| Constant  | Description  | Type  |
| ------------- | ------------- | ------------- |
| DEBLED  | Debug led | Analog |
| LCDLED  | Lcd brightness | Analog |

<br />

IO constants (inputs)
| Constant  | Description  | Type  |
| ------------- | ------------- | ------------- |
| BRIGHTNESS | Light sensor voltage output  | Analog |
| BATTERY | Battery voltage  | Analog |

<br />

# Example codes

<br />
Io write 
``
io write LCDLED 255 //Sets lcd backlight to bright.
``
<br />
Io read
``
variable = io read BATTERY //Get battery voltage.
``