#include <iostream>
#include <exception>
#include <memory>
#include <map>
#include <vector>
#include <string>
#include <cctype>
#include <variant>
#include "data_expression.h"
#include "parser.h"

void load_file(std::vector<std::string>& data_fale, std::string file_name) {

}

int main()
{
    const std::vector<std::vector<std::string>> data_file {}; //= load_file(data_file, );
    std::map<std::string, value_cell> processed_data;
    Parser::play_convert(data_file, processed_data);

    for (size_t index_column = 0; index_column < data_file.size(); ++index_column) 
    {
        for (size_t index_string = 0; index_string < data_file.size(); ++index_column) 
        {
            if (index_column == 0) {
                std::cout << data_file[0][index_string] << ',';
                continue;
            }
            if (index_string == 0) {
                std::cout << data_file[index_column][0] << ',';
                continue;
            }
            std::string key = data_file[0][index_string] + data_file[index_column][0];
            if (std::holds_alternative<int32_t>(processed_data[key])) {
                std::cout << std::get<int32_t>(processed_data[key]);
            }
            //data_expression& temp = std::get<data_expression>(processed_data[key]);
            //std::cout << temp.calculate(processed_data[key], processed_data) << ',';
        }
        std::cout << "\n";
    }

    return 0;
}


