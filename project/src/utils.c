#include "utils.h"

static size_t g_memoryInUse = 0;

bool nullOrEmpty(const char *str) {
	return str == NULL || str[0] == '\0';
}

unsigned short intStrLength(const int number) {
	if(number >= 0) {
		return number == 0 ? 1 : (unsigned short) (log10(number) + 1);
	}
	else {
		return intStrLength(abs(number)) + 1;
	}
}

unsigned short intStrMaxLength(const int *numbers, const size_t size) {
	unsigned short high = 0;

	for(size_t i = 0; i < size; i++) {
		unsigned short len = intStrLength(numbers[i]);

		if(len > high) {
			high = len;
		}
	}

	return high;
}

void* simpleAlloc(const size_t bytes, const bool exitOnFail) {
	//DEBUG("Allocating memory region with size %" PRIu64 " bytes", bytes);

	void *ptr = malloc(bytes);
	if(ptr == NULL) {
		ERROR(errno, exitOnFail, "New memory region could not be allocated");
		return NULL;
	}
	memset(ptr, 0, bytes);

	return ptr;
}

bool simpleFree(void *ptr) {
	if(ptr != NULL) {
		free(ptr);
		//DEBUG("Pointer freed successfully");
		return true;
	}
	//DEBUG("Pointer could not be freed as it is undefined");
	return false;
}

FILE* openFile(const char *filePath, const char *mode, const bool exitOnFail) {
	DEBUG("File \"%s\" being opened with mode \"%s\"", filePath, mode);
	FILE *file = NULL;

	if(!nullOrEmpty(filePath)) {
		file = fopen(filePath, mode);
	}

	if(file == NULL) {
		ERROR(errno, exitOnFail, "File \"%s\" could not be opened", filePath);
	}

	return file;
}

bool closeFile(FILE *file, const bool exitOnFail) {
	if(file != NULL) {
		if(fclose(file)) {
			ERROR(errno, exitOnFail, "File could not be closed");
			return false;
		}
		
		DEBUG("File closed successfully");
		return true;
	}
	DEBUG("File could not be closed as it is undefined");
	return false;
}

bool readFile(FILE *file, const bool exitOnFail, const char *format, ...) {
	if(file != NULL && !nullOrEmpty(format)) {
		va_list args;
		va_start(args, format);

		DEBUG("File is being matched for format \"%s\"", format);
		vfscanf(file, format, args);

		if(ferror(file)) {
			if(exitOnFail) {
				closeFile(file, false);
			}

			ERROR(errno, exitOnFail, "File could not be read");
			clearerr(file);
			
			return false;
		}

		va_end(args);
		return true;
	}
	DEBUG("File could not be read as it is undefined or an invalid format was supplied");
	return false;
}

void printCharBitArray(const char *bitArray, const size_t size, const size_t bitsLine) {
	for(size_t bitPos = 0; bitPos < size; bitPos++) {
		if(bitPos % bitsLine == 0) {
			printf("\n%" PRIu64 ": ", bitPos / bitsLine);
		}

		printf("%" PRIu16 " ", CHECK_BIT(bitArray, bitPos, char) ? 1 : 0);
	}
	printf("\n");
}
