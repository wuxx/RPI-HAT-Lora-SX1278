CFLAGS = $(cflags)

#$(warning CFLAGS: $(CFLAGS))

ROOT = .

CC = gcc
AS = as
#LD = ld
LD = gcc

XSERVER_REV = $(shell git rev-parse --short HEAD)

BUILD_DIR = $(ROOT)/out

LOCAL_ID = $(shell cat ~/.ssh/id_rsa.pub)

#-I/opt/vc/include/ -I/opt/vc/include/interface/vcos/pthreads -I/opt/vc/include/interface/vmcs_host/linux -I/opt/vc/include -Ilocallib/include

INCLUDE_DIR = .

ALL_LIBS = 

ALL_SRCS = \
        $(ROOT)/main.c   \
        $(ROOT)/common.c   \
        $(ROOT)/serial.c \
        $(ROOT)/sx1278.c \
        $(ROOT)/demo_echo_client.c \
        $(ROOT)/demo_echo_server.c

C_SRCS   = $(filter %.c, $(ALL_SRCS))
H_SRCS   = $(wildcard $(INCLUDE_DIR)/*.h)

C_OBJS   = $(addprefix $(BUILD_DIR)/, $(patsubst %.c,%.o,$(C_SRCS)))

ALL_OBJS = $(C_OBJS)

#$(warning ALL_SRCS $(ALL_SRCS))
#$(warning ALL_OBJS $(ALL_OBJS))

OBJ_PATHS = $(addprefix $(BUILD_DIR)/, $(sort $(dir $(ALL_SRCS))))
#$(warning OBJ_PATHS $(OBJ_PATHS))

TARGET = main
TARGET_ELF = $(BUILD_DIR)/$(TARGET)

CFLAGS += -g -Wall -O0 -I$(INCLUDE_DIR)

LDFLAGS = $(ALL_LIBS)

.PHONY: build_all clean tags

build_all: all

$(C_OBJS): $(BUILD_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

build_objs: $(C_OBJS)

init:
	@mkdir -p out
	@$(foreach d,$(OBJ_PATHS), mkdir -p $(d);)

all:init build_objs
	$(LD) $(ALL_OBJS) $(LDFLAGS) -o $(TARGET_ELF)


tags:
	ctags -R .


run:
	./out/main

cloc: 
	cloc --exclude-dir=locallib .

clean: 
	-rm -rf out
