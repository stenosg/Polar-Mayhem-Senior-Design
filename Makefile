LIBS = OGRE OIS CEGUI-OGRE CEGUI
CXX = g++
CPPFLAGS = $(shell pkg-config --cflags $(LIBS)) -g -O2 -Os -I/usr/include/AL
LDFLAGS = $(shell pkg-config --libs $(LIBS)) -lopenal -lalut #-ltinyxml
OTHERS = 
DIR = -I/home/projects/rgl/cs134-win08/include/

## Files to include in yourApp project
OGREAPP_OBJ = stats.o item.o unit.o tile.o map.o player.o aiplayer.o textrenderer.o soundmanager.o fishinghole.o gamemanager.o main.o
OGREAPP_SRC = stats.cpp item.cpp unit.cpp tile.cpp map.cpp player.cpp aiplayer.cpp textrenderer.cpp soundmanager.cpp fishinghole.cpp gamemanager.cpp main.cpp

## Compilation and linking occurs here
all: $(OGREAPP_OBJ)
	$(CXX) $(LDFLAGS) $(CPPFLAGS) $(OGREAPP_OBJ) $(OTHERS) $(DIR) -o ogreapp

%.o: %.cpp
	$(CXX) $(CPPFLAGS) -c $<

## Clean up
clean:
	rm -f *.o ogreapp

# detects and appends dependency rules to this Makefile
depend:
	makedepend -- $(OGREAPP_SRC) 

