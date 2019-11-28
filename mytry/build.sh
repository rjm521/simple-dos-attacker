# 编译脚本
  echo "╔╦╗╔═╗╔═╗╔═╗╦═╗"
  echo " ║║║ ║╚═╗║╣ ╠╦╝"
  echo "╚╩╝╚═╝╚═╝╚═╝╩╚═ v1.0 builder"
  echo "Created by Jimmy (NJUPT IOT) 11.24.2019"
  gcc  util.c message.c socket.c dos.c main.c -lm -Ofast -Os -lpthread -o dos

####### 文件说明 ############
#   util.c          一些工具函数的实现文件
#   message.c       用来在终端显示程序运行信息的函数的实现文件
#   socket.c        用来建立 TCP 连接等一些函数的实现文件
#   dos.c           用来开启多线程进行攻击的实现文件
#   main.c          程序入口文件
##############################

####### 编译参数 #############
#   -lm               link libm 链接数学库
#   -Ofast            优化的最高标准
#   -Os               对代码的大小进行优化，使得所生成代码长度最小化
#   -lpthread                 因为用到了多线程编程
#   -o dos            指定将编译好的可执行二进制文件命名为 dos
##############################