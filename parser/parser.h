#pragma once
#include <iostream>
#include <exception>
#include <memory>
#include <map>
#include <vector>
#include <list>
#include <string>
#include <cctype>
#include "data_expression.h"
#include <variant>

static const int32_t ERROR_VALUE = -1;

using expression = data_expression;
using value_cell = std::variant<int32_t, expression>;
 

class Parser {
private:
    static bool is_out_of_range_uint32_t(const std::string str) {
        try {
            if (std::stoul(str) > -2) return true;
            return false;
        }
        catch (std::out_of_range) {
            std::out_of_range int_diapason("Out of range uint32_t diapason");
            throw int_diapason;
        }
    }
    static bool is_number(const std::string& str) {
        for (auto it = str.cbegin(); it < str.end(); ++it) {
            if (!std::isdigit(*it)) return false;
        }
        is_out_of_range_uint32_t(str);
        return true;
    }
    static bool is_operator(char ch) {
        if (ch == '+' || ch == '-' || ch == '*' || ch == '/')
            return true;
        return false;
    }
    static type_operand convert_left_arg
    (std::string::const_iterator& it, std::string::const_iterator end) {
        std::string temp;

        while (isalpha(*it) && it != end) {
            temp.push_back(*it);
            ++it;
        }

        if (!isdigit(*it) && it != end) return ERROR_VALUE;

        while (isdigit(*it)) {
            temp.push_back(*it);
            ++it;
        }

        if (!is_operator(*it) && it != end) return ERROR_VALUE;
        if (is_number(temp)) return std::stoul(temp);
        return temp;
    }
    static operator_expr convert_operator(std::string::const_iterator& it) {
        switch (*it) {
        case '+':
            operator_expr temp = ADD;
            return temp;
        case '-':
            operator_expr temp = SUBTRACT;
            return temp;
        case '*':
            operator_expr temp = MYLTYPLY;
            return temp;
        case '/':
            operator_expr temp = DIVIDE;
            return temp;
        }
    }
    static type_operand convert_right_arg
    (std::string::const_iterator& it, std::string::const_iterator end) {
        ++it;
        std::string temp;

        while (isalpha(*it) && it != end) {
            temp.push_back(*it);
            ++it;
        }

        if (!isdigit(*it) || it == end) return ERROR_VALUE;

        while (isdigit(*it) && it != end) {
            temp.push_back(*it);
            ++it;
        }

        if (it != end) return ERROR_VALUE;
        if (is_number(temp)) return std::stoul(temp);
        return temp;
    }
    static value_cell convert_to_expression
    (const std::string& str) {
        try {
            auto it = str.begin();
            if (!(*it == '=')) return ERROR_VALUE;
            else ++it;
            if (is_number(str)) return std::stoul(str);
            type_operand left = std::move(convert_left_arg(it, str.cend()));
            if (std::holds_alternative<int32_t>(left) 
                && std::get<int32_t>(left) == ERROR_VALUE) return ERROR_VALUE;
            operator_expr op = std::move(convert_operator(it));
            type_operand right = std::move(convert_right_arg(it, str.cend()));
            if (std::holds_alternative<int32_t>(right)
                && std::get<int32_t>(right) == ERROR_VALUE) return ERROR_VALUE;

            expression temp(left, op, right);
            return temp;
        }
        catch (std::out_of_range int_diapason) {
            return ERROR_VALUE;
        }
        catch (std::bad_variant_access get_variant) {
            return ERROR_VALUE;
        }

    }

public:
    static void play_convert(const std::vector<std::vector<std::string>>& initial_data,
        std::map<std::string, value_cell>& result) {

        for (size_t index_column = 1;
            index_column < initial_data.size(); ++index_column) {

            for (size_t index_string = 1; index_string < initial_data[index_column].size();
                ++index_string) {
                std::string key_cell =
                    initial_data[index_column][0] + initial_data[0][index_string];
                value_cell temp_v =
                    convert_to_expression(initial_data[index_column][index_string]);
                if (!std::holds_alternative<int32_t>)
                {
                    result[key_cell] = std::get<data_expression>(temp_v);
                    continue;
                }
                if (std::holds_alternative<int32_t>)
                {
                    result[key_cell] = std::get<int32_t>(temp_v);
                    continue;
                }
            }

        }

        return;
    }
};