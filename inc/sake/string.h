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

sake_string sake_string_new(const char *string);
sake_string sake_string_new_range(const char *begin, const char *end);
void sake_string_free(sake_string string);
uint32_t sake_string_raw_size(sake_string string);
uint32_t sake_string_utf8_size(sake_string string);
bool sake_string_empty(sake_string string);
uint32_t sake_string_at(sake_string string, uint32_t index);
sake_string sake_string_push_back(sake_string string, const char *data);
void sake_string_pop_back(sake_string string);
void sake_string_erase(sake_string string, uint32_t index);
void sake_string_erase_range(sake_string string, uint32_t from, uint32_t to);
sake_string sake_string_insert(sake_string string, uint32_t index, const char *data);

#endif /* SAKE_STRING_H */