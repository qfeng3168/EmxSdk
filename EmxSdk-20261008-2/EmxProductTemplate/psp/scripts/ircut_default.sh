#!/bin/ash
echo "ircut_default"

GPIO_PIN_WIRE0="375"

check_and_export_gpio() {
    local pin=$1
    if [ ! -e "/sys/class/gpio/gpio$pin" ]; then
        echo "$pin" > "/sys/class/gpio/export"
    fi
}

set_direction_and_value() {
    local pin=$1
    local value=$2
    check_and_export_gpio "$pin"

    echo "out" > "/sys/class/gpio/gpio$pin/direction"
    echo "$value" > "/sys/class/gpio/gpio$pin/value"
}

check_and_export_gpio "$GPIO_PIN_WIRE0"

set_direction_and_value "$GPIO_PIN_WIRE0" "1"
sleep 0.2
set_direction_and_value "$GPIO_PIN_WIRE0" "0"