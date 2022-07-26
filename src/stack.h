#ifndef SRC_STACK_H_
#define SRC_STACK_H_

#include <stdio.h>
#include <math.h>

typedef enum type {
    X,
    NUMBER,
    PLUS,
    SUB,
    MUL,
    DIV,
    MOD,
    DEGREE,
    SIN,
    COS,
    TAN,
    ACOS,
    ASIN,
    ATAN,
    SQRT,
    LN,
    LOG,
    LEFT_BRACKET,
    RIGHT_BRACKET
} type_t;

typedef struct stack_struct {
    long double value;
    int priority;
    type_t type;
    struct stack_struct* next;
} stack;

int qt_entry(char* expression, long double x_value, long double *result_value);
void next_pop(stack **last_element);
void reverse_polish_notation(stack** first_element, stack** rpn_numbers, stack** rpn_support);
void destroy(stack* first_element);
void pop(stack **last_element);
void reverse_stack(stack **first_element, stack **answer);
int get_number(char* expression, char* single_number, int* i);
void push(long double value, int priority, type_t type, stack **first_element);
void fill_stack(char* expression, int* i, stack **first_element, long double x_value);
void calculation(stack** result, stack** calculated_stack);
void double_pop(stack **last_element);

#endif
