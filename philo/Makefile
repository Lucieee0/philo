# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lusimon <lusimon@student.42heilbronn.de    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/02 19:38:46 by lusimon           #+#    #+#              #
#    Updated: 2025/08/27 15:31:45 by lusimon          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Project name
NAME = philo

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -Werror

# Source files
SRCS = helper_functions.c philo_linked_list.c time.c main.c philo_routine.c monitor_routine.c

HEADER_FILE = philo.h

# Object directory (hidden folder)
OBJDIR = .objs

# Object files (replace .c with .o and add directory)
OBJS = $(addprefix $(OBJDIR)/, $(SRCS:.c=.o))

# Phony targets
.PHONY: all clean fclean re

# Default target
all: $(NAME)

# Create the executable
$(NAME): $(OBJDIR) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

# Create object directory
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Rule for creating object files in hidden directory
$(OBJDIR)/%.o: %.c $(HEADER_FILE)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up object files and directory
clean:
	rm -rf $(OBJDIR)

# Clean up the executable
fclean: clean
	rm -f $(NAME)

# Rebuild the library
re: fclean all

# Specify what happens when the Makefile is executed
help:
	@echo "Makefile for $(NAME)"
	@echo "Usage:"
	@echo "  make          Build the project"
	@echo "  make clean    Remove object files"
	@echo "  make fclean   Remove object files and the library"
	@echo "  make re       Rebuild the project"
	@echo "  make help     Display this help message"