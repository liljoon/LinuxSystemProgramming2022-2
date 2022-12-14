CFLAGS	=	-Wall -Wextra -Werror # c 컴파일 옵션, warning을 모두 표시하고 그것을 error처리
NAME	=	libftprintf.a # 최종적으로 만드려는 파일 명을 매크로 지정

SRCS	= ft_printf.c	ft_printf_hex.c ft_printf_utils.c # c파일들의 소스코드를 모아서 매크로

.SUFFIXES : .c .o # SUFFIXES 룰, .c 나 .o 모든 파일들이 거치게 됨
.c .o : # .c 파일을 이용해서 .o 파일로 컴파일, 내부에서 루틴을 자동으로 거침
	$(CC) $(CFLAGS) -c $<  # .c 파일을 CFLAGS 옵션을 넣어서 컴파일

OBJS	=	$(SRCS:.c=.o) # SRCS에서 .c를 전부 .o로 대체하여 OBJS라는 매크로 지정

$(NAME) : $(OBJS) # .o파일들로 libftprintf.a 라는 라이브러리 생성
	ar rc $(NAME) $(OBJS) # ar명령어로 static library를 목적파일을 이용해서 생성

all : $(NAME) # make all 명령어를 입력했을 때 make libftprint.a를 한 것과 동일하게 처리

clean : # make clean 실행 시
	rm -f $(OBJS) # 목적파일들만 전부 지움

fclean : clean # make fclean 실행 시
	rm -f $(NAME) $(OBJS) # 목적파일과 라이브러리 파일 삭제

re : fclean all # make로 만들어진 모든 파일 지우고 다시 생성

.PHONY : all clean fclean re # target파일이 아닌 단순 명령어 사용시 동명의 파일이 존재해도 명령어 우선 처리
