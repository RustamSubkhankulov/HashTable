CC = gcc 

OBJ = obj/main.o obj/hash.o obj/logs.o 				\
	  obj/general.o obj/list.o obj/list_tests.o 	\
	  obj/hamlet.o obj/hash_func.o obj/crc32.o 		\
	  obj/search.o

OPT_FLAG = 

all: global

global: $(OBJ) 
	$(CC) $(OBJ) -o hash $(OPT_FLAG) -no-pie
	
#-fsanitize=address -fsanitize=bounds

obj/crc32.o: src/hash/crc32.s 
	nasm src/hash/crc32.s -f elf64 -o obj/crc32.o 

obj/search.o: src/list/search.s
	nasm src/list/search.s -f elf64 -o obj/search.o

obj/hamlet.o: src/hamlet/hamlet.cpp src/global_conf.h src/hamlet/hamlet.h src/hamlet/hamlet_conf.h
	$(CC) src/hamlet/hamlet.cpp -c -o obj/hamlet.o $(OPT_FLAG)

obj/main.o: src/main.cpp src/global_conf.h
	$(CC) src/main.cpp -c -o obj/main.o $(OPT_FLAG)

obj/hash.o: src/global_conf.h src/hash/hash.cpp src/hash/hash.h src/hash/hash_conf.h src/hash/hash_func.cpp src/hash/hash_func.h
	$(CC) src/hash/hash.cpp -c -o obj/hash.o $(OPT_FLAG)

obj/hash_func.o: src/global_conf.h src/hash/hash.cpp src/hash/hash.h src/hash/hash_conf.h src/hash/hash_func.cpp src/hash/hash_func.h
	$(CC) src/hash/hash_func.cpp -c -o obj/hash_func.o $(OPT_FLAG)

obj/logs.o: src/global_conf.h src/logs/errors_and_logs.cpp src/logs/errors_and_logs.h src/logs/errors.h src/logs/log_definitions.h src/include/errors.txt
	$(CC) src/logs/errors_and_logs.cpp -c -o obj/logs.o $(OPT_FLAG)

obj/general.o: src/global_conf.h src/general/general.cpp src/general/general.h 
	$(CC) src/general/general.cpp -c -o obj/general.o $(OPT_FLAG)

obj/list.o: src/global_conf.h src/list/list.cpp src/list/list.h src/list/list_config.h 
	$(CC) src/list/list.cpp -c -o obj/list.o $(OPT_FLAG)

obj/list_tests.o: src/global_conf.h src/list/list_tests.cpp src/list/list_tests.h src/list/list_config.h 
	$(CC) src/list/list_tests.cpp -c -o obj/list_tests.o $(OPT_FLAG)

.PNONY: cleanup imagesdir

cleanup:
	rm obj/*.o 

imagesdir:
	mkdir /tmp/list_images
