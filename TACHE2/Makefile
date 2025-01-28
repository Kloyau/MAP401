# Nom de l'exécutable
EXEC = geom2d

# Compilateur et options
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -lm -I.  # -I. indique que les fichiers .h sont dans le même répertoire

# Fichiers source et objets
SRC = geom2d.c
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