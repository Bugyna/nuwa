
LIBS = raylib/lib/libraylib.a
CFLAGS=-Wno-format -std=c99 -fpermissive
CLIBS = -lraylib -lGL -lopenal -lm -lpthread -ldl -lX11 -lXrandr -lXinerama -lXi -lXxf86vm -lXcursor

main: *.c *.h
	gcc main.c -o main $(CFLAGS) -Iraylib/include -L. -Lraylib/lib/libraylib.a $(CLIBS) -g