OBJS = src/allegroGameModes.c src/allegroMenu.c src/hiScore.c src/playersList.c src/questionsList.c src/main.c
CC = gcc
COMPILER_FLAGS = -Wall -Wextra
LINKER_FLAGS = -lallegro -lallegro_image -lallegro_ttf -lallegro_font -lallegro_dialog
OBJ_NAME = SimpleQuiz

all: $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

clean:
	rm $(OBJ_NAME)
