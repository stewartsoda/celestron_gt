**If you don't wire up the port right, you'll fry something! Don't follow the steps below!**

# Design Notes
These are some notes relating to the design of this project.

## Purpose
The purpose of this project is to replace the mount electronics of an early Celestron Nexstar GT telescope mount. The prototype will be developed for an early (circa 2002) Celestron Nexstar 144GT mount, though other Nexstar mounts function similarly.

## Physical Dimensions
The project board will ideally fit the dimensions of Celestron MC boards, as it is intended to completely replace the MC board.
<img src="/docs/images/board_in_situ.JPEG" alt="Picture of a Nexstar 144GT MC board and associated motors and encoders." width="768" height="1024">

## Inputs and Outputs
Here are some of the considerations of inputs and outputs.

```mermaid
%%{init: {"flowchart": {"defaultRenderer": "elk"}} }%%
graph 
    direction TB

    %% Power Connections
    extpow-- 12V -->pc
    pc-- 12V -->powcon
    powcon-- 3.3V -->cpu
    powcon-- 3.3V -->ioshi
    powcon-- 5V -->ioshi
    powcon-- 12V -->motcon

    %% I/O Connections
    aux<==5V I/O==>ioshi
    ioshi<==3.3V CPU I/O==>cpu
    cpu--3.3V for encoder LED-->azienc
    cpu--3.3V for encoder LED-->altenc
    azienc--5V from encoder sensor-->ioshi
    altenc--5V from encoder sensor-->ioshi
    motcon--12V PWM-->azimot
    motcon--12V PWM-->altmot
    usb<==USB-C extension cable==>cpu
    extcon<==Native USB==>usb
    extcon<==USB-to-Serial==>usb
    extcon<==Celestron Protocol==>aux

    extpow[12V DC]
    extcon[External Control]
    subgraph mount[Telescope Mount]
        direction TB
        subgraph control[Alt-Az Control]
            direction TB
            subgraph altcon[Altitude Control]
                direction TB
                altenc[Alt Enc]
                altmot[Alt Motor]
            end
            subgraph azicon[Azimuth Control]
                direction TB
                azienc[Azi Enc]
                azimot[Azi Motor]
            end
        end
        subgraph pb[Processor Board]
            direction TB
            cpu[Processor]
            powcon[Power Control]
            motcon[Motor H-Bridge]
            ioshi[I/O Level Shifter]
        end
        subgraph cb[Connector Board]
            aux[AUX Port]
        end
        pc[Power Jack]
        usb[USB-C Connector]
    end
style pb stroke-width:8px
style control stroke-width:4px,stroke-dasharray: 5 5
```


