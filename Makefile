SRC = \
    src/main.c \
    src/map.c \
    src/player.c

OBJ = $(SRC:.c=.o)

MINILIBX = mlx-linux/libmlx_Linux.a

CFLAGS = -Wall -Wextra -Werror

AR = ar rcs

NAME = cub3D

all : $(NAME)

$(NAME): $(OBJ)
	@echo "compiling $(NAME)"
	@$(CC) $(OBJ) -Lmlx_linux -lmlx_Linux -L/usr/lib -Imlx_linux -lXext -lX11 -lm -lz -o $(NAME)
	@sleep 1
	@echo "$(NAME) compiled successfully."

%.o: %.c
	@$(CC) -Wall -Wextra -Werror -I/usr/include -Imlx_linux -O3 -Imlx -c $< -o $@

fclean : clean
	@echo full cleaning of $(NAME) ...
	@$(RM) -f $(NAME)
	@sleep 0.5
	@echo $(NAME) cleaned.

clean :
	@echo cleaning $(NAME) ...
	@$(RM) -f $(OBJ)
	@sleep 0.5
	@echo $(NAME) cleaned.

re : fclean all