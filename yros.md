Partitions
----------
All read-write during development.

```
/boot RO  50M                        FAT32
/     RO  50M or whatever is needed  EXT3
/data RW  5M                         FAT16
```

Mounts
------
```
/, /boot - root:root
/data    - yros:yros
```

Boot process
------------
1. Linux kernel loaded
2. BusyBox launches scripts in `/etc/init.d`
     1. `sshd` if development
     3. `pigpiod` start
     2. `wpa_supplicant` start
     3. `yrosd` start as `yros` user
3. Auto login `yros` (IF GUI)
4. `startx` if not already, `.xinitrc` executes `yros_disp` (IF GUI)

`yrosd`
=======
The whole protocol uses CRLF line endings. It also uses ASCII for every
character unless specified otherwise.

All sockets are of type `AF_INET` unless specified otherwise.

The protocol is split into multiple steps, called MODES. When the service
starts, it _must_ begin in INITIALIZATION MODE.

LED blinking should have 500ms ON and OFF. After count is finished, another
500ms must be waited before blinking again. Reference C code:

```c
void blink(int count) {
  int i;
  while (true) {
    for (i=0; i<count; i++) {
      LedOn();
      Sleep(500);
      LedOff();
      Sleep(500);
    }
    Sleep(500);
  }
}
```

