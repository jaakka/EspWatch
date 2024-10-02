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
| ------------- | ------------- |
| deb_led  | Debug led | Analog |
| lcd_led  | Lcd brightness | Analog |

<br />

IO constants (inputs)
| Constant  | Description  |
| ------------- | ------------- |
| brightness | Light sensor output  |
