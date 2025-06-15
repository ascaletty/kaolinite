#! /bin/bash
application_home=/usr/share/applications/
if [ $(awk '/^Terminal=/{sub(/^Terminal=/, ""); print}' $application_home/$1.desktop) != 'true' ]; then
  echo "in terminal app not true"
  ($1 &)
else
  (kitty $1)
  echo "in terminalapp false"
fi
