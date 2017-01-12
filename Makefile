

CC = gcc
SRC_DIR = src

CFLAGS = -Wall -pedantic -std=c99 -D_GNU_SOURCE #options du compilateur

INC_PATH = -I/Library/Frameworks/SDL.framework/Headers  -I/Library/Frameworks/SDL_ttf.framework/Headers #Path vers les headers (.h) des librairies qui ne sont dans les repertoires de recherches par défaut, ici exemple pour compiler avec la SDL sous mac

OBJS = $(SRC_DIR)/main.o $(SRC_DIR)/campagne.o $(SRC_DIR)/gestion.o $(SRC_DIR)/map.o $(SRC_DIR)/menu.o $(SRC_DIR)/multi.o $(SRC_DIR)/joueur.o #nom des fichiers objets que l'on va créer. Le fichier SDLmain.m doit etre copié dans le repertoire courant sous mac !

LIB = -lSDL -lSDL_image -lSDL_ttf -lSDL_mixer -lm  #les bibliothèques nécessaires à la compilation

PROG = bomberman #nom de l'executable après compilation

all : $(OBJS) 
	$(CC) $(INC_PATH) -o $(PROG) $(OBJS) $(LIB) #une tabulation avant $(CC)

$(SRC_DIR)/campagne.o: $(SRC_DIR)/campagne.c
	$(CC) $(CFLAGS) $(INC) -c $(SRC_DIR)/campagne.c -o $(SRC_DIR)/campagne.o

$(SRC_DIR)/gestion.o: $(SRC_DIR)/gestion.c
	$(CC) $(CFLAGS) $(INC) -c $(SRC_DIR)/gestion.c -o $(SRC_DIR)/gestion.o

$(SRC_DIR)/main.o: $(SRC_DIR)/main.c
	$(CC) $(CFLAGS) $(INC) -c $(SRC_DIR)/main.c -o $(SRC_DIR)/main.o

$(SRC_DIR)/map.o: $(SRC_DIR)/map.c
	$(CC) $(CFLAGS) $(INC) -c $(SRC_DIR)/map.c -o $(SRC_DIR)/map.o

$(SRC_DIR)/menu.o: $(SRC_DIR)/menu.c
	$(CC) $(CFLAGS) $(INC) -c $(SRC_DIR)/menu.c -o $(SRC_DIR)/menu.o

$(SRC_DIR)/multi.o: $(SRC_DIR)/multi.c
	$(CC) $(CFLAGS) $(INC) -c $(SRC_DIR)/multi.c -o $(SRC_DIR)/multi.o

$(SRC_DIR)/joueur.o: $(SRC_DIR)/joueur.c
	$(CC) $(CFLAGS) $(INC) -c $(SRC_DIR)/joueur.c -o $(SRC_DIR)/joueur.o


# regle pour nettoyer (make clean)
clean :
	-rm $(SRC_DIR)/*.o $(SRC_DIR)/*~ $(PROG)
