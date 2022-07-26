#include "stack.h"
#include "validation.h"

int qt_entry(char* expression, long double x_value, long double *result_value) {
    int return_value = ERROR;
    if (expression_validate(expression)) {
        return_value = SUCCESS;
        stack* reversed_first_element = NULL;
        for (int i = 0; expression[i] != '\0'; i++)
            fill_stack(expression, &i, &reversed_first_element, x_value);
        stack* first_element = NULL;
        reverse_stack(&reversed_first_element, &first_element);
        stack* rpn_numbers = NULL;
        stack* rpn_support = NULL;
        reverse_polish_notation(&first_element, &rpn_numbers, &rpn_support);
        stack* result = NULL;
        reverse_stack(&rpn_numbers, &result);
        stack* calculated_stack = NULL;
        calculation(&result, &calculated_stack);
        *result_value = calculated_stack->value;
        pop(&calculated_stack);
    }
    return return_value;
}

void calculation(stack** result, stack** calculated_stack) {
    int number_checker = 0;
    int check = 0;
    while (1) {
        switch ((*result)->type) {
        case PLUS:
            pop(result);
            push((*calculated_stack)->next->value + (*calculated_stack)->value, 0, NUMBER, calculated_stack);
            double_pop(calculated_stack);
            break;
        case SUB:
            pop(result);
            push((*calculated_stack)->next->value - (*calculated_stack)->value, 0, NUMBER, calculated_stack);
            double_pop(calculated_stack);
            break;
        case MUL:
            pop(result);
            push((*calculated_stack)->next->value * (*calculated_stack)->value, 0, NUMBER, calculated_stack);
            double_pop(calculated_stack);
            break;
        case DIV:
            pop(result);
            push((*calculated_stack)->next->value / (*calculated_stack)->value, 0, NUMBER, calculated_stack);
            double_pop(calculated_stack);
            break;
        case MOD:
            pop(result);
            push(fmod((*calculated_stack)->next->value,
            (*calculated_stack)->value), 0, NUMBER, calculated_stack);
            double_pop(calculated_stack);
            break;
        case DEGREE:
            pop(result);
            push(pow((*calculated_stack)->next->value,
            (*calculated_stack)->value), 0, NUMBER, calculated_stack);
            double_pop(calculated_stack);
            break;
        case SIN:
            pop(result);
            push(sin((*calculated_stack)->value), 0, NUMBER, calculated_stack);
            next_pop(calculated_stack);
            break;
        case COS:
            pop(result);
            push(cos((*calculated_stack)->value), 0, NUMBER, calculated_stack);
            next_pop(calculated_stack);
            break;
        case TAN:
            pop(result);
            push(tan((*calculated_stack)->value), 0, NUMBER, calculated_stack);
            next_pop(calculated_stack);
            break;
        case ACOS:
            pop(result);
            push(acos((*calculated_stack)->value), 0, NUMBER, calculated_stack);
            next_pop(calculated_stack);
            break;
        case ASIN:
            pop(result);
            push(asin((*calculated_stack)->value), 0, NUMBER, calculated_stack);
            next_pop(calculated_stack);
            break;
        case ATAN:
            pop(result);
            push(atan((*calculated_stack)->value), 0, NUMBER, calculated_stack);
            next_pop(calculated_stack);
            break;
        case SQRT:
            pop(result);
            push(sqrt((*calculated_stack)->value), 0, NUMBER, calculated_stack);
            next_pop(calculated_stack);
            break;
        case LN:
            pop(result);
            push(log((*calculated_stack)->value), 0, NUMBER, calculated_stack);
            next_pop(calculated_stack);
            break;
        case LOG:
            pop(result);
            push(log10((*calculated_stack)->value), 0, NUMBER, calculated_stack);
            next_pop(calculated_stack);
            break;
        default:
            break;
        }
        if (*result) {
            if ((*result)->type == NUMBER || (*result)->type == X) {
                number_checker++;
                push((*result)->value, (*result)->priority, (*result)->type, calculated_stack);
                pop(result);
            }
        }
        if (!*result || check)
            break;
        if (!(*result)->next)
            check++;
    }
}

void next_pop(stack **last_element) {
    stack *tmp = NULL;
    tmp = (*last_element)->next->next;
    free((*last_element)->next);
    (*last_element)->next = tmp;
}

void double_pop(stack **last_element) {
    stack *tmp = NULL;
    tmp = (*last_element)->next->next->next;
    free((*last_element)->next->next);
    free((*last_element)->next);
    (*last_element)->next = tmp;
}

