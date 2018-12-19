DESTDIR ?= .
CC = gcc
CFLAGS ?= -fPIC -Wall -std=c99
INCLUDES ?= -I/usr/include/curl
LDFLAGS ?= -lcurl
TARGET ?= bt-pam-module
PAM_DST ?= /lib/security/pam-bt.so

all: $(TARGET)

# let's let gcc do all the work (compiling + linking)
$(TARGET):
	$(CC) -shared -o $@.so $(LDFLAGS) $(LIBS) $(CFLAGS) $(INCLUDES) $(TARGET).c

install:
	install -m 0755 $(TARGET).so $(PAM_DST)

	
