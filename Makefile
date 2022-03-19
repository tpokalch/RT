NAME = rtv1

OBJ = lin_alg.o color.o init_data.o events.o central.o comline_obj.o main.o check_arg.o hits.o brights.o memory.o linalg1.o entex.o create_points.o points_initialize.o free_points.o

FLAGS = #-Wextra -Werror -Wall
all: $(NAME)

$(NAME):
#	make -C libft/ fclean && make -C libft/
	clang -g $(FLAGS) -I libft/includes -o lin_alg.o -c lin_alg.c
	clang -g $(FLAGS) -I libft/includes -o color.o -c color.c
	clang -g $(FLAGS) -I libft/includes -o init_data.o -c init_data.c
	clang -g $(FLAGS) -I libft/includes -o events.o -c events.c
	clang -g $(FLAGS) -I libft/includes -o main.o -c main.c
	clang -g $(FLAGS) -I libft/includes -o central.o -c central.c
	clang -g $(FLAGS) -I libft/includes -o comline_obj.o -c comline_obj.c
	clang -g $(FLAGS) -I libft/includes -o check_arg.o -c check_arg.c
	clang -g $(FLAGS) -I libft/includes -o brights.o -c brights.c
	clang -g $(FLAGS) -I libft/includes -o hits.o -c hits.c
	clang -g $(FLAGS) -I libft/includes -o memory.o -c memory.c
	clang -g $(FLAGS) -I libft/includes -o linalg1.o -c linalg1.c
	clang -g $(FLAGS) -I libft/includes -o entex.o -c entex.c
	clang -g $(FLAGS) -I libft/includes -o create_points.o -c create_points.c
	clang -g $(FLAGS) -I libft/includes -o points_initialize.o -c points_initialize.c
	clang -g $(FLAGS) -I libft/includes -o free_points.o -c free_points.c


#	deleted gcc -I /usr/local/include
#	gcc $(OBJ) -o rtv1 -L /usr/local/lib/mlx -lmlx -L ./libft -lft -framework OpenGL -framework AppKit
	gcc $(OBJ) -o rtv1 -L /usr/local/lib/mlx -lmlx -L ./libft -lft -L/opt/X11/lib -lX11 -lXext -lpthread -framework OpenGL -framework AppKit


clean:
#	make -C libft/ clean
	rm -rf $(OBJ)

fclean: clean
#	make -C libft/ fclean
	rm -rf $(NAME)

re: fclean all
	rm -rf $(OBJ)
