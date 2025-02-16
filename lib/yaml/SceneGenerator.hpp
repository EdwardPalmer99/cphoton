/**
 * @file YAMLSceneRenderer.hpp
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


class YAMLSceneRenderer
{
public:
    YAMLSceneRenderer() = delete;

    static void doRender(const std::string pathToYAML);

protected:
    YAMLSceneRenderer(const std::string pathToYAML_);

    void configRenderer();
    Scene *buildScene();
    Camera buildCamera();

private:
    void buildTextureMap();
    void buildMaterialMap();

    void buildSolidTexture(const YAMLList &textureBlock);
    void buildLambertianMaterial(const YAMLList &materialBlock);

    std::unordered_map<std::string, Texture *> textureMap;
    std::unordered_map<std::string, Material *> materialMap;

    const std::string pathToYAML;
    const YAMLFile dataFromYAML;
};