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

using Double3 = std::array<double, 3>;

using YAMLValue = std::variant<long, double, std::string, std::array<double, 3>>;
using YAMLSubBlock = std::unordered_map<std::string, YAMLValue>;
using YAMLBlock = std::unordered_map<std::string, std::variant<YAMLValue, YAMLSubBlock>>;

using YAMLBlocks = std::unordered_map<std::string, YAMLBlock>;


class YAMLParser
{
public:
    ~YAMLParser();
    YAMLParser() = default;

    static YAMLBlocks parse(const std::string &path)
    {
        YAMLParser instance;
        return instance.parseYAMLFile(path);
    }

    YAMLBlocks parseYAMLFile(const std::string &path);

    static YAMLBlock &getBlock(YAMLBlocks &data, std::string blockName)
    {
        return data[blockName];
    }

    static YAMLSubBlock &getSubBlock(YAMLBlocks &data, std::string blockName, std::string subBlockName)
    {
        return std::get<YAMLSubBlock>(getBlock(data, blockName)[subBlockName]);
    }

    template <typename T> static T &getBlockValue(YAMLBlocks &data, std::string blockName, std::string paramName)
    {
        return std::get<T>(getBlock(data, blockName)[paramName]);
    }

    template <typename T>
    static T &getSubBlockValue(YAMLBlocks &data, std::string blockName, std::string subBlockName, std::string paramName)
    {
        return std::get<T>(getSubBlock(data, blockName, subBlockName)[paramName]);
    }

protected:
    enum CoreType
    {
        Invalid,
        String,
        Integer,
        Double,
        Double3 // i.e. list of 3 doubles.
    };

    int countIndent();

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

    YAMLValue parseYAMLValue();
    YAMLSubBlock parseYAMLSubBlock(int indentLevel);

    YAMLBlock parseYAMLBlock(int indentLevel);

private:
    FILE *fp{nullptr};
};