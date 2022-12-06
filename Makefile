all:
	@gcc ./src/*.c -c
	@gcc *.o -o h71
	@mkdir -p obj
	@mv *o obj