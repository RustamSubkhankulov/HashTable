#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

#include "hamlet.h"
#include "../general/general.h"
#include "../logs/errors_and_logs.h"

//===============================================

#ifdef HAMLET_LOGS

	#define hamlet_log_report() \
	 	    log_report()
	
#else 

	#define hamlet_log_report() ""

#endif 

//===============================================

static int _hamlet_load_to_buffer(Hamlet* hamlet, FILE* source FOR_LOGS(, LOG_PARAMS));

static long _get_file_size(FILE* source FOR_LOGS(, LOG_PARAMS));

static int _fill_buffer_from_file(Hamlet* hamlet, FILE* source FOR_LOGS(, LOG_PARAMS));

static int _hamlet_count_entities(Hamlet* hamlet FOR_LOGS(, LOG_PARAMS));

static int _hamlet_split(Hamlet* hamlet FOR_LOGS(, LOG_PARAMS));

#ifdef SPLIT_IN_WORDS 

	static int _hamlet_words_init(Hamlet* hamlet FOR_LOGS(, LOG_PARAMS));

	static int _hamlet_count_words(Hamlet* hamlet FOR_LOGS(, LOG_PARAMS));

#else 

	static int _hamlet_strings_init(Hamlet* hamlet FOR_LOGS(, LOG_PARAMS));

	static int _str_is_empty(char* string_start FOR_LOGS(, LOG_PARAMS));

#endif 

//===============================================

#define hamlet_load_to_buffer(hamlet, src) \
       _hamlet_load_to_buffer(hamlet, src FOR_LOGS(, LOG_ARGS))

#define get_file_size(src) \
	   _get_file_size(src FOR_LOGS(, LOG_ARGS))

#define fill_buffer_from_file(hamlet, src) \
	   _fill_buffer_from_file(hamlet, src FOR_LOGS(, LOG_ARGS))

#define hamlet_count_entities(hamlet) \
	   _hamlet_count_entities(hamlet FOR_LOGS(, LOG_ARGS))

#define hamlet_split(hamlet) \
	   _hamlet_split(hamlet FOR_LOGS(, LOG_ARGS))

#define hamlet_words_init(hamlet) \
	   _hamlet_words_init(hamlet FOR_LOGS(, LOG_ARGS))

#define hamlet_count_words(hamlet) \
	   _hamlet_count_words(hamlet FOR_LOGS(, LOG_ARGS))

#define hamlet_strings_init(hamlet) \
	   _hamlet_strings_init(hamlet FOR_LOGS(, LOG_ARGS))

#define str_is_empty(str) \
	   _str_is_empty(str FOR_LOGS(, LOG_ARGS))

//===============================================

int _hamlet_init(struct Hamlet* hamlet, const char* filename FOR_LOGS(, LOG_PARAMS))
{
	hamlet_log_report();

	assert(filename);
	assert(hamlet);

	FILE* source = open_file(filename, "rb");
	if (!source) return -1;

	int ret_val = hamlet_load_to_buffer(hamlet, source);
	if (ret_val == -1) return -1;

	return hamlet_split(hamlet);
}

//-----------------------------------------------

int _hamlet_destruct(Hamlet* hamlet FOR_LOGS(, LOG_PARAMS))
{
	hamlet_log_report();
	assert(hamlet);
	
	free(hamlet->tokens);
	hamlet->tokens = NULL;

	free(hamlet->buffer);
	hamlet->buffer = NULL;

	hamlet->size   = 0;
	hamlet->number = 0;

	return 0;
}

//-----------------------------------------------

static int _hamlet_load_to_buffer(Hamlet* hamlet, FILE* source FOR_LOGS(, LOG_PARAMS))
{
	hamlet_log_report();

	assert(hamlet);
	assert(source);

	long file_size = get_file_size(source);
	if (file_size == -1) return -1;

	hamlet->size = file_size;

	int ret_val = fill_buffer_from_file(hamlet, source);
	if (ret_val == -1);

	if (fclose(source) != 0)
	{
		error_report(FCLOSE_ERR);
		return -1;
	}

	return 0;
}

//-----------------------------------------------

static long _get_file_size(FILE* source FOR_LOGS(, LOG_PARAMS))
{
	hamlet_log_report();
	assert(source);

	if (fseek(source, 0, SEEK_END) != 0)
		return -1;

	long size = ftell(source);
	if (size == -1) return -1;

	rewind(source);

	return size;
}

//-----------------------------------------------

static int _fill_buffer_from_file(Hamlet* hamlet, FILE* source FOR_LOGS(, LOG_PARAMS))
{
	hamlet_log_report();

	assert(hamlet);
	assert(source);

	hamlet->buffer = (char*) calloc(hamlet->size + 1, sizeof(char));
	if (!hamlet->buffer)
	{
		error_report(CANNOT_ALLOCATE_MEM);
		return -1;
	}

	int fread_ret = fread(hamlet->buffer, sizeof(char), hamlet->size, source);
	if (fread_ret != hamlet->size)
	{
		error_report(FREAD_ERR);
		return -1;
	}

	hamlet->buffer[hamlet->size] = '\0';
	return 0;
}

//-----------------------------------------------

