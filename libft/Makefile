SRC		= ft_isalpha.c ft_isalnum.c ft_isdigit.c ft_isascii.c \
                ft_isprint.c ft_strlen.c ft_memset.c ft_bzero.c ft_memcpy.c \
                ft_memmove.c ft_strlcpy.c ft_strlcat.c ft_toupper.c \
                ft_tolower.c ft_strchr.c ft_strrchr.c ft_strncmp.c \
                ft_memchr.c ft_memcmp.c ft_strnstr.c ft_atoi.c ft_calloc.c \
                ft_strdup.c ft_substr.c ft_strjoin.c ft_strtrim.c ft_split.c \
                ft_itoa.c ft_strmapi.c ft_striteri.c get_next_line.c get_next_line_utils.c

OBJ		= ${SRC:.c=.o}

CFLAGS	= -Wall -Werror -Wextra
CC		= cc
NAME	= libft.a

all:		${NAME}

${NAME}:	${OBJ}
	ar rcs ${NAME} ${OBJ}

%.o:		%.c
	${CC} ${CFLAGS} -c $< -o $@
    
clean:
	rm -f ${OBJ}

fclean: clean
	rm -f ${NAME}

re: fclean all