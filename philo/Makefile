NAME = philo

CC = cc
CFLAGS = -Wall -Wextra -Werror #-fsanitize=thread #only with clang

VALID_DIR = ./validation/
TYPES_DIR = ./types/
SIMUL_DIR = ./simulation/

INCLUDES = -I. -I$(VALID_DIR) -I$(TYPES_DIR) -I$(SIMUL_DIR)

SRCS = $(VALID_DIR)validation.c $(TYPES_DIR)memory.c $(TYPES_DIR)mutex.c \
       $(SIMUL_DIR)actions.c $(SIMUL_DIR)monitoring.c main.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
