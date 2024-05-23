CC = gcc
CCFLAGS = -Wall -O2 -c -I. -o
LDFLAGS = -O2 -g -o 
LDLIBRARY = -L. -lrtos -lpthread
NAME = pod

OBJECTS_CONTROL = sensors.o actuators.o architecture.o simulation_parameters.o control.o

all: $(OBJECTS_CONTROL)
	$(CC) $(LDFLAGS) $(NAME) $(OBJECTS_CONTROL) $(LDLIBRARY)

$(OBJECTS_CONTROL): %.o: %.c
	$(CC) $(CCFLAGS) $@ $<

clean:
	rm -f *.o
	rm -f *pipe*
	rm -f $(NAME)


