#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

#include "hamlet.h"
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

static int _get_file_size(FILE* source FOR_LOGS(, LOG_PARAMS));

static int _fill_buffer_from_file(Hamlet* hamlet, FILE* source FOR_LOGS(, LOG_PARAMS));

static int _hamlet_count_entities(Hamlet* hamlet FOR_LOGS(, LOG_PARAMS));

static int _hamlet_split(Hamlet* hamlet FOR_LOGS(, LOG_PARAMS));

#ifdef SPLIT_IN WORDS 

	static int _hamlet_words_init(Hamlet* hamlet FOR_LOGS(, LOG_PARAMS));

#else 

	static int _hamlet_strings_init(Hamlet* hamlet FOR_LOGS(, LOG_PARAMS));

#endif 

//===============================================

#define hamlet_load_to_buffer(hamlet) \
       _hamlet_load_to_buffer(hamlet FOR_LOGS(, LOG_ARGS))

#define get_file_size(src) \
	   _get_file_size(src FOR_LOGS(, LOG_ARGS))

#define fill_buffer_from_file(hamlet, src) \
	   _fill_buffer_from_file(hamlet, src FOR_LOGS(, LOG_ARGS))

#define hamlet_count_entities(hamlet) \
	   _hamlet_count_entities(hamlet FOR_LOGS(, LOG_ARGS))

#define hamlet_split(hamlet) \
	   _hamlet_split(hamlet FOR_LOGS(, LOG_ARGS))

//===============================================

int _hamlet_init(struct Hamlet* hamlet, const char* filename FOR_LOGS(, LOG_PARAMS))
{
	hamlet_log_report();

	assert(filename);
	assert(hamlet);

	FILE* source = open_file(filename, "rb");
	if (!source)
		return -1;

	if (hamlet_load_to_buffer(hamlet, source) == -1)
		return -1;

	return hamlet_split(hamlet);
}

//-----------------------------------------------

int _hamlet_destruct(Hamlet* hamlet FOR_LOGS(, LOG_PARAMS))
{
	hamlet_log_report();

	assert(hamlet);

	#ifdef SPLIT_IN_WORDS

		free(hamlet->words);
		hamlet->words = NULL;

	#else

		free(hamlet->strings);
		hamlet->strings = NULL;

	#endif 

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

	int file_size = get_file_size(source);
	if (file_Size == -1)
		return NULL;

	hamlet->size = (unsigned int) file_size;

	if (fill_buffer_from_file(hamlet, source) == -1)
		return -1;

	if (fclose(source != 0))
	{
		error_report(FCLOSE_ERR);
		return -1;
	}

	return 0;
}

//-----------------------------------------------

static int _get_file_size(FILE* source FOR_LOGS(, LOG_PARAMS))
{
	hamlet_log_report();

	assert(source);

	if (fseek(source, 0, SEEK_END) != 0)
		return -1;

	int size = ftell(source);
	if (size == -1)
		return -1;

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
			hamler->number += 1;
			slash_n += 1;
		}

	#else 



	#endif 
}

//-----------------------------------------------

static int _hamlet_split(Hamlet* hamlet FOR_LOGS(, LOG_PARAMS))
{
	hamlet_log_report();

	assert(hamlet);

	if (hamlet_count_entities(hamlet) == -1)
		return -1;

	#ifdef SPLIT_IN_WORDS

		hamlet->words = (*Word) calloc(hamlet->number, sizeof(Word));
		if (!hamlet->words)
			return -1;

		if (hamlet_words_init(hamlet) == -1)
			return -1;

	#else 

		hamlet->strings = (*String) calloc(hamlet->number, sizeof(String));
		if (!hamlet->strings)
			return -1;

		if (hamlet_strings_init(hamlet) == -1)
			return -1;

	#endif 
}

//-----------------------------------------------

#ifdef SPLIT_IN_WORDS

	static int _hamlet_words_init(Hamlet* hamlet FOR_LOGS(, LOG_PARAMS))
	{
		hamlet_log_report();

		assert(hamlet);

		return 0;
	}

#endif 

//-----------------------------------------------

#ifdef SPLIT_IN_STRINGS

	static int _hamlet_strings_init(Hamlet* hamlet FOR_LOGS(, LOG_PARAMS))
	{
		hamlet_log_report();

		assert(hamlet);

		return 0;
	}

#endif 