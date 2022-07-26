#include "validation.h"
// #include <stdio.h>
// int main() {
//     printf("%d", expression_validate("9-x"));
// }

int expression_validate(char* expression) {
    expression = remove_spaces(expression);
    char* actions = "+-*/^";
    char* trigonometry = "cstalx";
    int string_len = (int)strlen(expression);
    int left_bracket = 0, right_bracket = 0;
    int return_value = SUCCESS;
    if (!pre_check(expression, string_len, actions, trigonometry))
        return_value = ERROR;
    if (string_len > 255 || string_len < 3)
        return_value = ERROR;
    for (int i = 0; i < string_len; i++) {
        int offset = 0;
        if (expression[i] == '(') {
            left_bracket++;
            if (expression[i+1] == ')')
                return_value = ERROR;
        }
        if (expression[i] == ')')
            right_bracket++;
        if (expression[i] == '.' && (expression[i+1] <= '0' && expression[i+1] >= '9'))
            return_value = ERROR;
        if (expression[i] == '-' && i != 0) {
            if (expression[i-1] != '(' && expression[i-1] != ')' && expression[i-1] != 'x'
            && (expression[i-1] <= '0' && expression[i-1] >= '9'))
                return_value = ERROR;
        }
        if (expression[i] == 'm') {
            if (expression_processing("mod", expression, i, &offset)) {
                if ((expression[i-1] <= '0' && expression[i-1] >= '9')
                || (expression[i+3] <= '0' || expression[i+3] >= '9'))
                    return_value = ERROR;
            } else {
                return_value = ERROR;
            }
        }
        for (int j = 0; trigonometry[j] != '\0'; j++) {
            if (expression[i] == trigonometry[j]) {
                if (!triganometry_validate(i, expression, &offset))
                    return_value = ERROR;
                if (offset && expression[i + offset + 1] != '(')
                    return_value = ERROR;
            }
        }
        int count = 0;
        for (int j = 0; actions[j] != '\0'; j++) {
            if (expression[i] == ')' && expression[i+1] != actions[j]
            && expression[i+1] != '\0' && expression[i+1] != ')') {
                count++;
                if (count == (int)strlen(actions))
                    return_value = ERROR;
            }
            if (expression[string_len - 1] == actions[j])
                return_value = ERROR;
            int actions_counter = 0;
            if (expression[i] == actions[j]) {
                actions_counter++;
                int error_count = 0;
                for (int z = 0; trigonometry[z] != '\0'; z++) {
                    if ((expression[i+1] <= '0' && expression[i+1] >= '9')
                    && expression[i+1] != '(' && expression[i+1] != trigonometry[z])
                        error_count++;
                }
                if (error_count == (int)strlen(trigonometry))
                    return_value = ERROR;
                for (int k = 0; actions[k] != '\0'; k++) {
                    if (expression[i+1] == actions[k])
                        return_value = ERROR;
                }
            }
        }
        i += offset;
    }
    if (left_bracket != right_bracket)
        return_value = ERROR;
    free(expression);
    return return_value;
}

int pre_check(char* expression, int string_len, char* actions, char* trigonometry) {
    int action_check = 0;
    int trigonometry_check = 0;
    int return_value = SUCCESS;
    for (int i = 0; i < string_len; i++) {
        for (int a = 0; actions[a] != '\0'; a++) {
            if (actions[a] == expression[i])
                action_check++;
        }
        for (int t = 0; trigonometry[t] != '\0'; t++) {
            if (trigonometry[t] == expression[i] || expression[i] == 'm')
                trigonometry_check++;
        }
    }
    if (!trigonometry_check && !action_check)
        return_value = ERROR;
    return return_value;
}

int triganometry_validate(int counter, char* expression, int* offset) {
    int return_value = SUCCESS;
    int check = 0;
    switch (expression[counter]) {
    case 'c':
        check += expression_processing("cos", expression, counter, offset);
        break;
    case 's':
        if (expression[counter + 1] == 'i')
            check += expression_processing("sin", expression, counter, offset);
        else
            check += expression_processing("sqrt", expression, counter, offset);
        break;
    case 't':
        check += expression_processing("tan", expression, counter, offset);
        break;
    case 'a':
        if (expression[counter + 1] == 'c')
            check += expression_processing("acos", expression, counter, offset);
        else if (expression[counter + 1] == 's')
            check += expression_processing("asin", expression, counter, offset);
        else
            check += expression_processing("atan", expression, counter, offset);
        break;
    case 'l':
        if (expression[counter + 1] == 'n')
            check += expression_processing("ln", expression, counter, offset);
        else
            check += expression_processing("log", expression, counter, offset);
        break;
    case 'x':
        check += expression_processing("x", expression, counter, offset);
    }
    if (!check)
        return_value = ERROR;
    return return_value;
}

int expression_processing(char* trig_expression, char* expression, int counter, int* offset) {
    int return_value = ERROR;
    int check = 0;
    int string_len = (int)strlen(trig_expression);
    for (int i = counter, j = 0; i < counter + string_len; i++) {
        if (expression[i] == trig_expression[j++])
            check++;
    }
    if (check == string_len) {
        return_value = SUCCESS;
        *offset = string_len - 1;
    }
    return return_value;
}

char* remove_spaces(char* expression) {
    char* return_expression = calloc(strlen(expression), sizeof(char));
    for (int i = 0, j = 0; i < (int)strlen(expression); i++) {
        if (expression[i] != ' ')
            return_expression[j++] = expression[i];
    }
    return return_expression;
}

int axis_validation(long double xMax, long double xMin, long double yMin, long double yMax) {
    int return_value = SUCCESS;
    if (xMin >= xMax || yMin >= yMax)
        return_value = ERROR;
    return return_value;
}
