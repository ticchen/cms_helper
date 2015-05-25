#################################
# App config
#################################
app=main
CC=gcc
CFLAGS+=
LDFLAGS+=


#################################
# Framework for dependency check
#################################
sources=$(wildcard *.c)
CFLAGS+= -MMD -MP
LDFLAGS+=

all: $(app)
	./$(app)

${app}: $(sources:.c=.o)
	$(CC) $(LDFLAGS) $^ -o $(app)

clean:
	rm -rf *.o *.d ${app}
	
-include $(sources:.c=.d)

astyle:
	astyle  --style=linux --indent=tab --pad-oper --unpad-paren -p *.c *.h
