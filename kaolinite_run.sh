#! /bin/bash
# application_home=/usr/share/applications/
# if [ $(awk -F '/' '/^Terminal=/{sub(/^Terminal=/, ""); print}' $application_home/$1.desktop) !== "true" ]; then
#   echo "not interminal app"
($1 &)
# else
#   (kitty $1 &)
#   echo "in terminalapp"
# fi
