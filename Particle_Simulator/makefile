clusters_debug: clusters.c
	gcc clusters.c -o clusters -g -O3 -Wno-inline -pg -lallegro -lallegro_primitives -lallegro_image -lm -lallegro_font -lallegro_ttf -Wall -Werror -Wextra

clusters: clusters.c
	gcc clusters.c -o clusters -O3 -lallegro -lallegro_primitives -lallegro_image -lm -lallegro_font -lallegro_ttf -Wall -Werror -Wextra

game_of_life: game_of_life.c
	gcc game_of_life.c -o game_of_life -lallegro -O3 -lm -lallegro_font -lallegro_image -lallegro_primitives -Wall -Werror -Wextra

.PHONY: clean

clean:
	rm -f *.o *.exe *.out clusters game_of_life
