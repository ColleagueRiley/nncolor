CC = gcc

LIBS := -lgdi32 -lopengl32 -lwinmm -lm
EXT = .exe

ifneq (,$(filter $(CC),winegcc x86_64-w64-mingw32-gcc))
    detected_OS := Windows
	LIB_EXT = .dll
else
	ifeq '$(findstring ;,$(PATH))' ';'
		detected_OS := Windows
	else
		detected_OS := $(shell uname 2>/dev/null || echo Unknown)
		detected_OS := $(patsubst CYGWIN%,Cygwin,$(detected_OS))
		detected_OS := $(patsubst MSYS%,MSYS,$(detected_OS))
		detected_OS := $(patsubst MINGW%,MSYS,$(detected_OS))
	endif
endif

ifeq ($(detected_OS),Windows)
	LIBS := -lgdi32 -lopengl32 -lwinmm -lm
	EXT = .exe
endif
ifeq ($(detected_OS),Darwin)        # Mac OS X
	LIBS := -lm -framework Foundation -framework AppKit -framework OpenGL -framework CoreVideo
	EXT = 
endif
ifeq ($(detected_OS),Linux)
    LIBS := -lXrandr -lX11 -lm -lGL -ldl -lpthread
	EXT =
endif

nncolor: source/* include/*
	$(CC) source/main.c $(LIBS) -I./include -o $@

debug:
	$(CC) source/main.c $(LIBS) -I./include -o nncolor
	./nncolor