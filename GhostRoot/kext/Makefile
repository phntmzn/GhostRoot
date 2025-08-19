


KEXT_NAME = rootkit
KEXT_DIR = $(KEXT_NAME).kext
SRC = ../dkom/hide_process.c
OBJ = $(SRC:.c=.o)

SDKROOT = /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.9.sdk
ARCHS = x86_64
CFLAGS = -Wall -Wextra -O2 -mkernel -fno-builtin -isysroot $(SDKROOT) -arch $(ARCHS) -I$(SDKROOT)/System/Library/Frameworks/Kernel.framework/Headers
LDFLAGS = -static -nostdlib

all: $(KEXT_DIR)/Contents/MacOS/$(KEXT_NAME)

$(KEXT_DIR)/Contents/MacOS/$(KEXT_NAME): $(OBJ)
	mkdir -p $(KEXT_DIR)/Contents/MacOS
	ld $(LDFLAGS) -o $@ $^

%.o: %.c
	clang $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(KEXT_DIR) *.o

.PHONY: all clean