C_FLAGS = -O2 -lallegro -lallegro_primitives -lallegro_image -lm -lallegro_font -lallegro_ttf -Wall -Werror -Wextra

C_DEBUG = -g  -O3 -Wno-inline -pg -lallegro -lallegro_primitives -lallegro_image -lm -lallegro_font -lallegro_ttf -Wall -Werror -Wextra

OBJ_FLAGS = -O3 -Wall -Werror -Wextra
OBJS = clusters.o cluster_utils.o

%.o: %.c
	gcc -c $< -o $@ $(OBJ_FLAGS)

clusters_debug: $(OBJS) 
	gcc -o clusters_debug $(OBJS) $(C_DEBUG)

clusters: $(OBJS)
	gcc -o clusters $(OBJS) $(C_FLAGS)

game_of_life: game_of_life.c
	gcc game_of_life.c -o game_of_life -lallegro -O3 -lm -lallegro_font -lallegro_image -lallegro_primitives -Wall -Werror -Wextra

.PHONY: clean

clean:
	rm -f *.o *.exe *.out clusters game_of_life orbit
