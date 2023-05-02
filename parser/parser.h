#pragma once
#include <iostream>
#include <exception>
#include <memory>
#include <unordered_map>
#include <vector>
#include <list>
#include <string>
#include <cctype>
#include "data_expression.h"
#include <variant>

using expression = data_expression;

inline bool is_text(const std::string& str) noexcept {
    for (auto it = str.cbegin(); it != str.end(); ++it) {
        if (!std::isalpha(*it)) return false;
    }
    return true;
}

inline bool is_number(const std::string& str) noexcept {
    for (auto it = str.cbegin(); it < str.end(); ++it) {
        if (!std::isdigit(*it)) return false;
    }
    return true;
}

inline bool is_operator(char ch) {
    if (ch == '+' || ch == '-' || ch == '*' || ch == '/')
        return true;
    return false;
}

type_operand convert_arg
(std::string::const_iterator& it, std::string::const_iterator end,
    const std::vector<std::vector<std::string>>& initial_data) {
    std::string key_cell;
    std::string temp;

    while (it != end && isalpha(*it)) {
        temp.push_back(*it);
        ++it;
    }
    if (!isdigit(*it) || it == end) return ERROR_VALUE;
    bool find_res = false;
    for (size_t index_row = 0; index_row < initial_data[0].size(); ++index_row) {
        if (initial_data[0][index_row] == temp) {
            find_res = true;
            break;
        }
    }
    if (!find_res) return ERROR_VALUE;
    key_cell = temp;
    temp.erase();

    while (it != end && isdigit(*it)) {
        temp.push_back(*it);
        ++it;
    }

    if (is_number(temp) && key_cell.empty()) return std::stol(temp);
    //find name row
    for (size_t index_column = 0; index_column < initial_data.size(); ++index_column)
    {
        if (initial_data[index_column][0] == temp) {
            key_cell += temp;
            return key_cell;
        }
    }
    return ERROR_VALUE;
}

operator_expr convert_operator(std::string::const_iterator& it) {
    
    if (*it == '+') {
        return ADD;
    }
    if (*it == '-') {
        return SUBTRACT;
    }
    if (*it == '*') {
        return MYLTYPLY;
    }
    return DIVIDE;
}

value_cell convert_to_expression
(const std::string& str, const std::vector<std::vector<std::string>>& initial_data) {
    try {
        auto it = str.begin();
        if (!(*it == '=')) return ERROR_VALUE;
        else ++it;

        type_operand left = std::move(convert_arg(it, str.cend(), initial_data));
        if (std::holds_alternative<int32_t>(left)
            && std::get<int32_t>(left) == ERROR_VALUE) return ERROR_VALUE;
        if (!is_operator(*it) && it != str.end()) return ERROR_VALUE;

        operator_expr op = std::move(convert_operator(it));

        type_operand right = std::move(convert_arg(it, str.cend(), initial_data));
        if (it != str.end()) return ERROR_VALUE;
        if (std::holds_alternative<int32_t>(right)
            && std::get<int32_t>(right) == ERROR_VALUE) return ERROR_VALUE;

        expression temp(left, op, right);
        return temp;
    }
    catch (...) {
        return ERROR_VALUE;
    }

}

void play_convert(const std::vector<std::vector<std::string>>& initial_data,
    std::unordered_map<std::string, value_cell>& result) {

    for (size_t index_column = 1;
        index_column < initial_data.size(); ++index_column) {

        for (size_t index_string = 1; index_string < initial_data[index_column].size();
            ++index_string) {
            std::string key_cell =
                initial_data[0][index_string] + initial_data[index_column][0];
            if (!is_text(initial_data[0][index_string])
                || !is_number(initial_data[index_column][0])) {
                result[key_cell] = ERROR_VALUE;
            }
            else {
                value_cell temp_expr;
                if (is_number(initial_data[index_column][index_string]))
                    temp_expr = std::stol(initial_data[index_column][index_string]);
                else temp_expr =
                    convert_to_expression(initial_data[index_column][index_string], initial_data);
                    result[key_cell] = temp_expr;
            }
            
        }

    }

    return;
}


//реализация в срр
//поместить в namespace
