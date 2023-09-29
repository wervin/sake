#ifndef SAKE_STRING_H
#define SAKE_STRING_H

#include <stdint.h>

/*
 *  +------+----------+------+
 *  | size | capacity | data |
 *  +------+----------+------+
 */

typedef char * sake_string;

sake_string sake_string_new(const char *string);
void sake_string_free(sake_string string);


#endif /* SAKE_STRING_H */