#!/bin/bash

if [[ "$(whoami)" != "root" ]]; then
    echo "Please run this as root."
    exit 1
fi

tmpfile="$(mktemp)"
wget "https://raw.githubusercontent.com/todbot/blink1/master/linux/51-blink1.rules" -O "$tmpfile"
if [ $? -eq 0 ]; then
    echo "Successfully downloaded file. Moving to /etc/udev/rules.d/51-blink1.rules"
    mv "$tmpfile" /etc/udev/rules.d/51-blink1.rules
    echo "Reloading udev"
    udevadm control --reload
else
    echo "Failed to download rules files (check that https://raw.githubusercontent.com/todbot/blink1/master/linux/51-blink1.rules is still around)"
    rm $tmpfile
fi
