#ifndef MACROS_H_
#define MACROS_H_

#include "constants.h"

/**
 * Utility macros for printing information, warning
 * error or debug messages
 * @constant NTRACE 	whether to print file name and line information
 * @constant NDEBUG		whether debug messages should be printed
 */
#ifndef NTRACE
	#define PRINT_MSG(file, header, format, ...) ( \
		fprintf((file), (header) SEPARATOR " " (format) "\n", ##__VA_ARGS__); \
	)

	#define PRINT_MSG(file, header, format, ...) ( \
		fprintf(stderr, (header) SEPARATOR " " (format), ##__VA_ARGS__); \
	)
#else
	#define PRINT_MSG(file, header, format, ...) ({ \
		fprintf(file, "%s" SEPARATOR "%s" SEPARATOR "%" PRIu32 SEPARATOR " " header " " format "\n", __FILE__, __func__, __LINE__, ##__VA_ARGS__); \
	})

	#define PRINT_ERROR(header, status, errval, format, ...) ({ \
		fprintf(stderr, "%s" SEPARATOR "%s" SEPARATOR "%" PRIu32 SEPARATOR " " header " " format, __FILE__, __func__, __LINE__, ##__VA_ARGS__); \
		HANDLE_ERROR(status, errval); \
	})
#endif

#define HANDLE_ERROR(status, errval) ({ \
	if((errval)) { \
		fprintf(stderr, SEPARATOR " %s", strerror((errval))); \
	} \
	fprintf(stderr, "\n"); \
	if((status)) { \
		exit((status)); \
	} \
})

#ifndef NDEBUG
	#define DEBUG(format, ...)
#else
	#define DEBUG(format, ...) ( \
		PRINT_MSG(stdout, DEBUG_HEADER, format, ##__VA_ARGS__) \
	)
#endif

#define ERROR(status, errval, format, ...) (PRINT_ERROR(ERROR_HEADER, status, errval, format, ##__VA_ARGS__))

#define F_ERROR(errval, format, ...) (ERROR(EXIT_FAILURE, errval, format, ##__VA_ARGS__))

#define NF_ERROR(errval, format, ...) (ERROR(0, errval, format, ##__VA_ARGS__))

#define INFO(format, ...) (PRINT_MSG(stdout, INFO_HEADER, format, ##__VA_ARGS__))

#define WARN(format, ...) (PRINT_MSG(stdout, WARNING_HEADER, format, ##__VA_ARGS__))

/**
 * Utility macros for manipulating bits in arrays
 * acting as bit containers
 */
#define INT_BIT sizeof(int) * CHAR_BIT

#define OR_MASK(array, index, mask) ((array)[(index)] |= (mask))

#define AND_MASK(array, index, mask) ((array)[(index)] &= (mask))

#define XOR_MASK(array, index, mask) ((array)[(index)] ^= (mask))

#define HAS_MASK(array, index, mask) ((array)[(index)] & (mask))

#define SET_BIT(array, bitNum, type) ({ \
	size_t typeBitSz = sizeof(type) * CHAR_BIT; \
	type mask = (type) 1 << ((bitNum) % typeBitSz); \
	OR_MASK((array), ((bitNum) / typeBitSz), mask); \
})

#define CLEAR_BIT(array, bitNum, type) ({ \
	size_t typeBitSz = sizeof(type) * CHAR_BIT; \
	type mask = (type) ~(1 << ((bitNum) % typeBitSz)); \
	AND_MASK((array), ((bitNum) / typeBitSz), mask); \
})

#define TOGGLE_BIT(array, bitNum, type) ({ \
	size_t typeBitSz = sizeof(type) * CHAR_BIT; \
	type mask = (type) 1 << ((bitNum) % typeBitSz); \
	XOR_MASK((array), (bitNum) / typeBitSz, mask); \
})

#define CHECK_BIT(array, bitNum, type) ({ \
	size_t typeBitSz = sizeof(type) * CHAR_BIT; \
	type mask = (type) 1 << ((bitNum) % typeBitSz); \
	HAS_MASK((array), (bitNum) / typeBitSz,  mask); \
})

#endif
