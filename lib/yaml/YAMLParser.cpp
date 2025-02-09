/**
 * @file Parser.cpp
 * @author Edward Palmer
 * @date 2025-02-09
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "YAMLParser.hpp"
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <unordered_map>

YAMLMap YAMLParser::parseYAMLFile(const std::string &path)
{
    fp = fopen(path.c_str(), "r");
    if (!fp) throw std::runtime_error("failed to open file: " + path);

    YAMLMap parsedYAMLFile;

    // Skip top of file.
    skipRequiredChar('-', 3);
    skipOptionalChar('\n'); // Skip optional newlines.

    int indentLevel = 2;

    while (!isEOF())
    {
        // Skip comment lines.
        skipOptionalCommentLine();
        skipOptionalChar('\n');

        if (isEOF()) break;

        // Read name of list group:
        std::string key = parseKey();

        skipRequiredChar(':');
        skipRequiredChar('\n');

        // Read list:
        YAMLList value = parseYAMLListValue(indentLevel);
        parsedYAMLFile[key] = std::move(value);
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

std::array<double, 3> YAMLParser::parseDouble3(const char *buffer)
{
    char numBuffer[50];
    int iBuffer = 0;
    bool hasDot = false;

    int iDouble = 0;
    std::array<double, 3> values;

    if (buffer[0] != '[')
    {
        throw std::runtime_error("expected '[' for double3 type");
    }

    for (char *c = (char *)buffer + 1; *c; ++c)
    {
        if (isnumber(*c) || (!hasDot && *c == '.'))
        {
            if (*c == '.') hasDot = true;
            numBuffer[iBuffer++] = *c;
        }
        else if (*c == ' ')
        {
            continue;
        }
        else if (*c == ',' && iDouble < 3 && iBuffer != 0)
        {
            numBuffer[iBuffer] = '\0';
            values[iDouble++] = atof(numBuffer);

            iBuffer = 0; // Reset.
            hasDot = false;
        }
        else if (*c == ']')
        {
            break;
        }
        else
        {
            throw std::runtime_error("unexpected character when parsing double3 type");
        }
    }

    if (iDouble != 2)
    {
        throw std::runtime_error("failed to read all double3 values");
    }

    return values;
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

        if (!isalpha(c))
        {
            throw std::runtime_error("encountered non-alpha character while parsing key: " + std::to_string(c));
        }

        buffer[iBuffer++] = c;
    }

    buffer[iBuffer] = '\0';

    if (iBuffer < 1) throw std::runtime_error("empty key");

    return std::string(buffer);
}


YAMLCore YAMLParser::parseYAMLCoreValue()
{
    char buffer[100];

    int iBuffer = 0;

    while (!isEOF() && peek() != '\n')
    {
        buffer[iBuffer++] = getc(fp);
    }

    buffer[iBuffer] = '\0';

    switch (classifyCoreType(buffer))
    {
        case String:
            return YAMLCore(std::string(buffer));
        case Integer:
            return YAMLCore((long)atoi(buffer));
        case Double:
            return YAMLCore((long)atof(buffer));
        case Double3:
            return YAMLCore(parseDouble3(buffer));
        case Invalid:
        default:
            throw std::runtime_error("failed to classify core type");
    }
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


YAMLList YAMLParser::parseYAMLListValue(int indentLevel)
{
    YAMLList parsedList;

    while (peek() == ' ')
    {
        skipRequiredChar(' ', indentLevel);

        std::string key = parseKey();
        skipRequiredChar(':');
        skipOptionalChar(' ');

        if (peek() == '\n') // Further nesting!
        {
            // TODO: - handle additional nesting here.
            // YAMLList values = parseYAMLListValue(indentLevel + 2);
        }

        YAMLCore value = parseYAMLCoreValue();

        parsedList[key] = std::move(value);

        skipRequiredChar('\n'); // Skip newline.
    }

    if (parsedList.size() < 1)
    {
        throw std::runtime_error("failed to parse any key:values");
    }

    return parsedList;
}
