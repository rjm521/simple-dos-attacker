
  echo "╔╦╗╔═╗╔═╗╔═╗╦═╗"
  echo " ║║║ ║╚═╗║╣ ╠╦╝"
  echo "╚╩╝╚═╝╚═╝╚═╝╩╚═ v1.0 builder"
  echo "Created by Jimmy (NJUPT IOT) 11.28.2019"
  gcc  util.c message.c socket.c dos.c main.c -lm -Ofast -Os -lpthread -o dos