void reverse_polish_notation(stack** first_element, stack** rpn_numbers, stack** rpn_support) {
    int check = 0;
    while (1) {
            if ((*first_element)) {
                if ((*first_element)->type == RIGHT_BRACKET) {
                    pop(first_element);
                    while ((*rpn_support)->type != LEFT_BRACKET) {
                        push((*rpn_support)->value, (*rpn_support)->priority,
                        (*rpn_support)->type, rpn_numbers);
                        pop(rpn_support);
                    }
                    pop(rpn_support);
                } else {
                    if ((*first_element)->type == NUMBER || (*first_element)->type == X) {
                        push((*first_element)->value, (*first_element)->priority,
                        (*first_element)->type, rpn_numbers);
                        pop(first_element);
                    } else {
                        if (*rpn_support) {
                            if ((*first_element)->priority != -1
                            && (*first_element)->priority <= (*rpn_support)->priority) {
                                push((*rpn_support)->value, (*rpn_support)->priority,
                                (*rpn_support)->type, rpn_numbers);
                                pop(rpn_support);
                            }
                        }
                        push((*first_element)->value, (*first_element)->priority,
                        (*first_element)->type, rpn_support);
                        pop(first_element);
                    }
                }
            }
            if (!*first_element || check)
                break;
            if (!(*first_element)->next)
                check++;
    }
    check = 0;
    if (*rpn_support) {
        while (1) {
            push((*rpn_support)->value, (*rpn_support)->priority, (*rpn_support)->type, rpn_numbers);
            pop(rpn_support);
            if (check || !*rpn_support)
                break;
            if (!(*rpn_support)->next)
                check++;
        }
    }
}

void fill_stack(char* expression, int* i, stack **first_element, long double x_value) {
    char single_number[256] = {'\0'};
    if (expression[*i] >= '0' && expression[*i] <= '9') {
        get_number(expression, single_number, i);
        long double pure_number = 0;
        sscanf(single_number, "%Lf", &pure_number);
        push(pure_number, 0, NUMBER, first_element);
    }
    switch (expression[*i]) {
        case 'X':
        case 'x':
            push(x_value, 0, X, first_element);
            break;
        case '(':
            push(0, -1, LEFT_BRACKET, first_element);
            break;
        case ')':
            push(0, -1, RIGHT_BRACKET, first_element);
            break;
        case '+':
            if (!*i || (*first_element)->type == LEFT_BRACKET)
                push(0, 0, NUMBER, first_element);
            push(0, 1, PLUS, first_element);
            break;
        case '-':
            if (!*i || (*first_element)->type == LEFT_BRACKET)
                push(0, 0, NUMBER, first_element);
            push(0, 1, SUB, first_element);
            break;
        case '*':
            push(0, 2, MUL, first_element);
            break;
        case '/':
            push(0, 2, DIV, first_element);
            break;
        case 'm':
            push(0, 2, MOD, first_element);
            *i += 2;
            break;
        case '^':
            push(0, 3, DEGREE, first_element);
            break;
        case 'c':
            push(0, 4, COS, first_element);
            *i += 2;
            break;
        case 's':
            if (expression[*i + 1] == 'i') {
                push(0, 4, SIN, first_element);
                *i += 2;
            } else {
                push(0, 4, SQRT, first_element);
                *i += 3;
            }
            break;
        case 't':
                push(0, 4, TAN, first_element);
                *i += 2;
            break;
        case 'a':
            if (expression[*i + 1] == 'c') {
                push(0, 4, ACOS, first_element);
                *i += 3;
            } else if (expression[*i + 1] == 's') {
                push(0, 4, ASIN, first_element);
                *i += 3;
            } else {
                push(0, 4, ATAN, first_element);
                *i += 3;
            }
            break;
        case 'l':
            if (expression[*i + 1] == 'n') {
                push(0, 4, LN, first_element);
                *i += 1;
            } else {
                push(0, 4, LOG, first_element);
                *i += 2;
            }
            break;
        }
}

void reverse_stack(stack **first_element, stack **answer) {
    int check = 0;
    while (1) {
        push((*first_element)->value, (*first_element)->priority, (*first_element)->type, answer);
        pop(first_element);
        if (check)
            break;
        if (!(*first_element)->next)
            check++;
    }
}

void push(long double value, int priority, type_t type, stack **first_element) {
    stack *tmp = malloc(sizeof(stack));
    tmp->value = value;
    tmp->priority = priority;
    tmp->next = *first_element;
    tmp->type = type;
    *first_element = tmp;
}

void pop(stack **last_element) {
    stack *tmp = NULL;
    tmp = (*last_element)->next;
    free(*last_element);
    (*last_element) = tmp;
}

int get_number(char* expression, char* single_number, int* i) {
    int j = 0;
    int check = 0;
    int return_value = 0;
    while (expression[*i] != '\0') {
        if ((expression[*i] >= '0' && expression[*i] <= '9') || expression[*i] == '.') {
            single_number[j++] = expression[*i];
            if ((expression[*i+1] < '0' || expression[*i+1] > '9') && expression[*i+1] != '.')
                check++;
        }
        if (check)
            break;
        (*i)++;
    }
    if (expression[*i] == '\0')
        return_value++;
    return return_value;
}
