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
using value_cell = std::variant<int32_t, expression>;

class Parser {
private:
    static bool is_out_of_range_int32_t(const std::string str) {
        try {
            if (std::stol(str) <= ERROR_VALUE) return true;
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
        is_out_of_range_int32_t(str);
        return true;
    }
    static bool is_operator(char ch) {
        if (ch == '+' || ch == '-' || ch == '*' || ch == '/')
            return true;
        return false;
    }
    static type_operand convert_left_arg
        (std::string::const_iterator& it, std::string::const_iterator end,
        const std::vector<std::vector<std::string>>& initial_data) {
        std::string key_cell;
        std::string temp;

        while (it != end &&isalpha(*it)) {
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

        if (!is_operator(*it) && it != end) return ERROR_VALUE;
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
    static operator_expr convert_operator(std::string::const_iterator& it) {
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
    static type_operand convert_right_arg
        (std::string::const_iterator& it, std::string::const_iterator end, 
        const std::vector<std::vector<std::string>>& initial_data) {
        ++it;
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

        if (it != end) return ERROR_VALUE;
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
    static value_cell convert_to_expression
        (const std::string& str, const std::vector<std::vector<std::string>>& initial_data) {
        try {
            auto it = str.begin();
            if (!(*it == '=')) return ERROR_VALUE;
            else ++it;
            type_operand left = std::move(convert_left_arg(it, str.cend(), initial_data));
            if (std::holds_alternative<int32_t>(left) 
                && std::get<int32_t>(left) == ERROR_VALUE) return ERROR_VALUE;
            operator_expr op = std::move(convert_operator(it));
            type_operand right = std::move(convert_right_arg(it, str.cend(), initial_data));
            if (std::holds_alternative<int32_t>(right)
                && std::get<int32_t>(right) == ERROR_VALUE) return ERROR_VALUE;

            expression temp(left, op, right);
            return temp;
        }
        catch (std::out_of_range& int_diapason) {
            return ERROR_VALUE;
        }
        catch (std::bad_variant_access& get_variant) {
            return ERROR_VALUE;
        }

    }

public:
    static void play_convert(const std::vector<std::vector<std::string>>& initial_data,
        std::unordered_map<std::string, value_cell>& result) {

        for (size_t index_column = 1;
                index_column < initial_data.size(); ++index_column) {

            for (size_t index_string = 1; index_string < initial_data[index_column].size();
                ++index_string) {
                std::string key_cell =
                    initial_data[0][index_string] + initial_data[index_column][0];
                value_cell temp_expr;
                if (is_number(initial_data[index_column][index_string])) 
                    temp_expr = std::stol(initial_data[index_column][index_string]);
                else temp_expr =
                    convert_to_expression(initial_data[index_column][index_string], initial_data);
                if (std::holds_alternative<data_expression>(temp_expr))
                {
                    result[key_cell] = std::get<data_expression>(temp_expr);
                    continue;
                }
                if (std::holds_alternative<int32_t>(temp_expr))
                {
                    result[key_cell] = std::get<int32_t>(temp_expr);
                    continue;
                }
            }

        }

        return;
    }
};