#include <iostream>
#include <exception>
#include <memory>
#include <unordered_map>
#include <vector>
#include <string>
#include <cctype>
#include <variant>
#include <fstream>
#include <sstream>
#include "data_expression.h"
#include "parser.h"

//Поймать все исключения


std::vector<std::vector<std::string>> load_file( std::string file_name) {
    std::vector<std::vector<std::string>> sheet;
    std::string line_from_file;
    std::ifstream loader(file_name);
    char delimiter = ',';
    while (std::getline(loader, line_from_file)) {
        std::stringstream ohuennyi_stream_of_stroka(line_from_file);
        std::vector<std::string> row;
        std::string cell_str;
        while (std::getline(ohuennyi_stream_of_stroka, cell_str, ',')) {
            row.push_back(std::move(cell_str));
            cell_str.clear();
        }
        sheet.push_back(std::move(row));
    }
    return sheet;
}

int main(int agrc, char* argv[]) {
    std::string file_name = "C:\\Users\\Foxick\\Desktop\\table.csv";
    //auto file_name = argv[1];
    const std::vector<std::vector<std::string>> data_file = std::move(load_file(file_name));
    std::unordered_map<std::string, value_cell> processed_data;
    play_convert(data_file, processed_data);

    for (size_t index_column = 0, index_row = 0; index_row < data_file[0].size(); ++index_row)
    {
        bool last_cell = index_row == data_file[0].size() - 1;
        if (!is_text(data_file[0][index_row])) {
            std::cout << "ErrNameColumn" << (last_cell ? "" : ",");
        } else std::cout << data_file[0][index_row] << (last_cell ? "" : ",");
    }
    std::cout << "\n";

    for (size_t index_column = 1; index_column < data_file.size(); ++index_column)
    {
        for (size_t index_row = 0; index_row < data_file[0].size(); ++index_row)
        {
            if (index_row == 0) {
                if (!is_number(data_file[index_column][0])) {
                    std::cout << "ErrNameRow" << ',';
                } else std::cout << data_file[index_column][0] << ',';
                continue;
            }
            std::string key = data_file[0][index_row] + data_file[index_column][0];
            value_cell temp = processed_data[key];
            if (std::holds_alternative<int32_t>(processed_data[key])) {
                bool last_cell = index_row == data_file.size() - 1;
                if (std::get<int32_t>(temp) == ERROR_VALUE) {
                    std::cout << "ErrExpr" << (last_cell ? "" : ",");
                }
                else {
                    std::cout << std::get<int32_t>(temp) << (last_cell ? "" : ",");
                }
            }
            else {
                data_expression& temp = std::get<data_expression>(processed_data[key]);
                bool last_cell = index_row == data_file.size() - 1;
                int32_t result = temp.calculate(processed_data[key], processed_data);
                if (result == ERROR_VALUE) {
                    std::cout << "ErrExpr" << (last_cell ? "" : ",");
                }
                else {
                    std::cout << result << (last_cell ? "" : ",");
                }
            }
        }
        std::cout << "\n";
    }
    return 0;
}


