NAME = mandelbrot

all: $(NAME)

$(NAME): $(NAME).c
	$(CC) -o $@ $<

clean:
	rm -f $(NAME);

run: $(NAME)
	./$(NAME)

.PHONY: all clean run
