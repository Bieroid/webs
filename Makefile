SRCS_DIR	=	./srcs
SRCS		=	$(shell basename -a $(shell find ${SRCS_DIR} -type f -name '*.cpp'))

OBJS_FILES	=	${SRCS:.cpp=.o}
OBJS_DIR	=	./objs
OBJS		=	${addprefix ${OBJS_DIR}/, ${OBJS_FILES}}

DEPS		=	${OBJS:.o=.d}

INCS_DIR	=	./incs
HEADERS		=	-I ${INCS_DIR}

NAME		=	webserv

STANDART	=	-std=c++98
WARN		=	-Wall -Wextra -Werror
OPTIMIZERS	=	-O2
CXXFLAGS	=	-MMD -g3 ${STANDART}
CC			=	c++

RM			=	rm -rf

all:	${NAME}

go:		all
	./webserv

valgrind: all
	valgrind --leak-check=full --track-origins=yes ./webserv

${NAME}:	${OBJS}
		${CC} ${CXXFLAGS} ${OBJS} ${HEADERS} -o ${NAME}

-include ${DEPS}
${OBJS_DIR}/%.o:	${SRCS_DIR}/%.cpp
	@mkdir -p ${OBJS_DIR}
	${CC} ${CXXFLAGS}  ${HEADERS} -c $< -o $@

clean:
	${RM} ${OBJS_DIR}

fclean:	clean
	${RM} ${NAME}

re:			fclean all

.PHONY:		all clean fclean re