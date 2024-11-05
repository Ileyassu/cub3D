SRC = \
	src/main.c \
	src/map.c \
	src/player.c

OBJ = $(SRC:.c=.o)

CFLAGS = -Wall -Wextra -Werror

AR = ar rcs

NAME = cub3D

all : $(NAME)

$(NAME): $(OBJ)
	@echo "compiling $(NAME)"
	$(CC) $(OBJ) -Lmlx -lmlx -framework OpenGL -framework AppKit -o $(NAME)
	@sleep 1
	@echo "$(NAME) compiled successfully."

%.o: %.c
	$(CC) -Wall -Wextra -Werror -Imlx -c $< -o $@

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