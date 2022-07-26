#ifndef SRC_VALIDATION_H_
#define SRC_VALIDATION_H_

#include <string.h>
#include <stdlib.h>

enum exit_code {
    ERROR,
    SUCCESS
};

int acos_asin_value_checker(char* expression, int offset);
int axis_validation(long double xMax, long double xMin, long double yMin, long double yMax);
int pre_check(char* expression, int string_len, char* actions, char* trigonometry);
int expression_validate(char* expression);
int triganometry_validate(int counter, char* expression, int* offset);
int expression_processing(char* trig_expression, char* expression, int counter, int* offset);
char* remove_spaces(char* expression);

#endif