INITIALIZATION MODE
-------------------
1. Checks if already running, quits appropriately
2. Detach unless -D (don't detach)
3. Wait for `pigpiod` to start
4. Parse `/data/syssettings.toml`. If invalid, blink LED 1 time indefinitely.
5. If `/data/usersettings.toml` exists:
     1. Parse user settings using tomlc99, if any error occurs, delete
        `/data/usersettings.toml` and go to 5
   else:
     1. Enter SETUP MODE
     2. Write file and go to 4
6. Start sending broadcast messages for discovery. See the DISCOVERY section
   for more details. This should run indefinitely.
7. Await SSL TCP connections on all if (0.0.0.0). Port must be randomized,
   this is a requirement for broadcasts.
8. For each new connection, enter AUTH MODE and pause sending broadcast
   messages.
9. Go to 6 unless shutting down

SETUP MODE
----------
1. Create an AF_BLUETOOTH socket and start listening for a single connection.
   If connection is dropped when configuration is incomplete, delete current
   temporary configuration from memory then re-listen again for a new
   connection.
2. Send a magic header: `YROSsetup<YROS version>\r\n`
3. Await for client to send data (`usersettings.toml` contents). If data is
   invalid, send response `SETUP_ERR_INVALID`.
4. Await for client to send `SETUP_CLIENT_OK`. Handle cases otherwise.
5. Write data to `/data/usersettings.toml` and return to INITIALIZATION MODE
   appropriately.

AUTH MODE
---------
Auth mode is the mode where the basic authentication set up in the SETUP
MODE part is validated from the client. 

1. Send the magic header: `YROSauth<YROS version>\r\n`
2. Wait for the client to send a password. If the password is wrong, send
`AUTH_PASS_FAIL` and go to 2
3. Send `AUTH_PASS_OK`
4. Enter COMMAND MODE

COMMAND MODE
------------
Command mode is the main mode of any YROS system. This is where all the
interesting stuff happens. 

Each input has the following format:

```
<command|alias> [arguments...]
```

Commands and aliases are NOT case-sensitive.

If a command does not have enough arguments, send a `COMMAND_ERR_ARGC`.

`COMMAND_ERR_INV_ARG` must be sent whenever an argument is not found or is
invalid.

Commands
--------

- `INFO` - Request information

    This command reports what features are enabled for the robot, as well as
    some other information such as network information.

    View the INFORMATION for the response data.

- `UCONF <category> <key> <value>` - Update configuration

    This commands looks for any configuration key-value pair in the specified
    category and updates its value with a new one.

    If found, update the pair correspondingly and write the file. After that
    is done, send `COMMAND_OK`. If not, send a `COMMAND_ERR_NOT_FOUND`.

- `SYS <system request>` - Send a system request.

    System requests: shutdown, reboot.

- `MOTOR <motor id> <action> [arguments...]` - Motor request \[Aliases: `M`]

    If the specified motor is not found, send `COMMAND_ERR_NOT_FOUND`.

    Actions:
    1. `SET|S <speed>` - Set motor speed.

        This command sets the motor speed to a value specified. The value must
        be in this range and may be a floating point number: \[-1, 1].

        If any error occurs, send `COMMAND_ERR_UNK`. If not, send `COMMAND_OK`.

    2. `STOP|T` - Stop the current motor.

        This command stops the motor. 

    3. `DISABLE` - Disable the motor. This is a very dangerous command and must
        be used with great care.
    4. `ENABLE` - Enable the motor.

- `SERVO <servo id> <angle>` - Servo request \[Aliases: `S`]

    If the specified servo is not found, send `COMMAND_ERR_NOT_FOUND`.

- `GPIO <pin> <action> [arguments...]` - GPIO request.

    If the specified pin is invalid, send `COMMAND_ERR_NOT_FOUND`.

    Actions:
    1. `DIGITAL_WRITE|DW <1|0>` - Writes a digital value to the pin. It also
       sets the pin mode to OUTPUT.
    2. `ANALOGUE_WRITE|AW <0-255>` - Writes an analogue value to the pin. It
       also sets the pin mode to OUTPUT.

- `DRIVE <speed...>` - Drive command. \[Aliases: `D`]

    Depending on the _drive mode_, it sets motor speeds for the right motors.

- `VIDEO <true|false>` - Enable/disable video streaming.

    If video streaming is not configured, send `COMMAND_ERR_CFG`.

    `true`: Start an HTTPS web server on a random port with a live MJPEG feed of
    the camera. After the web server has started, send the port and HTTPS
    location of the stream (for example, `1236 /randomhashhere.mjpeg`) and
    `COMMAND_OK`.

    `false`: Stop the HTTPS server if started and send `COMMAND_OK`.

Drive modes
-----------

List, motor values are specified in order.

<!-- LTeX: enabled=false -->
Type     | Motors (in order)
=========|===============================================================
single   | MotorTopLeft
tank     | MotorTopLeft, MotorTopRight
quadtank | MotorTopLeft, MotorTopRight
mecanum  | TranslationAngle, TranslationPower, TurnPower
<!-- LTeX: enabled=true -->

`mecanum` also supports `quadtank` drive mode.

DISCOVERY
---------
The discovery packet is used to see what YROS devices are available on the
network by clients. It must contain the following:

```
YROSDS<YROS version>
```

This, however, must be sent only every 5 seconds. This is to not spam the
network too much.

It must be sent on port 9901.

INFORMATION
-----------

TOML format, may be extended.
```toml
Features=[
  'VideoStreaming',  # Video streaming available
]
Network='MyCoolSSID' # Available if connected.
RobotName='my-robot'
DrivingMode='tank'
```

`syssettings.toml`
-------------------

```toml
[Hardware]
Motors=[  # Pololu Dual G2 Motor Driver
  { fault=5, pwm=12, enabled=22, direction=24 },
  { fault=6, pwm=13, enabled=23, direction=25, inverted=true },
]

[VideoStreaming]
Enabled = true
CameraDevice="/dev/video0"

[Driving]
Mode="tank"
MotorTopLeft=0
MotorTopRight=1
```

`usersettings.toml`
-------------------

```toml
[General]
RobotName="my-robot"

[Conectivity]
WiFiNetworks=[
  { ssid="MyEpicWiFiNetwork", password="secretpassword123" },
  { ssid="MyOpenEpicWiFiNetwork" },
]
```

Responses
---------

Format: `R<code byte>\r\n`

<!-- LTeX: enabled=false -->
ERROR SYMBOL          | VALUE | Message/Description
----------------------|-------|-------------------------------------------------
SETUP_OK              |  0x00 | Data on the server has been processed correctly.
SETUP_CLIENT_OK       |  0x01 | Data on the client has been processed correctly.
SETUP_ERR_INVALID     |  0x02 | Invalid configuration provided.
AUTH_PASS_FAIL        |  0x10 | Authentication failed. Please try again.
AUTH_PASS_OK          |  0x11 | Authentication succeeded.
COMMAND_OK            |  0x20 | Command executed successfully.
COMMAND_ERR_UNK       |  0x21 | An unknown error occured.
COMMAND_ERR_ARGC      |  0x22 | Not enough arguments.
COMMAND_ERR_INV_ARG   |  0x23 | Invalid argument(s).
COMMAND_ERR_NOT_FOUND |  0x24 | Cannot find data/target specified.
COMMAND_ERR_CFG       |  0x35 | Command not configured.
<!-- LTeX: enabled=true -->

