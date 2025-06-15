#! /bin/bash
cd /usr/share/applications/
dmenucacheraw=($(ls -d *.desktop))
size=${#dmenucacheraw[@]}
echo size
echo $size
declare -A applications
i=0
until [ $i -ge $size ]; do
  # awk -F'=' '/^Name=/ { print $2; exit}' "${dmenucacheraw[$i]}"
  # awk -F'=' '/^Exec=/ { print $2; exit }' "${dmenucacheraw[$i]}"
  applications[$(awk -F'=' '/^Name=/ { print $2; exit}' "${dmenucacheraw[$i]}")]=$(awk -F'=' '/^Exec=/ { print $2; exit }' "${dmenucacheraw[$i]}" | cut -d " " -f1)
  ((i++))
done
for key in "${!applications[@]}"; do
  printf "$key,${applications[$key]}," >>$HOME/.cache/kaolinitedrun.cache
done
