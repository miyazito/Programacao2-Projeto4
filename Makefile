CFLAGS = -Wall -g # flags de compilacao
LDFLAGS = -lm

CC = gcc

# arquivos-objeto
	objects = src/vina++.o src/archiver.o src/arguments.o src/vina_options.o src/vina_aux_functions.o
     
all: vina++ 

vina++: $(objects)
	$(CC) $(objects) $(LDFLAGS) -o vina++

src/%.o: src/%.c
	$(CC) -c $< -o $@ $(CFLAGS) 

clean:
	rm -f $(objects) vina++