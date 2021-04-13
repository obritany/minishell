#SETUP
NAME		= minishell
CC			= gcc
CFLAGS		= -Wall -Wextra -Werror
RM			= rm -f

#FILES
FLS			= main.c

SRCS_DIR	= sources/
HEADER		= $(SRCS_DIR)minishell.h
SRCS		= $(addprefix $(SRCS_DIR), $(FLS))
OBJS		= $(SRCS:.c=.o)

LIBFT_DIR	= libft/
LIBFT		= $(LIBFT_DIR)libft.a

#COMMANDS
all:		tools $(NAME)

$(NAME):	$(LIBFT) $(HEADER) $(OBJS)
			$(CC) $(CFLAGS) $(LIBFT) $(OBJS) -o $(NAME)
			@echo $(NAME) created!

tools:
			$(MAKE) -C $(LIBFT_DIR)

clean:
			$(RM) $(OBJS)
			$(MAKE) -C $(LIBFT_DIR) clean
			@echo $(NAME) cleaned!

fclean:		clean
			$(RM) $(NAME)
			$(RM) $(LIBFT)

re:			fclean all

.PHONY:		all clean fclean re