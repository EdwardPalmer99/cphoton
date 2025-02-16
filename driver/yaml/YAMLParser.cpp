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

static constexpr bool isNumber(char c)
{
    return (c >= '0' && c <= '9');
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

        Logger(LoggerDebug, "parsing lists for key: %s", key.c_str());
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
    char numBuffer[50];
    int iBuffer = 0;
    bool hasDot = false;

    int iDouble = 0;
    double values[3];

    if (buffer[0] != '[')
    {
        throw std::runtime_error("expected '[' for double3 type");
    }

    for (char *c = (char *)buffer + 1; *c; ++c)
    {
        if (isNumber(*c) || (!hasDot && *c == '.'))
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


YAMLValue YAMLParser::parseYAMLValue()
{
    char buffer[100];

    int iBuffer = 0;

    while (!isEOF() && peek() != '\n')
    {
        buffer[iBuffer++] = getc(fp);
    }

    buffer[iBuffer] = '\0';

    Logger(LoggerDebug, "parsing YAML value: %s", buffer);

    switch (classifyCoreType(buffer))
    {
        case String:
            return YAMLValue(std::string(buffer));
        case Integer:
            return YAMLValue((long)atoi(buffer));
        case Double:
            return YAMLValue((double)atof(buffer));
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
        Logger(LoggerDebug, "parsing list key: %s", key.c_str());

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
            Logger(LoggerDebug, "parsing sublist");
            lists.push_back(parseYAMLList());
        }
        else
        {
            std::string key = parseKey();
            Logger(LoggerDebug, "parsing key: %s", key.c_str());

            skipRequiredChar(':');
            skipOptionalChar(' ');

            YAMLValue value = parseYAMLValue();

            lists[0][key] = std::move(value);

            skipRequiredChar('\n');
        }
    }

    return lists;
}
