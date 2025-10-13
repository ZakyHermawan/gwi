# gwi

Graphical User Interface Application

This project uses the [WiringPi](https://github.com/WiringPi/WiringPi.git) library to access the GPIO pins of the Raspberry Pi Zero 2 W.  
The [installation script](https://github.com/WiringPi/WiringPi?tab=readme-ov-file#from-source) provided by the official repository can be used to build the library from source, as shown below.

During the development of this project, WiringPi version **3.16** was built from source on an ARM-based Debian Bookworm system.

```sh
sudo apt install git
git clone https://github.com/WiringPi/WiringPi.git
cd WiringPi

./build debian
mv debian-template/wiringpi-3.16.deb .

sudo apt install ./wiringpi-3.16.deb
```
