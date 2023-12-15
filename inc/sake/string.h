#ifndef SAKE_STRING_H
#define SAKE_STRING_H

#include <stdint.h>
#include <stdbool.h>

/*
 *  +------+----------+------+
 *  | size | capacity | data |
 *  +------+----------+------+
 */

typedef char *sake_string;

sake_string sake_string_new(const char *begin, const char *end);
void sake_string_free(sake_string string);
uint32_t sake_string_capacity(sake_string string);
uint32_t sake_string_raw_size(sake_string string);
uint32_t sake_string_utf8_size(sake_string string);
bool sake_string_empty(sake_string string);
sake_string sake_string_push_back(sake_string string, const char *begin, const char *end);
void sake_string_pop_back(sake_string string);
void sake_string_erase(sake_string string, uint32_t index);
void sake_string_erase_range(sake_string string, uint32_t from, uint32_t to);
sake_string sake_string_insert(sake_string string, uint32_t index, const char *begin, const char *end);
uint32_t sake_string_raw_index(sake_string string, uint32_t utf8_index);
uint32_t sake_string_utf8_index(sake_string string, uint32_t raw_index);

#endif /* SAKE_STRING_H */