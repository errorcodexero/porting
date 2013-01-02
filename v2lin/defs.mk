# global options 
CC := gcc-3
CXX := g++-3
CFLAGS += -I$(top)/lib
CFLAGS += -g
CFLAGS += -D_USR_SYS_INIT_KILL
CFLAGS += -Wall -Wno-format -Wno-unused-label -Wno-unused-variable -Werror
CFLAGS += -fmessage-length=0
#CFLAGS += -fPIC # not needed on x86
#CFLAGS += -DDEBUG # use for the global debugging
