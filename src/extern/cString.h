#ifndef cString_h
#define cString_h

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define cstring_length(obj) cstring_size(obj)

/**
 * @brief A dynamic heap-allocated string object.
 *
 * @param data  Pointer to the null-terminated character buffer.
 * @param len   Number of characters in the string, excluding the null terminator.
 * @param cap   Allocated buffer size in bytes, always greater than len.
 *
 * @note Should only be created via cstring_init() and freed via cstring_free().
 * @note Direct modification of struct members is discouraged.
 */

typedef struct cString {
    char *data;
    size_t len;
    size_t cap; // Oguri Cap
} cString;


/**
 * @brief cString constructor.
 *
 * @param str  Initial string. Must not be NULL.
 *
 * @note Returns NULL on NULL input.
 * @note The intended way for the object to be created.
 *
 * @example
 *   cString *s = cstring_init("Oguri");
 *   // cString object created!!!
 *   // s->data == "Oguri"
 *   // s->len  == 5
 *   // s->cap  == 16 
 */

cString *cstring_init(const char *str);

/**
 * @brief cString deep copy.
 *
 * @param dst  Destination cString to copy to. Must not be NULL
 * @param src  Target cString to copy from. Must not be NULL.
 *
 * @note Silently returns on NULL input.
 *
 * @example
 *   cString *s1 = cstring_init("Oguri"), *s2 = cstring_init("");
 *   cstring_copy(s2, s1);
 *   // s1 == s2
 */

void cstring_copy(cString *dst, const cString *src);

/**
 * @brief cString deep clone.
 *
 * @param src  Target cString to clone from. Must not be NULL.
 *
 * @note Returns NULL on NULL input.
 *
 * @example
 *   cString *s1 = cstring_init("Oguri");
 *   cString *s2 = cstring_clone(s1);
 *   // s1 == s2
 */

cString *cstring_clone(const cString *src);

/**
 * @brief Memory deallocator for cString object.
 *
 * @param obj  The cString. Must not be NULL.
 *
 * @note Silently returns on NULL input.
 * @note Always call this when done with a cString to avoid memory leaks.
 *
 * @example
 *   cString *s = cstring_init("Oguri");
 *   cstring_free(s);
 *   // Memory deallocated!!!
 */

void cstring_free(cString *obj);

/**
 * @brief Getter for cString->len.
 *
 * @param obj  The cString. Must not be NULL.
 *
 * @note Returns 0 on NULL input.
 * @note Identical to cstring_length(). Both exist for convenience.
 *
 * @example
 *   cString *s = cstring_init("Oguri");
 *   // cstring_size(s) == 5
 */

static inline size_t cstring_size(const cString *obj){
    if(!obj) return 0;
    return obj->len;
}

/**
 * @brief Getter for the beginning pointer.
 *
 * @param obj  The cString. Must not be NULL.
 *
 * @note Returns NULL on NULL input.
 *
 * @example
 *   cString *s = cstring_init("Oguri");
 *   char *x = cstring_begin(s);
 *   // x == s->data
 */

static inline char *cstring_begin(cString *obj){
    if(!obj) return NULL;
    return obj->data;
}

/**
 * @brief Getter for the ending pointer.
 *
 * @param obj  The cString. Must not be NULL.
 *
 * @note Returns NULL on NULL input.
 *
 * @example
 *   cString *s = cstring_init("Oguri");
 *   char *x = cstring_end(s);
 *   // x == s->data + s->len ('\0')
 */

static inline char *cstring_end(cString *obj){
    if(!obj) return NULL;
    return obj->data + obj->len;
}

/**
 * @brief Getter for is_empty.
 *
 * @param obj  The cString. Must not be NULL.
 *
 * @note Returns -1 on NULL input.
 *
 * @example
 *   cString *s = cstring_init("Oguri");
 *   // cstring_empty(s) == 0
 */

static inline int cstring_empty(cString *obj){
    if(!obj) return -1;
    return obj->len == 0 ? 1 : 0;
}

