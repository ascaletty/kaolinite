#! /bin/bash
cd /usr/share/applications/
dmenucacheraw=($(ls -d *.desktop))
size=${#dmenucacheraw[@]}
echo size
echo $size
declare -A applications
i=0
until [ $i -ge $size ]; do
  awk -F'=' '/^Name=/ { print $2}' "${dmenucacheraw[$i]}"
  awk -F'=' '/^Exec=/ { print $2 }' "${dmenucacheraw[$i]}"
  echo $value
  applications[$key]=$value
  ((i++))
done
