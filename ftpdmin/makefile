
CC=cl /nologo
CFLAGS=-c -W3 -Ox /MT /wd "4996" /MP
LINKER=link
LINKCON = /nologo /NODEFAULTLIB:libc

TARGET=Bin\ftpdmin.exe
OBJS = ftpdmin.obj paths.obj

all: $(TARGET)

$(OBJS): $(@B).c paths.h
    $(CC) $(CFLAGS) $(@B).c

$(TARGET): $(OBJS)
    $(LINKER) $(LINKCON) -OUT:Bin/ftpdmin.exe $(OBJS) ws2_32.lib 
    
clean:
    del /a /q $(TARGET) $(OBJS) 2>nul

.PHONY: clean all
rebuild: clean all
