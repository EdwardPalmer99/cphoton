/**
 * @file SceneGenerator.hpp
 * @author Edward Palmer
 * @date 2025-02-11
 *
 * @copyright Copyright (c) 2025
 *
 */

extern "C"
{
#include "engine/Camera.h"
#include "engine/Scene.h"
}

#include "YAMLParser.hpp"
#include "engine/Material.h"
#include "engine/Primitive.h"
#include "engine/Texture.h"
#include <string>
#include <unordered_map>


class SceneGenerator
{
public:
    SceneGenerator() = delete;
    SceneGenerator(const std::string pathToYAML_);

    Scene *buildScene();

    Camera buildCamera();

    void configRenderer();

protected:
    void buildTextureMap();
    void buildMaterialMap();

    static Point3 convertDouble3ToPoint3(const Double3 &theValue);

    Texture *buildSolidTexture(const YAMLSubBlock &textureBlock);
    Material *buildLambertianMaterial(const YAMLSubBlock &materialBlock);
    // TODO: - implement primitive building methods for different primitive types.

private:
    template <typename T> T getBlockValue(std::string blockName, std::string paramName) const;

    // std::unordered_map<std::string, Primitive *> primitiveMap;
    std::unordered_map<std::string, Texture *> textureMap;
    std::unordered_map<std::string, Material *> materialMap;

    const std::string pathToYAML;
    const YAMLBlocks dataFromYAML;
};