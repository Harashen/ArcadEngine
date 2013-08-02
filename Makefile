# C/C++ compiler
CC		?= gcc
CXX		?= g++

# Directories
INCDIRS		= . /usr/include/freetype2
LIBDIRS		=

# Includes/Libraries
INCLUDE		= $(foreach dir, $(INCDIRS), -I$(dir))
LIBS		= $(foreach dir, $(LIBDIRS), -L$(dir))

# C/C++ flags
CFLAGS		= -O3 -Wall -std=c99 $(INCLUDE)
CXXFLAGS	= -O3 -Wall $(INCLUDE)

# Linker flags
LDFLAGS		= $(LIBS) -lftgl -lpng -lrt -lGL -lGLU -lSDL_image -lSDL_mixer -lSDL

# Objects
OBJS	= \
	ai.o		\
	animation.o	\
	audio.o		\
	bullet.o	\
	camera.o	\
	character.o	\
	collision.o	\
	color.o		\
	console.o	\
	credits.o	\
	enemy.o		\
	engine.o	\
	entity.o	\
	font.o		\
	hud.o		\
	image.o		\
	input.o		\
	item.o		\
	level.o		\
	menu.o		\
	movie.o		\
	music.o		\
	object.o	\
	player.o	\
	rect.o		\
	resource.o	\
	sample.o	\
	script.o	\
	sdl_ffmpeg.o	\
	storyboard.o	\
	surface.o	\
	texture.o	\
	timer.o		\
	video.o		\
	main.o

# Executable target
TARGET	= litEngine


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
	@rm -f *.o $(TARGET)
