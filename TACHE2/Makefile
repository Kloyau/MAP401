# Nom de l'exécutable
EXEC = geom2d

# Compilateur et options
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -lm

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

# Nettoyage
clean:
	rm -f $(OBJ) $(EXEC)

# Nettoyage complet
mrproper: clean
	rm -f *~