NAME			=	mlx_test

SRCS			=	$(addprefix $(SRCS_PATH), \
					main.c)

SRCS_PATH		=	./srcs/

OBJS			=	$(SRCS:.c=.o)

CC				=	gcc

CFLAGS			=	-Wall -Wextra -Werror

RM				=	rm -rf

LIBFT_PATH		=	./libs/libft/

MLX_PATH		=	./libs/mlx/

LDLIBS			=	-lmlx -lft -lXext -lX11 -lm -lz

LDFLAGS			=	-L$(LIBFT_PATH) -L$(MLX_PATH)

LIBFT			=	$(LIBFT_PATH)/libft.a

MLX				=	$(MLX_PATH)/mlx.a

all : $(NAME)

$(NAME) : $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) $(LDLIBS) -o $@

libs : $(LIBFT) $(MLX)

$(LIBFT) :
	make -C $(LIBFT_PATH)

$(MLX) :
	make -C $(MLX_PATH)

%.o : %.c Makefile libs
	$(CC) $(CFLAGS) -c $< $(LDFLAGS) $(LDLIBS) -o $@

clean :
	make clean -C $(LIBFT_PATH)
	make clean -C $(MLX_PATH)
	$(RM) $(OBJS)

fclean : clean
	$(RM) $(NAME)

re : fclean $(NAME)

.PHONY : all mlx_test libs clean fclean re
