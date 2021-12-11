# Drip protocol specification

## Overview

The Drip protocol exchanges line-based text records between client and server over UDP.

All data is encoded in UTF-8.

Newlines are determined by a `\n`

When the client connects to the server, the server will answer with informations about the
protocol:

```
OK DRIP version
```

where `version` is a version identifier (for example, `1.0.0`). This version identifier is
the version of the protocol spoken and not the version of the server itself.

## Requests

Requests are usually one byte long, each byte representing a command.
This is to remove any potential latency, especially on _very_ slow 
internet speeds.

Command parameters are delimited by a `:`, `,` or `<whitespace>`.

Here are some examples:
```
Client: hl o
Client: s 1 180
Client: ml:200
Client: Hs,180 50
```

## Responses

A command returns a `OK` or `ERR error_code` on failure. These denote 
the end of command execution.

Some commands return more data before the response ends with `OK`. Each
line is usually in the form of a `KEY: VALUE` pair.

Example:
```
Client: S
Server: MotorFault: false
Server: MotorSpeeds: 0,0
Server: Servos: 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
Server: ProcessorUsage: 10.2,14.6,9.1,9.6,12.9,5.6,9.1,6.8,8.0,6.2,4.5,3.4
Server: DiskUsage: 83.3
Server: MemoryUsage: 56.7
Server: BytesReceived: 1882364246
Server: BytesSent: 153183834
Server: ProcessorTemperature: 16.8
Server: OK
```

## Command reference

 * **S** - Gives a full report of motors, servos and system information.
   * **MotorFault** - `true` if any motor faults are detected.
   * **MotorSpeeds** - The speeds of the motors.
   * **Servos** - The angle of all 16 servos in degrees.
   * **ProcessorUsage** - The usage percentage for each processor core.
   * **DiskUsage** - The usage percentage of the `/` partition.
   * **MemoryUsage** - The usage percentage of the RAM.
   * **BytesReceived** - The amount of bytes received system-wide.
   * **BytesSent** - The amount of bytes sent system-wide.
   * **ProcessorTemperature** - The temperature of the processor.

 * **m\<l/r\> \<speed\>** - Sets the speed of the (l)eft or (r)ight
   motor.

 * **s\<index\> \<angle\>** - Sets a servo's angle in degrees.

 * **h\<l/r\> \<o/c\>** <sup>\[[1](#since-v1.0)\]</sup> - (O)pens or (c)loses the (l)eft or (r)ight
   hand on the robot. (if configured)

 * **H<s/t/T>** <sup>\[[1](#since-v1.0)\]</sup> - Head
   * **s \<yaw\> \<pitch\>** - Set head Yaw and Pitch in degrees (if 
     configured)
   * **t** - Turn face tracking on (if configured)
   * **T** - Turn face tracking off (if configured)

 * **R** - Reboot server

 * **p** <sup>\[[2](#since-v1.1)\]</sup> - Power
   * **o** - Off
   * **r** - Reboot

 * **E** <sup>\[[2](#since-v1.2)\]</sup> - Emotion
   * **1** - Angry
   * **2** - Happy
   * **3** - In love
   * **4** - Sad
   * **5** - Surprised

## Error codes

 * **1** - Not enough arguments
 * **2** - Invalid argument(s)
 * **3** - Failed to set motor speed(s)
 * **4** - Failed to set servo angle(s)
 * **5** - Unknown
 * **6** - No command specified
 * **7** - Not configured
 * **8** - Invalid command

### Footnotes

<a name="since-v1.0">1</a>: Since DRIP 1.0
<a name="since-v1.1">2</a>: Since DRIP 1.1
<a name="since-v1.2">3</a>: Since DRIP 1.1
