appsraw=($(ls -d /usr/share/applications/*.desktop))
size=${#appsraw[@]}
for ((i = 0; i < $size; i = i + 1)); do
  echo ${appsraw[$i]} | awk -F/ '{print $5}'
  echo $a
done
