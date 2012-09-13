#ifndef BITSET_VECTOR_H_
#define BITSET_VECTOR_H_

#include "bitset/bitset.h"
#include "bitset/operation.h"
#include "bitset/estimate.h"

/**
 * Bitset buffers can be packed together into a vector which is compressed
 * using length encoding.  Each vector buffer consists of zero or more bitsets
 * prefixed with their offset and length
 *
 *    <offset1><length1><bitset_buffer1>...<offsetN><lengthN><bitset_bufferN>
 *
 * For example, adding a bitset with a length of 12 bytes at position 3
 * followed by a bitset with a length of 4 bytes at position 12 would result in
 *
 *    <offset=3><length=12><bitset1><offset=9><length=4><bitset2>
 *
 * Offsets and lengths are encoded using the following format
 *
 *    |00xxxxxx| 6 bit length
 *    |01xxxxxx|xxxxxxxx| 14 bit length
 *    |10xxxxxx|xxxxxxxx|xxxxxxxx| 22 bit length
 *    |11xxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx| 30 bit length
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Bitset vector types.
 */

typedef struct bitset_vector_s {
    char *buffer;
    size_t length;
    size_t size;
} bitset_vector_t;

typedef struct bitset_vector_operation_s bitset_vector_operation_t;

typedef struct bitset_vector_operation_step_s {
    union {
        bitset_vector_t *i;
        bitset_vector_operation_t *o;
    } data;
    void *userdata;
    bool is_nested;
    bool is_operation;
    enum bitset_operation_type type;
} bitset_vector_operation_step_t;

struct bitset_vector_operation_s {
    bitset_vector_operation_step_t **steps;
    unsigned min;
    unsigned max;
    size_t length;
};

#define BITSET_VECTOR_START 0
#define BITSET_VECTOR_END 0

/**
 * Create a new bitset vector.
 */

bitset_vector_t *bitset_vector_new();

/**
 * Create a new bitset vector based on an existing buffer.
 */

bitset_vector_t *bitset_vector_import(const char *, size_t);

/**
 * Free the specified vector.
 */

void bitset_vector_free(bitset_vector_t *);

/**
 * Copy a vector.
 */

bitset_vector_t *bitset_vector_copy(bitset_vector_t *);

/**
 * Get the vector buffer.
 */

char *bitset_vector_buffer(bitset_vector_t *);

/**
 * Get the byte length of the vector buffer.
 */

size_t bitset_vector_length(bitset_vector_t *);

/**
 * Get the number of bitsets in the vector.
 */

unsigned bitset_vector_count(bitset_vector_t *);

/**
 * Push a bitset on to the end of the vector.
 */

void bitset_vector_push(bitset_vector_t *, bitset_t *, unsigned);

/**
 * Resize the vector buffer.
 */

void bitset_vector_resize(bitset_vector_t *, size_t);

/**
 * Iterate over all bitsets.
 */

#define BITSET_VECTOR_FOREACH(iterator, bitset, offset) \
    for (unsigned BITSET_TMPVAR(i, __LINE__) = 0; \
         (BITSET_TMPVAR(i, __LINE__) < iterator->length \
            ? (offset = iterator->offsets[BITSET_TMPVAR(i, __LINE__)], \
               bitset = iterator->bitsets[BITSET_TMPVAR(i, __LINE__)], 1) : 0); \
         BITSET_TMPVAR(i, __LINE__)++)

/**
 * Concatenate an vector to another at the specified offset.
 */

void bitset_vector_concat(bitset_vector_t *, bitset_vector_t *, unsigned);

/**
 * Count bits in each bitset.
 */

void bitset_vector_count_bits(bitset_vector_t *, unsigned *, unsigned *);

/**
 * Merge (bitwise OR) each vector bitset.
 */

bitset_t *bitset_vector_merge(bitset_vector_t *);

/**
 * Create a new vector operation.
 */

bitset_vector_operation_t *bitset_vector_operation_new(bitset_vector_t *);

/**
 * Free the specified vector operation. By default vector operands will not be
 * freed. Use the second function before calling free() to free vectors.
 */

void bitset_vector_operation_free(bitset_vector_operation_t *);
void bitset_vector_operation_free_operands(bitset_vector_operation_t *);

/**
 * Add a vector to the operation.
 */

void bitset_vector_operation_add(bitset_vector_operation_t *,
    bitset_vector_t *, enum bitset_operation_type);

/**
 * Add a nested operation.
 */

void bitset_vector_operation_add_nested(bitset_vector_operation_t *,
    bitset_vector_operation_t *, enum bitset_operation_type);

/**
 * Execute the operation and return the result.
 */

bitset_vector_t *bitset_vector_operation_exec(bitset_vector_operation_t *);

/**
 * Provide a way to associate user data with each step and use the data to lazily
 * lookup vectors.
 */

void bitset_vector_operation_add_data(bitset_vector_operation_t *,
    void *data, enum bitset_operation_type);

void bitset_vector_operation_resolve_data(bitset_vector_operation_t *,
        bitset_vector_t *(*)(void *data, void *context), void *context);

void bitset_vector_operation_free_data(bitset_vector_operation_t *, void (*)(void *data));

#ifdef __cplusplus
} //extern "C"
#endif

#endif

