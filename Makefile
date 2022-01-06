OBJS=regexp.o player.o

all: $(OBJS)
	$(CC) $(LDFLAGS) $^
