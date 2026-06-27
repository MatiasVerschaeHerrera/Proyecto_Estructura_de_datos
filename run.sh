#!/bin/bash
gcc main.c historia.c tdas/*.c -o program
if [ $? -ne 0 ]; then
  echo "Error de compilacion."
  exit 1
fi
echo "STOP" > audio/audio_command.txt
echo "=== Corre el servidor en otro Shell: ==="
echo "=== python3 -m http.server 3000 --bind 0.0.0.0 --directory audio ==="
./program