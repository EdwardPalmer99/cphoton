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

#include "engine/Material.h"
#include "engine/Primitive.h"
#include "engine/Texture.h"
#include "yaml/YAMLParser.hpp"
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
    void buildPlanePrimitive(const YAMLList &list, Primitive *&primitive);
    void buildSpherePrimitive(const YAMLList &list, Primitive *&primitive);
    void buildCubePrimitive(const YAMLList &list, Primitive *&primitive);
    // void buildDiscPrimitive(const YAMLList &list, Primitive *&primitive);
    // void buildConePrimitive(const YAMLList &list, Primitive *&primitive);
    // void buildTrianglePrimitive(const YAMLList &list, Primitive *&primitive);
    // void buildCylinderPrimitive(const YAMLList &list, Primitive *&primitive);

    void buildTextureMap();
    void buildMaterialMap();

    // TODO: - extract texture map stuff into separate class.
    void buildSolidTexture(const YAMLList &textureBlock);
    void buildCheckerTexture(const YAMLList &textureBlock);

    // TODO: - extract material stuff into separate class.
    void buildMetalMaterial(const YAMLList &materialBlock);
    void buildLambertianMaterial(const YAMLList &materialBlock);
    // void buildDielectricMaterial(const YAMLList &materialBlock);
    // void buildEmitterMaterial(const YAMLList &materialBlock);

    std::unordered_map<std::string, Texture *> textureMap;
    std::unordered_map<std::string, Material *> materialMap;

    const std::string pathToYAML;
    const YAMLFile dataFromYAML;
};