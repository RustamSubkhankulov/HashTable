CC = gcc 

OBJ = obj/main.o obj/hash.o obj/logs.o obj/general.o obj/list.o obj/list_tests.o obj/hamlet.o obj/hash_func.o

all: global

global: $(OBJ) 
	$(CC) $(OBJ) -o hash -fsanitize=address -fsanitize=bounds

obj/hamlet.o: src/hamlet/hamlet.cpp src/global_conf.h src/hamlet/hamlet.h src/hamlet/hamlet_conf.h
	$(CC) src/hamlet/hamlet.cpp -c -o obj/hamlet.o 

obj/main.o: src/main.cpp src/global_conf.h
	$(CC) src/main.cpp -c -o obj/main.o

obj/hash.o: src/global_conf.h src/hash/hash.cpp src/hash/hash.h src/hash/hash_conf.h src/hash/hash_func.cpp src/hash/hash_func.h
	$(CC) src/hash/hash.cpp -c -o obj/hash.o 

obj/hash_func.o: src/global_conf.h src/hash/hash.cpp src/hash/hash.h src/hash/hash_conf.h src/hash/hash_func.cpp src/hash/hash_func.h
	$(CC) src/hash/hash_func.cpp -c -o obj/hash_func.o 

obj/logs.o: src/global_conf.h src/logs/errors_and_logs.cpp src/logs/errors_and_logs.h src/logs/errors.h src/logs/log_definitions.h src/include/errors.txt
	$(CC) src/logs/errors_and_logs.cpp -c -o obj/logs.o 

obj/general.o: src/global_conf.h src/general/general.cpp src/general/general.h 
	$(CC) src/general/general.cpp -c -o obj/general.o 

obj/list.o: src/global_conf.h src/list/list.cpp src/list/list.h src/list/list_config.h 
	$(CC) src/list/list.cpp -c -o obj/list.o

obj/list_tests.o: src/global_conf.h src/list/list_tests.cpp src/list/list_tests.h src/list/list_config.h 
	$(CC) src/list/list_tests.cpp -c -o obj/list_tests.o 

.PNONY: cleanup imagesdir

cleanup:
	rm obj/*.o 

imagesdir:
	mkdir /tmp/list_images
