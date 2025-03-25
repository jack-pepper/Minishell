# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/11 13:50:01 by mmalie            #+#    #+#              #
#    Updated: 2025/03/25 18:44:21 by mmalie           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

###################################################
### Name of the file obtained after compilation ###
###################################################

NAME = minishell

############################
### Compilation settings ###
############################

CC=cc
CFLAGS=-g -Wall -Wextra -Werror

####################
### Dependencies ###
####################

DEPS = ./inc/minishell.h
DEPS_LIBFT = ./libft/libft.h
LIBFT = ./libft/libft.a

####################
### Source files ###
####################

SRC = ./src/minishell.c \
      ./src/ms_signals.c \
      ./src/ms_input_manager.c \
      ./src/ms_commands_manager.c \
      ./src/commands/ms_cmd_cd.c \
      ./src/commands/ms_cmd_echo.c \
      ./src/commands/ms_cmd_env.c \
      ./src/commands/ms_cmd_exit.c \
      ./src/commands/ms_cmd_export.c \
      ./src/commands/ms_cmd_pwd.c \
      ./src/commands/ms_cmd_unset.c \

### Object files
OBJ = $(SRC:.c=.o)

##############################################
### Default rules (compile the executable) ###
##############################################

all: $(LIBFT) $(NAME)

$(LIBFT):
	cd libft && make

$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(OBJ) -Llibft -lft -lreadline -lncurses -o $(NAME)

######################################
### Compile .c files into .o files ###
######################################

$(OBJ): %.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

#########################
### Cleaning-up rules ###
#########################

clean:
	cd libft && make clean
	rm -f $(OBJ)

fclean: clean
	cd libft && make fclean
	rm -f $(NAME)

re: fclean all

#########################
### .PHONY Exceptions ###
#########################

.PHONY: all clean fclean re bonus