/**
 * @brief Clears cString buffer
 *
 * @param obj  The cString. Must not be NULL.
 *
 * @note Silently returns on NULL input.
 *
 * @example
 *   cString *s = cstring_init("Oguri");
 *   cstring_clear(s);
 *   // s->len == 0
 *   // *(s->data) == '\0'
 */

static inline void cstring_clear(cString *obj){
    if(!obj) return;
    obj->len = 0;
    obj->data[0] = '\0';
}

/**
 * @brief Getter for cString->data[index].
 *
 * @param obj  The cString. Must not be NULL.
 * @param idx  The index of cString->data. Must not exceed cString->len.
 *
 * @note Returns '\0' on invalid index or NULL input.
 *
 * @example
 *   cString *s = cstring_init("Oguri");
 *   // cstring_at(s, 2) == 'u'
 */

static inline char cstring_at(const cString *obj, size_t idx){
    if(!obj || !(idx < obj->len)) return '\0';
    return obj->data[idx];
}

/**
 * @brief Appends a char array to the end of a cString object.
 *
 * @param obj  The cString to append to. Must not be NULL.
 * @param str  The string to append. Must not be NULL.
 *
 * @note Silently returns on NULL input or empty string.
 * @note Capacity is doubled when exceeded, with a minimum of 16 bytes.
 *
 * @example
 *   cString *s = cstring_init("Oguri");
 *   cstring_push_back(s, " Cap");
 *   // s->data == "Oguri Cap"
 */

void cstring_push_back(cString *obj, const char *str);

/**
 * @brief Prepends a char array to a cString object.
 *
 * @param obj  The cString to append to. Must not be NULL.
 * @param str  The string to append. Must not be NULL.
 *
 * @note Silently returns on NULL input or empty string.
 * @note Capacity is doubled when exceeded, with a minimum of 16 bytes.
 *
 * @example
 *   cString *s = cstring_init("Oguri");
 *   cstring_push_front(s, "Cap ");
 *   // s->data == "Cap Oguri"
 */

void cstring_push_front(cString *obj, const char *str);

/**
 * @brief Inserts a char array into a cString object at a given index.
 *
 * @param obj  The cString to append to. Must not be NULL.
 * @param str  The string to append. Must not be NULL.
 * @param idx  The index position to insert.
 *
 * @note Returns the pointer of the inserted position.
 *
 * @example
 *   cString *s = cstring_init("Oguri Cap");
 *   cstring_insert(s, "Fat ", 6);
 *   // s->data == "Oguri Fat Cap"
 */

char *cstring_insert(cString *obj, const char *str, size_t idx);

/**
 * @brief Remove n elements from the back.
 *
 * @param obj  The cString to remove. Must not be NULL.
 * @param n    The number of elements to remove.
 *
 * @example
 *   cString *s = cstring_init("Oguri Cap");
 *   cstring_pop_back(s, 4);
 *   // s->data == "Oguri"
 */

void cstring_pop_back(cString *obj, size_t n);

/**
 * @brief Remove n elements from the front.
 *
 * @param obj  The cString to remove. Must not be NULL.
 * @param n    The number of elements to remove.
 *
 * @example
 *   cString *s = cstring_init("Oguri Cap");
 *   cstring_pop_front(s, 6);
 *   // s->data == "Cap"
 */

void cstring_pop_front(cString *obj, size_t n);

/**
 * @brief Remove n elements from a specific position.
 *
 * @param obj  The cString to remove. Must not be NULL.
 * @param idx  The index position to erase from. 
 * @param n    The number of elements to remove. If n exceeds the number of
 *             elements remaining from idx to the end, it is clamped: every
 *             character from idx onward is removed.
 *
 * @note If idx >= cstring_size(obj), this is a no-op and returns the end
 *       pointer (obj->data + obj->len).
 *
 * @example
 *   cString *s = cstring_init("Oguri Fat Cap");
 *   cstring_erase(s, 5, 4);
 *   // s->data == "Oguri Cap"
 *
 *   cString *s2 = cstring_init("Oguri Cap");
 *   cstring_erase(s2, 5, 999);
 *   // s2->data == "Oguri"
 */

