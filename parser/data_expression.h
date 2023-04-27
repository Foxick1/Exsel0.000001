#pragma once
#include <iostream>
#include <exception>
#include <memory>
#include <map>
#include <vector>
#include <string>
#include <cctype>

static const int32_t ERROR_VALUE = -1;

using type_operand = std::variant<std::string, int32_t>;

enum operator_expr { ADD = '+', SUBTRACT = '-', MYLTYPLY = '*', DIVIDE = '/' } ; // operator expression
    
struct data_expression {
    type_operand left;   // the left operand from the cell expression
    operator_expr operator_exp;
    type_operand right;  // the right operand from the cell expression
    int32_t result_expression = ERROR_VALUE;  //result cell expression
    bool is_sumoned= false;

    data_expression(type_operand left, operator_expr op, type_operand right)
        :left(left), operator_exp(op), right(right) {};

    using value_cell = std::variant<int32_t, data_expression>;

    int32_t calculate(value_cell& cell, std::map<std::string, value_cell>& result) {
        if (std::holds_alternative<int32_t>(cell)) return std::get<int32_t>(cell);
        data_expression temp = std::get<data_expression>(cell);
        if (temp.is_sumoned) return ERROR_VALUE;
        temp.is_sumoned = true;

        int32_t left;
        int32_t right;
        if (std::holds_alternative<int32_t>(temp.left)) left = std::get<int32_t>(temp.left);
        else  left = calculate (result[std::get<std::string>(temp.left)], result);
        if (std::holds_alternative<int32_t>(temp.right)) right = std::get<int32_t>(temp.right);
        else right = calculate(result[std::get<std::string>(temp.right)], result);

        switch (operator_exp) {
        case ADD:
            if(left != ERROR_VALUE && right != ERROR_VALUE) result_expression = left + right;
            cell = result_expression;
            return result_expression;
        case SUBTRACT:
            if (left != ERROR_VALUE && right != ERROR_VALUE) result_expression = left - right;
            cell.emplace<int32_t>(result_expression);
            return result_expression;
        case MYLTYPLY:
            if (left != ERROR_VALUE && right != ERROR_VALUE) result_expression = left * right;
            cell = result_expression;
            return result_expression;
        case DIVIDE:
            if(right != 0 && left != ERROR_VALUE && right != ERROR_VALUE) 
                result_expression = left / right;
            cell = result_expression;
            return result_expression;
        }
    }

};
