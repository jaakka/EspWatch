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
| HALLPWR  | Hall sensors power | Digital |
| GYROPWR  | Gyroscope power | Digital |
| HEARTRATEPWR  | Heartrate sensor power | Digital |

<br />

IO constants (inputs)
| Constant  | Description  | Type  |
| ------------- | ------------- | ------------- |
| BRIGHTNESS | Light sensor voltage output  | Analog |
| BATTERY | Battery voltage  | Analog |
| HALL1 | Hall sensor output  | Analog |
| HALL2 | Hall sensor output  | Analog |

<br />

# Example codes

<br />

IO Write
```
io write LCDLED 255 //Sets lcd backlight to bright.
```
<br />

IO Read
```
variable = io read BATTERY //Get battery voltage. 
```