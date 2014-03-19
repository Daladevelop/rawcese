CC      =   g++
DEBUG   =   -g -Wall
CFLAGS  =   `sdl-config --cflags`
LIBS    =   -lSDL_image -lSDL_ttf
COMPILE =   $(CC) $(DEBUG) $(CFLAGS) -c
EXEC    =   rawcese
OBJS    :=  $(patsubst %.cpp,%.o,$(wildcard *.cpp))

link: $(OBJS)
	$(CC) $(OBJS) $(LIBS) -o $(EXEC)

%.o: %.cpp
	$(COMPILE) $< -o $@

clean:
	rm $(OBJS)
