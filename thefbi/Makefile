include dependency_paths

CFLAGS = -O2 -Werror-implicit-function-declaration -DLINUX_PLUGIN -DxTINYDEBUG -DMOZ_X11 -DOUTPUT_X -fPIC -pthread

all: libthefbiplugin.so ThefbiTestExe thefbi_sandbox_host

clean: 
	rm *.c.o

libthefbiplugin.so: tinydebug.c.o nspluginbase.c.o vxplugin.c.o imagetowindow.c.o pluginevents.c.o  lzma/LzmaDec.c.o
	gcc -fPIC -shared -Wl,-soname,libthefbiplugin.so -static -o libthefbiplugin.so nspluginbase.c.o vxplugin.c.o tinydebug.c.o imagetowindow.c.o pluginevents.c.o lzma/LzmaDec.c.o -lX11 -lXt -lXext 

thefbi_sandbox_host: thefbi_sandbox_host.c.o thefbivxthread.c.o  tinydebug.c.o
	gcc $(CFLAGS) -o thefbi_sandbox_host  thefbi_sandbox_host.c.o thefbivxthread.c.o tinydebug.c.o  -lX11 -lXt -L $(LIBVX32) -lvx32  
	
thefbi_sandbox_host.c.o: thefbi_sandbox_host.c
	gcc $(CFLAGS) -c -o thefbi_sandbox_host.c.o thefbi_sandbox_host.c $(GECKO_INCLUDES) -I $(LIBVX32)
	
tinydebug.c.o: tinydebug.c
	gcc $(CFLAGS) -c -o tinydebug.c.o tinydebug.c

nspluginbase.c.o: nspluginbase.c
	gcc $(CFLAGS) -c -o nspluginbase.c.o nspluginbase.c $(GECKO_INCLUDES)

vxplugin.c.o: vxplugin.c
	gcc $(CFLAGS) -c -o vxplugin.c.o vxplugin.c  $(GECKO_INCLUDES) -I $(LIBVX32)

imagetowindow.c.o: imagetowindow.c
	gcc $(CFLAGS) -c -o imagetowindow.c.o imagetowindow.c 

pluginevents.c.o: pluginevents.c 
	gcc $(CFLAGS) -c -o pluginevents.c.o pluginevents.c $(GECKO_INCLUDES)

thefbivxthread.c.o: thefbivxthread.c
	gcc $(CFLAGS) -c -o thefbivxthread.c.o thefbivxthread.c $(GECKO_INCLUDES) -I $(LIBVX32)

lzma/LzmaDec.c.o: lzma/LzmaDec.c
	gcc $(CFLAGS) -DLZMA_Bool -c -o lzma/LzmaDec.c.o lzma/LzmaDec.c

ThefbiTestExe: tests/tiny.asm.o
	gcc -Wall -s -nostdlib tests/tiny.asm.o -o ThefbiTestExe
	
tests/tiny.asm.o: tests/tiny.asm
	nasm -f elf tests/tiny.asm -o tests/tiny.asm.o

