/**
 * @file Parser.cpp
 * @author Edward Palmer
 * @date 2025-02-09
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "yaml/YAMLParser.hpp"
#include <cstdio>
#include <cstdlib>
#include <stack>
#include <stdexcept>
#include <unordered_map>

extern "C"
{
#include "logger/Logger.h"
}

YAMLFile YAMLParser::parse(const std::string &path)
{
    YAMLParser instance;
    return instance.parseYAMLFile(path);
}

YAMLFile YAMLParser::parseYAMLFile(const std::string &path)
{
    fp = fopen(path.c_str(), "r");
    if (!fp) throw std::runtime_error("failed to open file: " + path);

    YAMLFile parsedYAMLFile;

    // Skip top of file.
    skipRequiredChar('-', 3);
    skipOptionalChar('\n'); // Skip optional newlines.

    while (!isEOF())
    {
        char c = peek();
        if (c == ' ' || c == '\t' || c == '\n' || c == '#')
        {
            skipLine();
            continue; // Back to top.
        }

        // Read name of lists group:
        std::string key = parseKey();
        skipRequiredChar(':');
        skipRequiredChar('\n');

        parsedYAMLFile[key] = parseYAMLLists();
    }

    // Cleanup:
    fclose(fp);
    fp = nullptr;

    return parsedYAMLFile;
}


YAMLParser::~YAMLParser()
{
    if (!fp) return;

    fclose(fp);
    fp = nullptr;
}

void YAMLParser::skipLine()
{
    while (!isEOF() && getc(fp) != '\n')
    {
        ;
    }
}

void YAMLParser::skipOptionalCommentLine()
{
    while (peek() == '#')
    {
        skipLine();
    }
}

Point3 YAMLParser::parseDouble3(const char *buffer)
{
    if (!buffer)
    {
        LogError("Empty buffer for Point.");
        return point3(0, 0, 0);
    }

    char numBuffer[50];

    int iBuffer = 0;
    int iDouble = 0;

    double values[] = {0.0, 0.0, 0.0};

    bool hasOpeningClosingBrackets = (buffer[0] == '[' && buffer[strlen(buffer) - 1] == ']');
    if (!hasOpeningClosingBrackets)
    {
        LogError("Invalid format for point type: missing opening/closing brackets.");
        return point3(0, 0, 0); // Empty point.
    }

    bool hasDot = false;

    for (char *c = (char *)buffer + 1; *c; ++c) // Skip opening bracket.
    {
        bool isDigit = (*c >= '0' && *c <= '9');
        bool isDot = (*c == '.');
        bool isMinus = (*c == '-');

        if (iBuffer == 0)
        {
            hasDot = false;
        }

        if (isDigit || (!hasDot && isDot) || (iBuffer == 0 && isMinus))
        {
            if (isDot) hasDot = true;

            numBuffer[iBuffer++] = *c;
        }
        else if (*c == ',' || *c == ']') // Separator.
        {
            numBuffer[iBuffer] = '\0';

            values[iDouble++] = atof(numBuffer);

            if (iDouble > 2) break;

            // Do reset.
            iBuffer = 0;
        }
        else
        {
            LogError("Unexpected number character: '%c'.", *c);
            break;
        }
    }

    if (iDouble != 3)
    {
        LogError("LogLevelFailed to parse all point values.");
    }

    LogDebug("Parsed point: (%.2lf, %.2lf, %.2lf)", values[0], values[1], values[2]);
    return point3(values[0], values[1], values[2]);
}

YAMLParser::CoreType YAMLParser::classifyCoreType(const char *buffer) const
{
    if (!buffer) return CoreType::Invalid;

    bool hasDot = false;

    for (char *c = (char *)buffer; *c; ++c)
    {
        if ((*c >= '0' && *c <= '9') || (!hasDot && *c == '.'))
        {
            if (*c == '.') hasDot = true;
        }
        else if (isalpha(*c))
        {
            return CoreType::String;
        }
        else if (*c == '[')
        {
            return CoreType::Double3;
        }
        else
        {
            return CoreType::Invalid;
        }
    }

    return hasDot ? CoreType::Double : CoreType::Integer;
}

std::string YAMLParser::parseKey()
{
    char buffer[100];

    int iBuffer = 0;

    // TODO: - add additional safety checks to avoid skipping lines or going beyond bounds of buffer.
    while (!isEOF() && peek() != ':')
    {
        char c = getc(fp);

        if (isalnum(c) || c == '_' || c == '-' || c == '.')
        {
            buffer[iBuffer++] = c;
        }
        else
        {
            throw std::runtime_error("encounted invalid character while parsing key");
        }
    }

    buffer[iBuffer] = '\0';

    if (iBuffer < 1)
    {
        throw std::invalid_argument("empty key");
    }

    LogDebug("Parsed key: '%s'.", buffer);
    return std::string(buffer);
}


YAMLValue YAMLParser::parseYAMLValue()
{
    char buffer[100];

    int iBuffer = 0;

    while (!isEOF() && peek() != '\n')
    {
        char c = getc(fp);

        if (isspace(c)) // Skip spaces.
        {
            continue;
        }

        buffer[iBuffer++] = c;
    }

    if (iBuffer < 1)
    {
        throw std::runtime_error("empty value");
    }

    buffer[iBuffer] = '\0';

    switch (classifyCoreType(buffer))
    {
        case String:
            LogDebug("Parsed string value: %s", buffer);
            return YAMLValue(std::string(buffer));
        case Integer:
            LogDebug("Parsed integer value: %d", atoi(buffer));
            return YAMLValue((long)atoi(buffer));
        case Double:
            LogDebug("Parsed float value: %.2lf", atof(buffer));
            return YAMLValue(atof(buffer));
        case Double3:
            return YAMLValue(parseDouble3(buffer));
        case Invalid:
        default:
            throw std::runtime_error("failed to classify core type");
    }
}

YAMLList YAMLParser::parseYAMLList()
{
    const int kIndentLevel = 4; // Sublist indented by 4.

    bool isFirstCall = true;

    YAMLList subList;

    while (!isEOF() && (isFirstCall || (!isFirstCall && (countIndent() == kIndentLevel))))
    {
        if (isFirstCall)
        {
            // Skip "- " characters at start of sublist:
            isFirstCall = false;
            skipRequiredChar('-');
            skipRequiredChar(' ');
        }
        else
        {
            skipRequiredChar(' ', kIndentLevel);
        }

        std::string key = parseKey();

        skipRequiredChar(':');
        skipOptionalChar(' ');

        YAMLValue value = parseYAMLValue();

        subList[key] = std::move(value);

        skipRequiredChar('\n');
    }

    return subList;
}

bool YAMLParser::isEOF()
{
    return (peek() == EOF);
}

void YAMLParser::skipRequiredChar(char requiredChar, int count)
{
    if (count < 1) return;

    int skipCount = 0;

    while (!isEOF() && skipCount != count)
    {
        char c = getc(fp);

        if (c != requiredChar)
        {
            throw std::runtime_error("failed to skip character " + std::to_string(requiredChar));
        }

        ++skipCount;
    }

    if (skipCount != count)
    {
        throw std::runtime_error("failed to skip character required number of times");
    }
}


void YAMLParser::skipOptionalChar(char skippable)
{
    char c;

    while (!isEOF())
    {
        if ((c = getc(fp)) != skippable)
        {
            ungetc(c, fp);
            break;
        }
    }
}

char YAMLParser::peek()
{
    char c = getc(fp);

    ungetc(c, fp);

    return c;
}


int YAMLParser::countIndent()
{
    int indent = 0;

    while (!isEOF())
    {
        char next = getc(fp);

        if (next == ' ')
        {
            ++indent;
        }
        else
        {
            ungetc(next, fp);
            break;
        }
    }

    for (int i = 0; i < indent; ++i)
    {
        ungetc(' ', fp);
    }

    return indent;
}


std::vector<YAMLList> YAMLParser::parseYAMLLists()
{
    const int kIndentLevel = 2;

    std::vector<YAMLList> lists;

    bool hasSublists = false;
    bool isFirstCall = true;

    while (!isEOF() && countIndent() == kIndentLevel)
    {
        skipRequiredChar(' ', kIndentLevel);

        if (isFirstCall)
        {
            isFirstCall = false;
            hasSublists = (peek() == '-');

            // Expect there to be no sublists: just a single list of key-values.
            if (!hasSublists)
            {
                lists.push_back(YAMLList()); // Push-back empty list.
            }
        }


        if (hasSublists)
        {
            lists.push_back(parseYAMLList());
        }
        else
        {
            std::string key = parseKey();

            skipRequiredChar(':');
            skipOptionalChar(' ');

            YAMLValue value = parseYAMLValue();

            lists[0][key] = std::move(value);

            skipRequiredChar('\n');
        }
    }

    return lists;
}
