#将目录下的源代码文件生成静态库
add_library(sctp_lib recvmsg.c sendmsg.c)