static int _hamlet_count_entities(Hamlet* hamlet FOR_LOGS(, LOG_PARAMS))
{
	hamlet_log_report();
	assert(hamlet);

	#ifndef SPLIT_IN_WORDS

		char* slash_n = hamlet->buffer;

		while ((slash_n = strchr(slash_n, '\n')) != NULL)
		{
			hamlet->number += 1;
			slash_n += 1;
		}

	#else 

		int ret_val = hamlet_count_words(hamlet);
		if (ret_val == -1) return -1;

	#endif 

	return 0;
}

//-----------------------------------------------

static int _hamlet_split(Hamlet* hamlet FOR_LOGS(, LOG_PARAMS))
{
	hamlet_log_report();

	assert(hamlet);

	int ret_val = hamlet_count_entities(hamlet);
	if (ret_val == -1) return -1;

	hamlet->strings = (Token*) calloc(hamlet->number, sizeof(Token));
	if (!hamlet->tokens) return -1;

	#ifdef SPLIT_IN_WORDS

		ret_val = hamlet_words_init(hamlet);
		if (ret_val) == -1;

	#else 

		ret_val = hamlet_strings_init(hamlet);
		if (ret_val) == -1;

	#endif 

	return 0;
}

//-----------------------------------------------

#ifdef SPLIT_IN_WORDS

static int _hamlet_count_words(Hamlet* hamlet FOR_LOGS(, LOG_PARAMS))
{
	hamlet_log_report();
	assert(hamlet);

	char* buffer = hamlet->buffer;
	unsigned long long int words_ct = 0;
	int inword   = 0;

	while (*buffer++ != '\0')
	{	
		if (isalpha(*buffer) && !inword)
		{
			inword = 1;
			continue;
		}

		if (!isalpha(*buffer) && inword)
		{
			words_ct++; 
			inword = 0;
		}
	}

	hamlet->number = words_ct;

	return 0;
}
//-----------------------------------------------

static int _hamlet_words_init(Hamlet* hamlet FOR_LOGS(, LOG_PARAMS))
{
	hamlet_log_report();
	assert(hamlet);

	char* buffer = hamlet->buffer;

	unsigned long long int words_ct = 0;
	int   char_ct    = 0;
	char* word_start = NULL;
	char  inword     = 0;

	while (*buffer != '\0')
	{	
		if (isalpha(*buffer) && !inword)
		{
			word_start = buffer;
			inword = 1;
		}

		if (isalpha(*buffer) && inword)
		{
			char_ct++;
		}

		if (!isalpha(*buffer) && inword)
		{
			*buffer = '\0';

			hamlet->tokens[words_ct].data = word_start;
			hamlet->tokens[words_ct].len  = char_ct;
			hamlet->tokens[words_ct].num  = words_ct;

			char_ct = 0;
			inword  = 0;
			words_ct++; 
		}

		buffer++;
	}

	return 0;
}

#endif 

//-----------------------------------------------

#ifdef SPLIT_IN_STRINGS

static int _hamlet_strings_init(Hamlet* hamlet FOR_LOGS(, LOG_PARAMS))
{
	hamlet_log_report();
	assert(hamlet);

	unsigned long long string_ct = 0;

	char* string_start = strtok(hamlet->buffer, "\n\r");
	if (!string_start)
	{
		error_report(NO_STRINGS_IN_TEXT);
		return -1;
	}

	// //
	// printf("\n number of emtities: %d size %d\n", hamlet->number, hamlet->size);
	// //
	// printf("\n first string start: %s \n", string_start);	
	// //
	// fflush(stdout);

	while(string_start)
	{
		if (!str_is_empty(string_start))
		{
			while(*string_start == ' ' || *string_start == '\t')
				string_start++;

			// printf("added string: |%s|\n", string_start);
			// fflush(stdout);

			hamlet->tokens[string_ct].data = string_start;
			hamlet->tokens[string_ct].len  = strlen(string_start);
			hamlet->tokens[string_ct].num  = string_ct;

			// printf("\n String data: |%s| len = %d number = %d \n", hamlet->strings[string_ct].data, hamlet->strings[string_ct].len, hamlet->strings[string_ct].num);
			// fflush(stdout);

			string_ct++;
		}

		string_start = strtok(NULL, "\n\r");

		//printf("\n %dth string_start: %p \n", string_ct - 1, string_start);
		//printf("\n |%s| \n", string_start);
		//fflush(stdout);
	}

	hamlet->number = string_ct;

	return 0;
}

//-----------------------------------------------

static int _str_is_empty(char* string_start FOR_LOGS(, LOG_PARAMS))
{
	hamlet_log_report();
	assert(string_start);

	while (*string_start != 0)
	{
		if (isalpha(*string_start))
			return 0;

		string_start++;
	}

	return 1;
}

#endif 

//-----------------------------------------------

int _hamlet_print_data(Hamlet* hamlet FOR_LOGS(, LOG_PARAMS))
{
	hamlet_log_report();
	assert(hamlet);

	// printf("\n hamlet->number %lld \n", hamlet->number);

	for (unsigned long long int counter = 0;
					            counter < hamlet->number;
					            counter++)
	{
			printf("%05d: len = %03d |%s| \n", hamlet->tokens[counter].num, 
											   hamlet->tokens[counter].len, 
											   hamlet->tokens[counter].data);
	}

	return 0;
}