char *cstring_erase(cString *obj, size_t idx, size_t n);

/**
 * @brief Compares two cString objects lexicographically.
 *
 * @param a  First cString. Must not be NULL.
 * @param b  Second cString. Must not be NULL.
 *
 * @note Returns 0 on equal, -1 on a < b, 1 on a > b (lexicographically)
 * @note NULL input(s) are undefined behavior (unchecked precondition).
 *
 * @example
 *   cString *a = cstring_init("Oguri");
 *   cString *b = cstring_init("Cap");
 *   cString *c = cstring_init("Oguri");
 *   // cstring_compare(a, b) == 1
 *   // cstring_compare(a, c) == 0
 */

int cstring_compare(const cString *a, const cString *b);

/**
 * @brief Checks if two cString objects are equal.
 *
 * @param a  First cString. Must not be NULL.
 * @param b  Second cString. Must not be NULL.
 *
 * @note Returns 1 on equal, else returns 0
 * @note NULL input(s) are undefined behavior (unchecked precondition).
 *
 * @example
 *   cString *a = cstring_init("Oguri");
 *   cString *b = cstring_init("Oguri");
 *   // cstring_equals(a, b) == 1
 */

int cstring_equals(const cString *a, const cString *b);

/**
 * @brief Fit allocated memory to string length
 *
 * @param obj  The cString. Must not be NULL.
 *
 * @note Silently returns on NULL input.
 * @note Use when array will not be changed any further for maximum efficiency.
 *
 * @example
 *   cString *s = cstring_init("Oguri");
 *   cstring_push_back(s, " Cap is so fat");
 *   cstring_fit(s);
 *   // s->cap == 20 instead of 32
 */

void cstring_fit(cString *obj);

/**
 * @brief Finds the first occurrence of a substring in a cString object.
 *
 * @param obj  The cString to search in. Must not be NULL.
 * @param str  The substring to search for. Must not be NULL.
 *
 * @note Returns NULL on NULL input or if substring is not found.
 *
 * @example
 *   cString *s = cstring_init("Oguri Cap");
 *   char *p = cstring_find(s, "Cap");
 *   // p == s->data + 6
 */

char *cstring_find(const cString *obj, const char *str);

/**
 * @brief Counts non-overlapping occurrences of a substring in a cString object.
 *
 * @param obj  The cString to search in. Must not be NULL.
 * @param str  The substring to count. Must not be NULL.
 *
 * @note Returns 0 on NULL input or empty substring.
 * @note "Non-overlapping"
 *
 * @example
 *   cString *s = cstring_init("Oguri Cap Oguri");
 *   // cstring_count(s, "Oguri") == 2
 */

size_t cstring_count(const cString *obj, const char *str);

/**
 * @brief Replaces all non-overlapping occurrences of a substring.
 *
 * @param obj     The cString to modify. Must not be NULL.
 * @param target  The substring to search for. Must not be NULL or empty.
 * @param str     The replacement string. Must not be NULL.
 *
 * @note Returns 0 on NULL input, empty target, or if no occurrences were found.
 * @note "Non-overlapping", matched left to right.
 *
 * @example
 *   cString *s = cstring_init("Hi, I love {name}!!! {name} is very fat!!!");
 *   cstring_replace(s, "{name}", "Oguri Cap");
 *   // s->data == "Hi, I love Oguri Cap!!! Oguri Cap is very fat!!!"
 */

size_t cstring_replace(cString *obj, const char *target, const char *str);

