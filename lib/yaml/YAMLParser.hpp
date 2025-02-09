/**
 * @file YAMLParser.hpp
 * @author Edward Palmer
 * @date 2025-02-09
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include <array>
#include <cstdio>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <variant>

using YAMLCore = std::variant<long, double, std::string, std::array<double, 3>>;
using YAMLList = std::unordered_map<std::string, YAMLCore>;
using YAMLLists = std::unordered_map<std::string, YAMLList>;

// using YAMLMap = std::unordered_map<std::string, std::variant<YAMLList, YAMLLists>>;
using YAMLMap = std::unordered_map<std::string, YAMLList>;

class YAMLParser
{
public:
    ~YAMLParser();

    YAMLMap parseYAMLFile(const std::string &path);


protected:
    enum CoreType
    {
        Invalid,
        String,
        Integer,
        Double,
        Double3 // i.e. list of 3 doubles.
    };

    CoreType classifyCoreType(const char *buffer) const;

    void skipOptionalChar(char skippable);
    void skipRequiredChar(char requiredChar, int count = 1);

    void skipLine();
    void skipOptionalCommentLine();

    std::array<double, 3> parseDouble3(const char *buffer);

    // TODO: - skip comments.

    char peek();

    bool isEOF();

    std::string parseKey();

    YAMLCore parseYAMLCoreValue();
    YAMLList parseYAMLListValue(int indentLevel);

private:
    FILE *fp{nullptr};
};