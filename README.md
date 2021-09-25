# raspi-pico-grove-starter-kit
C++ codes for [***Grove Starter Kit Raspberry Pi Pico***](https://www.seeedstudio.com/Grove-Starter-Kit-for-Raspberry-Pi-Pico-p-4851.html).

## Grove Sensors
- Grove LCD 16x2
- Grove Temperature and Humidity (DHT-11)

## Source code
- src
    - Source codes to utilize grove sensors.
    - Generate `libgrove=sensors.a` statis library file.
- examples  
    Sample codes for each sensors.
    - lcd  
      Output string to LCD 16x2.
    - temperature_humidity  
      Get temperature and humidity from DHT-11 and output the result to LCD. 
    - pico_led  
      The code is not for grove sensors. Just use LED on pico.

## How to build
```
$ git clone (TBD)
$ cd (TBD)
$ mkdir build
$ cd build
$ cmake ..
$ make
```

## Run
Copy executable `*.uf2` file which you want to use on pico from `build/example/` to pico.
