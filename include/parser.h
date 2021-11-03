#pragma once

#include "lexer.h"

#include <fstream>
#include <string>

class Parser
{
public:
    void parse(const std::string& filepath)
    {
        bool parsing = true;

        std::ifstream ifs(filepath);
		if (!ifs || ifs.fail())
        {
            std::cerr << "Failed loading file: " << filepath << std::endl;
            return;
		}

        // Get the size of the file.                                     
        ifs.seekg(0, std::ios::end);
        // std::fstream::pos_type size = ifs.tellg();
        auto size = ifs.tellg();

        // Allocate enough memory for the content buffer
        std::string fileContent(1 + size, '\0');

        // Read the entire file contents.
        ifs.seekg(0, std::ios::beg);
        ifs.read(&fileContent[0], size);
        ifs.close();
    
        // Empty file
        if (fileContent == "")
            return;

        Lexer tokenizer(fileContent.data());
        while (parsing)
        {
            auto token = tokenizer.getNext();
            switch(token.type)
            {
                case TokenType::EndOfFile:
                {
                    std::cout << "End of file\n";
                    parsing = false;
                    break;
                }

                default:
                {
                    std::cout << "read token: " << (uint16_t)token.type << " (" << tokenToString[(uint16_t)token.type] << ")"
                            << ", line: " << token.line
                            << ", indent: " << token.indent
                            << ", text: " << token.text << std::endl;
                    break;
                }
            }
        }
    }
};