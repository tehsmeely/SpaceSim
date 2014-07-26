CPP = g++
RM = rm -f
# Update these paths to match your installation
# You may also need to update the linker option rpath, which sets where to look for
# the SDL2 libraries at runtime to match your install
SDL_LIB = -L/usr/local/lib -Wl,-rpath=/usr/local/lib
SDL_INCLUDE = -I/usr/local/include/SDL2
# You may need to change -std=c++11 to -std=c++0x if your compiler is a bit older
CPPFLAGS = -Wall -c -std=c++11 $(SDL_INCLUDE)
LDFLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf
LDLIBS = $(SDL_LIB)
EXE = SpaceSim


SRCS=$(wildcard *.cpp)
OBJS=$(SRCS:.cpp=.o)


all: $(EXE)

$(EXE): $(OBJS)
	$(CPP) $(OBJS) $(LDLIBS) $(LDFLAGS) -o $(EXE) 


%.o : %.cpp
	$(CPP) $(CPPFLAGS) $< -o $@
	

clean:
	$(RM) $(OBJS)



