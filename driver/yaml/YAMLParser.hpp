/**
 * @file YAMLParser.hpp
 * @author Edward Palmer
 * @date 2025-02-09
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

extern "C"
{
#include "utility/Vector3.h"
}

using YAMLValue = std::variant<long, double, std::string, Point3>;
using YAMLList = std::unordered_map<std::string, YAMLValue>;
using YAMLFile = std::unordered_map<std::string, std::vector<YAMLList>>;

class YAMLParser
{
public:
    static YAMLFile parse(const std::string &path);

protected:
    YAMLParser() = default;
    ~YAMLParser();

    enum CoreType
    {
        Invalid,
        String,
        Integer,
        Double,
        Double3 // i.e. list of 3 doubles.
    };

    YAMLFile parseYAMLFile(const std::string &path);

    int countIndent();

    CoreType classifyCoreType(const char *buffer) const;

    void skipOptionalChar(char skippable);
    void skipRequiredChar(char requiredChar, int count = 1);

    void skipLine();
    void skipOptionalCommentLine();

    Point3 parseDouble3(const char *buffer);

    char peek();

    bool isEOF();

    std::string parseKey();

    YAMLValue parseYAMLValue();

    // Parse a YAML list (started by '-' character).
    YAMLList parseYAMLList();

    // Parse one or more YAML lists of key, values.
    std::vector<YAMLList> parseYAMLLists();

private:
    FILE *fp{nullptr};
};