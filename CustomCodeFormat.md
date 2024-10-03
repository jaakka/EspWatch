
<< [Return to readme](./README.md)
# Custom language rules / format


### Hardcoded commands
| Command  | Arg0  | Arg1  | Arg2  | Arg3  | Arg4  | Arg5  | Desc  |
| ------------- | ------------- | ------------- | ------------- | ------------- | ------------- | ------------- | ------------- |
| io  | write | IO_PIN | 0 - 255 |  |  |  | Update io output |
| io  | read | IO_PIN |  |  |  |  | Returns io value to 0-1023 |
| lcd  | draw | box | X | Y | WIDTH | HEIGHT | Draws box |
| lcd  | draw | line | X | Y | XX | YY | Draws line |
| lcd  | draw | text | X | Y | XX | YY | Draws line |
| lcd  | set | color | R | G | B |  | Sets draw color |
| shutdown  |  |  |  |  |  |  | Power off |
| delay  | time |  |  |  |  |  | Wait |
| run  | script name |  |  |  |  |  | run script |
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

### IO control

### Write
```
io write LCDLED 255; //Sets lcd backlight to bright.
```

### Read
```
FLOAT $variable$ = io read BATTERY; //Get battery voltage. 
```
<br />
<br />

### LCD control

### Set color
```
lcd set color 255 255 255; //Set draw color to white
```

### Draw box
```
lcd draw box 32 32 0 0; //Draw box to top left corner.
```

### Draw line
```
lcd draw line 0 0 32 0 //Draw 32px long vertical line
```

### Draw text
```
lcd draw text "text" 100 100 3; //Draw text with "3" size
```

### Draw text with variables
```
lcd draw text %variable% %x% %y% 3; //Draw text with "3" size
```
note: %x% & %y% need be float or integer
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

## How declare variables / How create new variable

#### Without values
|  Type |  Variable | 
| ------------- | ------------- |
| INT | %variable% | 
| FLOAT | %variable% | 
| STR | %variable% | 
| BOOL | %variable% | 

#### With values
|  Type |  Variable | Value
| ------------- | ------------- | ------------- |
| INT | %variable% | 32
| FLOAT | %variable% | 32.2
| STR | %variable% | "sdasf"
| BOOL | %variable% | true/false

## Array usage

#### Without values
| |  Indexes |  Type | Variable 
| ------------- | ------------- | ------------- | ------------- |
| ARRAY | 3 | INT | %variable% 
| ARRAY | 3 | FLOAT | %variable% 
| ARRAY | 3 | STR | %variable% 
| ARRAY | 3 | BOOL | %variable% 

#### With values
| |  Indexes |  Type | Variable | value0 | value1 | value2
| ------------- | ------------- | ------------- | ------------- | ------------- | ------------- | ------------- |
| ARRAY | 3 | INT | %variable% | 32 | 43 | 53
| ARRAY | 3 | FLOAT | %variable% | 32.2 | 43.54 | 53.65
| ARRAY | 3 | STR | %variable% | "sdasf" | "test" | "lol"
| ARRAY | 3 | BOOL | %variable% | false | true | false

<br />

## Example scripts

<br />

### Battery level 0-100%
```
FLOAT $max_voltage$ = 4.7;
INT $io_value$ = io read BATTERY; 
FLOAT $current_voltage$ = $io_value$ * ( 5.0 / 1023.0 );
INT %battery_level% = ( $current_voltage$ / $max_voltage$ ) * 100;
```
<br />

### Lcd brightness control
```
FLOAT $max_voltage$ = 3;
INT $io_value$ = io read BRIGHTNESS;
FLOAT $current_voltage$ = $io_value$ * ( 5.0 / 1023.0 );
INT $brightness$ = ( $current_voltage$ / $max_voltage$ ) * ( 255 - ( $brightness2$ * 255 ) );
io write LCDLED $brightness$;
```