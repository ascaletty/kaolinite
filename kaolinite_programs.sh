appsraw=($(ls -d /usr/share/applications/*.desktop))
size=${#appsraw[@]}
for ((i = 0; i < $size; i = i + 1)); do
  echo ${appsraw[$i]} | rev | awk -F. '{print $2}'
  echo $a
done
