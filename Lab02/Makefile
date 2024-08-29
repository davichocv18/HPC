# Nombre del ejecutable
EXEC = matrix_product

# Compilador
CC = gcc

# Flags de compilación
CFLAGS = -Wall -Wextra -O2

# Archivos fuente
SRC = C_version.c

# Archivos objeto
OBJ = $(SRC:.c=.o)

# Regla para compilar el ejecutable
$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJ)

# Regla para compilar los archivos objeto
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Regla para limpiar los archivos generados
clean:
	rm -f $(OBJ) $(EXEC)
