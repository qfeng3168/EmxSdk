#!/bin/sh

if [ $# != 1 ];then
    echo 'Please enter the control parameter 1(open) or 0(close)'
    exit
fi
cvitekSerial() {
    echo "ctrl cvitek serial port $1"
    if [ $1 != 0 ];then
        devmem 0x03001040 32 0x0
        devmem 0x03001044 32 0x0
    else
        devmem 0x03001040 32 0x3
        devmem 0x03001044 32 0x3
    fi
}
sigmastarSerial() {
    echo "ctrl sigmastar serial port $1"
    if [ $1 != 0 ];then
        devmem 0x1F203D4C 32 0x3012
    else
        devmem 0x1F203D4C 32 0x3010
    fi
}

if [ $1 != 0 ];then
  echo 'Open the serial port'
else
  echo 'Close the serial port'
fi

if [ -d "/proc/cvitek" ]; then
    cvitekSerial $1    
elif [ -d "/proc/mi_modules" ]; then
    sigmastarSerial $1
fi
