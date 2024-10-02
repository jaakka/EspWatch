
<< [Return to readme](./README.md)
# Custom language rules / format


### Hardcoded commands
| Command  | Argument0  | Argument1  | Argument2  | Description  |
| ------------- | ------------- | ------------- | ------------- | ------------- |
| io  | write | IO_PIN | 0 - 255 | Update io output |
| io  | read | IO_PIN | Returns io value to 0-1023 |

<br />

### IO constants (outputs)
| Constant  | Description  | Type  |
| ------------- | ------------- | ------------- |
| DEBLED  | Debug led | Analog |
| LCDLED  | Lcd brightness | Analog |
| HALLPWR  | Hall sensors power | Digital |
| GYROPWR  | Gyroscope power | Digital |
| HEARTRATEPWR  | Heartrate sensor power | Digital |

<br />

### IO constants (inputs)
| Constant  | Description  | Type  |
| ------------- | ------------- | ------------- |
| BRIGHTNESS | Light sensor voltage output  | Analog |
| BATTERY | Battery voltage  | Analog |
| HALL1 | Hall sensor output  | Analog |
| HALL2 | Hall sensor output  | Analog |

<br />

# Example codes

<br />

### IO Write
```
io write LCDLED 255; //Sets lcd backlight to bright.
```
<br />

### IO Read
```
$variable$ = io read BATTERY; //Get battery voltage. 
```
<br />

## Global and local variables
| Format  | Type  |
| ------------- | ------------- | 
| `$variable$`  | Local variable| 
| `%variable%`  | Global variable| 
| `$test$`  | Local variable| 
| `%test%`  | Global variable| 

Local is only for one script.
All scripts can read/write global variables.

<br />

## Example scripts

<br />

### Battery level 0-100%
```
$max_voltage$ = 4.7;

$io_value$ = io read BATTERY; 

$current_voltage$ = $io_value$ * (5.0 / 1023.0);

%battery_level% = ($current_voltage$ / $max_voltage$) * 100;
```
<br />

### Lcd brightness control
```
$max_voltage$ = 3;

$io_value$ = io read BRIGHTNESS;

$current_voltage$ = $io_value$ * (5.0 / 1023.0);

$brightness$ = ($current_voltage$ / $max_voltage$) * 255;

$lcdbrightness$ = 255 - $brightness$;

io write LCDLED $lcdbrightness$;
```