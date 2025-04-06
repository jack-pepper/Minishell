# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/11 13:50:01 by mmalie            #+#    #+#              #
#    Updated: 2025/04/06 12:01:23 by mmalie           ###   ########.fr        #
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

DEPS = ./inc/minishell.h ./libft/libft.h
LIBFT = ./libft/libft.a

####################
### Source files ###
####################

MS_SRC = ./src/minishell.c \
      ./src/ms_signals.c \
      ./src/ms_env_utils.c \
      ./src/ms_input_manager.c \
      ./src/ms_normalizer.c \
      ./src/ms_redirections_manager.c \
      ./src/ms_quotes_handler.c \
      ./src/ms_replace_utils.c \
      ./src/ms_commands_manager.c \
      ./src/commands/ms_cmd_cd.c \
      ./src/commands/ms_cmd_echo.c \
      ./src/commands/ms_cmd_env.c \
      ./src/commands/ms_cmd_exit.c \
      ./src/commands/ms_cmd_export.c \
      ./src/commands/ms_cmd_pwd.c \
      ./src/commands/ms_cmd_unset.c \
      ./src/yahya_ms_to_pipex.c \

PIPEX_SRC =       ./pipex_ms/pipex.c \
      ./pipex_ms/pipex_utils.c \
      ./pipex_ms/pipex_bonus.c \
      ./pipex_ms/pipex_bonus_utils.c \
      ./pipex_ms/parsing.c \
      ./pipex_ms/free.c \

SRC = $(MS_SRC) $(PIPEX_SRC)

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
