# Nom de l'exécutable
EXEC = contours

# Compilateur et options
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -lm -I.  # -I. pour inclure les répertoires actuels des headers

# Fichiers source
SRC = geom2d.c image.c contours.c
OBJ = $(SRC:.c=.o)

# Règle principale
all: $(EXEC)

# Génération de l'exécutable
$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

# Compilation des fichiers objets
%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

# Nettoyage des fichiers objets et de l'exécutable
clean:
	rm -f $(OBJ) $(EXEC)

# Nettoyage complet (fichiers temporaires, backup, etc.)
mrproper: clean
	rm -f *~