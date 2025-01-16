SRC = \
    src/main.c \
    src/map.c \
    src/player.c \
    parsing/parsing_v0.c \
    parsing/parsing_v1.c \
    parsing/parsing_v2.c \
    parsing/parsing_v3.c \
    parsing/utils_v0.c \
    parsing/utils_v1.c \

OBJ = $(SRC:.c=.o)


LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

CFLAGS = -Wall -Wextra -Werror #-fsanitize=address

RM = rm  
NAME = cub3D

all : $(NAME)

$(LIBFT):
	@echo "compiling libft"
	@$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(OBJ) $(LIBFT)
	@echo "compiling $(NAME)"
	@$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -Lmlx_linux -lmlx_Linux -L/usr/lib -Imlx_linux -lXext -lX11 -lm -lz -o $(NAME)
	@sleep 1
	@echo "$(NAME) compiled successfully."

%.o: %.c
	@$(CC) $(CFLAGS) -I/usr/include -Imlx_linux -O3 -Imlx -c $< -o $@

clean:
	@echo cleaning object files ...
	rm -f $(OBJ) && rm -f libft/*.o
	@sleep 0.5
	@echo object files cleaned.

fclean : clean
	@echo full cleaning of $(NAME) ...
	@$(RM) -f $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@sleep 0.5
	@echo $(NAME) cleaned.

re: fclean all