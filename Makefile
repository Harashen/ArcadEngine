# C/C++ compiler
CC		= gcc
CXX		= g++

# Directories
RES 		= ArcadEngine_private.res
INCDIRS		= . /mingw/include/freetype2
LIBDIRS		=

# Includes/Libraries
INCLUDE		= $(foreach dir, $(INCDIRS), -I$(dir))
LIBS		= $(foreach dir, $(LIBDIRS), -L$(dir))

# C/C++ flags
CFLAGS		= -O3 -Wall -Wextra -std=c99 $(INCLUDE)
CXXFLAGS	= -O3 -Wall -Wextra -std=c++17 $(INCLUDE)

# Linker flags
LDFLAGS		= $(LIBS) -lftgl -lpng -lopengl32 -lglu32 -lmingw32 -lSDLmain -lSDL_image -lSDL_mixer -lSDL

# Objects
OBJS	= \
	ai.o		\
	animation.o	\
	audio.o		\
	camera.o	\
	character.o	\
	collision.o	\
	log.o   \
	credits.o	\
	enemy.o		\
	engine.o	\
	entity.o	\
	font.o		\
	hud.o		\
	image.o		\
	item.o		\
	joystick.o	\
	keyboard.o	\
	level.o		\
	menu.o		\
	music.o		\
	player.o	\
	point.o		\
	rect.o		\
	sample.o	\
	script.o	\
	storyboard.o	\
	surface.o	\
	texture.o	\
	timer.o		\
	video.o		\
	weapon.o	\
	main.o		\
	$(RES)

# Executable target
TARGET	= ArcadEngine.exe


# Compile target
$(TARGET): $(OBJS)
	@echo -e "   LD\t$(notdir $(TARGET))"
	@$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Compile objects
%.o: %.c
	@echo -e "   CC\t$(notdir $<)"
	@$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	@echo -e "   CXX\t$(notdir $<)"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean data
clean:
	@echo -e "Cleaning..."
	@del -f *.o $(TARGET)
