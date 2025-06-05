C_FLAGS =  -lallegro -lallegro_primitives -lallegro_image -lm -lallegro_font -lallegro_ttf -Wall -Werror -Wextra

C_DEBUG = -g -pg -Wno-inline -lallegro -lallegro_primitives -lallegro_image -lm -lallegro_font -lallegro_ttf -Wall -Werror -Wextra

OPTIM_FLAGS = -Ofast -ffast-math -funroll-loops -fomit-frame-pointer -march=native -fno-math-errno

OBJ_FLAGS = -Wall -Werror -Wextra -pg
OBJS = clusters.o cluster_utils.o

%.o: %.c
	gcc -c $< -o $@ $(OBJ_FLAGS)

clusters_debug: $(OBJS) 
	gcc -o clusters_debug $(OBJS) $(C_DEBUG) -pg

clusters: $(OBJS)
	gcc -o clusters $(OBJS) $(C_FLAGS) $(OPTIM_FLAGS)

game_of_life: game_of_life.c
	gcc game_of_life.c -o game_of_life -lallegro -O3 -lm -lallegro_font -lallegro_image -lallegro_primitives -Wall -Werror -Wextra

.PHONY: clean

clean:
	rm -f *.o *.exe *.out clusters game_of_life orbit
