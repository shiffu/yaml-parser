#pragma once

#include <string>

static const char* tokenToString[] = {
    "OpenBrace",
    "CloseBrace",
    "OpenBracket",
    "CloseBraket",
    "StartDoc",
    "EndDoc",
    "Comment",
    "EmptyLine",
    "Space",
    "Comma",
    "Seq",
    "MapKey",
    "String",
    "Invalid",
    "EndOfFile"
};

enum class TokenType
{
    OpenBrace,
    CloseBrace,
    OpenBracket,
    CloseBraket,

    StartDoc,
    EndDoc,

    Comment,
    EmptyLine,
    
    Space,
    Comma,

    Seq,
    MapKey,

    String,

    Invalid,
    EndOfFile
};

struct Token
{
    TokenType type;
    std::string text;
    uint32_t indent;
    uint32_t line;
};

class Lexer
{
public:
    Lexer(char* content)
        : m_at(content) {}

    ~Lexer() = default;

    Token getNext()
    {
        if (isNewLine())
        {
            gotoNewLine();
            ++m_at;
        }

        Token token{TokenType::Invalid, "", 0};
        token.text.push_back(*m_at);
        token.line = m_line;

        // If before this token we just moved to a new line
        // then we have to compute the indent size
        if (m_justMovedToNewLine)
        {
            while(*m_at && (isWhiteSpace(*m_at) || isNewLine()))
            {
                // Check for empty line
                if (isNewLine())
                {
                    gotoNewLine();
                    ++m_at;
                    token.type = TokenType::EmptyLine;
                    token.text = "";
                    return token;
                }

                // Tabs are not supported for indentation
                if (*m_at == '\t')
                {
                    ++m_at;
                    token.text = "Lex Error: Tabs are not supported for indentation";
                    return token;
                }
                m_at++;
                token.indent++;
            }
            m_justMovedToNewLine = false;
        }

        switch(*m_at)
        {
            case '{': { token.type = TokenType::OpenBrace; break; }
            case '}': { token.type = TokenType::CloseBrace; break; }
            case '[': { token.type = TokenType::OpenBracket; break; }
            case ']': { token.type = TokenType::CloseBraket; break; }
            case '\0': { token.type = TokenType::EndOfFile; break; }

            case ' ': { token.type = TokenType::Space; break; }
            case ',': { token.type = TokenType::Comma; break; }

            case '#':
            {
                // Check if we have a comment
                if (m_at[1] == ' ')
                {
                    token.type = TokenType::Comment;
                    auto start = m_at;
                    gotoNewLine();
                    token.text.assign(start, size_t(m_at - start));
                }
                break;
            }

            // '-' Could be either a start document or a Sequence
            case '-':
            {
                ++m_at;
                if (m_at[0] == ' ')
                {
                    // ++m_at;
                    token.type = TokenType::Seq;
                }
                else if (m_at[0] == '-' && m_at[1] == '-')
                {
                    token.text.pop_back();
                    token.indent += 2;
                    m_at += 2;
                    gotoNewLine();
                    token.type = TokenType::StartDoc;
                }
                else
                {
                    token.type = TokenType::Invalid;
                }
                break;
            }

            case '.':
            {
                ++m_at;
                if (m_at[0] == '.' && m_at[1] == '.')
                {
                    token.type = TokenType::EndDoc;
                    token.text = "";
                }
                else
                {
                    token.type = TokenType::Invalid;
                    token.text = "Dot should either be in the form of '...' to signal the end of the document or in a string";
                }
                ++m_at;
                break;
            }
            
            case '"':
            {
                ++m_at;
                buildString(token, '"');
                break;
            }
            
            case '\'':
            {
                ++m_at;
                buildString(token, '\'');
                break;
            }
            
            // Here we can have the following:
            //  + multi lines strings (| or >)
            //  + map key => key:
            //  + Scalar (strings, numbers, etc)
            default:
            {
                // We will first assume that we are going to get a string
                auto start = m_at;
                token.type = TokenType::String;

                while (!isEOF() && !isNewLine())
                {
                    // We are dealing with a Map key
                    if (m_at[0] == ':' && m_at[1] == ' ')
                    {
                        token.type = TokenType::MapKey;
                        break;
                    }
                    ++m_at;
                }
                token.text.assign(start, size_t(m_at - start));
                break;
            }
        }
        ++m_at;

        return token;
    }

private:
    void buildString(Token& token, char endOfString)
    {
        token.type = TokenType::String;
        auto start = m_at;

        while(m_at[0] && m_at[0] != endOfString)
        {
            if (m_at[0] == '\\' && m_at[1])
            {
                ++m_at;
            }
            ++m_at;
        }
        token.text.assign(start, size_t(m_at - start));
    }

    inline void buildComment()
    {
        while(*m_at && !isNewLine())
        {
            // cout << "eatComment -> eating char: " << *m_at << endl;
            ++m_at;
        }
    }

    inline bool isEOF()
    {
        bool result = (*m_at == '\0');

        return result;
    }

    inline bool isNewLine()
    {
        bool newLineFound = false;
        if (!isEOF())
        {
            if (*m_at == '\n')
            {
                if (m_at[1] == '\r') m_at++;
                newLineFound = true;
            }
            else if (*m_at == '\r')
            {
                if (m_at[1] == '\n') m_at++;
                newLineFound = true;
            }
        }

        return newLineFound;
    }

    inline void gotoNewLine()
    {
        while (!isNewLine())
        {
            ++m_at;
        }
        ++m_line;
        m_justMovedToNewLine = true;
    }

    inline bool isWhiteSpace(char c)
    {
        bool result = ((c == ' ') || (c == '\t'));

        return result;
    }

private:
    char* m_at{nullptr};
    uint32_t m_line{1};
    bool m_justMovedToNewLine{true};
};