/**
 * @brief Ensures capacity for at least n characters (plus the null terminator).
 *
 * @param obj  The cString. Must not be NULL.
 * @param n    The number of characters to reserve room for.
 *
 * @note Silently returns on NULL input or allocation failure.
 * @note Never shrinks. Use cstring_fit() to shrink.
 * @note Reserves exactly n + 1 bytes, bypassing the doubling growth policy.
 *
 * @example
 *   cString *s = cstring_init("Oguri");
 *   cstring_reserve(s, 100);
 *   // s->cap == 101, future appends up to 100 chars won't reallocate
 */

void cstring_reserve(cString *obj, size_t n);

/**
 * @brief Extracts a substring into a newly allocated cString.
 *
 * @param obj  The cString to extract from. Must not be NULL.
 * @param idx  The starting index. Clamped to cstring_size(obj) if out of range.
 * @param n    The number of characters to extract. Clamped to the number of
 *             characters remaining from idx to the end.
 *
 * @note Returns NULL on NULL input or allocation failure.
 * @note The returned cString must be freed with cstring_free().
 *
 * @example
 *   cString *s = cstring_init("Oguri Cap");
 *   cString *sub = cstring_substr(s, 6, 3);
 *   // sub->data == "Cap"
 */

cString *cstring_substr(const cString *obj, size_t idx, size_t n);

/**
 * @brief Removes leading and trailing whitespace in place.
 *
 * @param obj  The cString. Must not be NULL.
 *
 * @note Silently returns on NULL input.
 * @note Whitespace is defined by isspace(): ' ', '\\t', '\\n', '\\v', '\\f', '\\r'.
 *
 * @example
 *   cString *s = cstring_init("  Oguri Cap \n");
 *   cstring_trim(s);
 *   // s->data == "Oguri Cap"
 */

void cstring_trim(cString *obj);

/**
 * @brief Converts the string to uppercase in place.
 *
 * @param obj  The cString. Must not be NULL.
 *
 * @note Silently returns on NULL input.
 *
 * @example
 *   cString *s = cstring_init("Oguri Cap");
 *   cstring_to_upper(s);
 *   // s->data == "OGURI CAP"
 */

void cstring_to_upper(cString *obj);

/**
 * @brief Converts the string to lowercase in place.
 *
 * @param obj  The cString. Must not be NULL.
 *
 * @note Silently returns on NULL input.
 *
 * @example
 *   cString *s = cstring_init("Oguri Cap");
 *   cstring_to_lower(s);
 *   // s->data == "oguri cap"
 */

void cstring_to_lower(cString *obj);

/**
 * @brief Checks if a cString starts with a given prefix.
 *
 * @param obj  The cString to check. Must not be NULL.
 * @param str  The prefix to test for. Must not be NULL.
 *
 * @note Returns 1 if obj starts with str, else 0. Returns 0 on NULL input.
 * @note An empty prefix always returns 1.
 *
 * @example
 *   cString *s = cstring_init("Oguri Cap");
 *   // cstring_starts_with(s, "Oguri") == 1
 *   // cstring_starts_with(s, "Cap") == 0
 */

int cstring_starts_with(const cString *obj, const char *str);

/**
 * @brief Checks if a cString ends with a given suffix.
 *
 * @param obj  The cString to check. Must not be NULL.
 * @param str  The suffix to test for. Must not be NULL.
 *
 * @note Returns 1 if obj ends with str, else 0. Returns 0 on NULL input.
 * @note An empty suffix always returns 1.
 *
 * @example
 *   cString *s = cstring_init("Oguri Cap");
 *   // cstring_ends_with(s, "Cap") == 1
 *   // cstring_ends_with(s, "Oguri") == 0
 */

int cstring_ends_with(const cString *obj, const char *str);

/**
 * @brief Reverses the string in place.
 *
 * @param obj  The cString. Must not be NULL.
 *
 * @note Silently returns on NULL input.
 *
 * @example
 *   cString *s = cstring_init("Oguri");
 *   cstring_reverse(s);
 *   // s->data == "irugO"
 */

void cstring_reverse(cString *obj);

#ifdef __cplusplus
}
#endif

#endif
