/**
 * @file SceneGenerator.cpp
 * @author Edward Palmer
 * @date 2025-02-11
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "yaml/SceneGenerator.hpp"
#include <cstring>
#include <stdexcept>
#include <thread>

extern "C"
{
#include "engine/RayTracer.h"
#include "engine/RenderSettings.h"
#include "logger/Logger.h"
#include "utility/PPMWriter.h"
}

YAMLSceneRenderer::YAMLSceneRenderer(const std::string pathToYAML_)
    : pathToYAML(std::move(pathToYAML_)), dataFromYAML(YAMLParser::parse(pathToYAML))
{
}

void YAMLSceneRenderer::doRender(const std::string pathToYAML)
{
    LogInfo("Parsing YAML.");
    YAMLSceneRenderer yamlRenderer(std::move(pathToYAML));

    yamlRenderer.configRenderer();

    Camera camera = yamlRenderer.buildCamera();

    Scene *scene = yamlRenderer.buildScene();

    PPMImage *outputImage = renderScene(scene, &camera);

    writeBinary16BitPPMImage(outputImage, gRenderSettings.outputPath);

    // Cleanup.
    scene->destructor(scene);
    freePPMImage(outputImage);
}

Scene *YAMLSceneRenderer::buildScene()
{
    // Build material map since required for primitive definitions:
    if (materialMap.empty())
    {
        buildMaterialMap();
    }

    Scene *scene = makeScene();

    // Create the primitives here.
    for (auto &list : dataFromYAML.at("primitives"))
    {
        const std::string &type = std::get<std::string>(list.at("type"));

        Primitive *primitive{nullptr};

        if (type == "plane")
        {
            buildPlanePrimitive(list, primitive);
        }
        else if (type == "sphere")
        {
            buildSpherePrimitive(list, primitive);
        }
        else if (type == "cube")
        {
            buildCubePrimitive(list, primitive);
        }
        else if (type == "disc")
        {
            buildDiscPrimitive(list, primitive);
        }
        else if (type == "cylinder")
        {
            buildCylinderPrimitive(list, primitive);
        }
        else if (type == "cone")
        {
            buildConePrimitive(list, primitive);
        }
        else if (type == "triangle")
        {
            buildTrianglePrimitive(list, primitive);
        }
        else
        {
            throw std::runtime_error("unsupported primitive type: " + type);
        }

        if (primitive != nullptr)
        {
            scene->addObject(scene, primitive);
        }
    }

    // Finished adding primitives:
    scene->markAsFinished(scene);

    return scene;
}


void YAMLSceneRenderer::buildPlanePrimitive(const YAMLList &list, Primitive *&primitive)
{
    Point3 p0 = std::get<Point3>(list.at("p0"));
    Point3 normal = std::get<Point3>(list.at("normal"));

    LogDebug("Building plane: p0: (%.2lf, %.2lf, %.2lf); "
             "normal: (%.2lf, %.2lf, %.2lf)",
             p0.x, p0.y, p0.z, normal.x, normal.y, normal.z);

    auto &materialName = std::get<std::string>(list.at("material"));

    // NB: reference to pointer so don't need double indirection.
    primitive = makePlane(p0, normal, materialMap.at(materialName));
}


void YAMLSceneRenderer::buildSpherePrimitive(const YAMLList &list, Primitive *&primitive)
{
    auto center = std::get<Point3>(list.at("center"));
    auto radius = std::get<double>(list.at("radius"));

    LogDebug("Building sphere: center: (%.2lf, %.2lf, %.2lf); radius: %.2lf", center.x, center.y, center.z, radius);

    auto &materialName = std::get<std::string>(list.at("material"));

    primitive = makeSphere(center, radius, materialMap.at(materialName));
}


void YAMLSceneRenderer::buildCubePrimitive(const YAMLList &list, Primitive *&primitive)
{
    auto center = std::get<Point3>(list.at("center"));
    auto rotation = std::get<Point3>(list.at("rotation"));
    auto length = std::get<double>(list.at("length"));

    LogDebug("Building cube: center: (%.2lf, %.2lf, %.2lf); length: %.2lf", center.x, center.y, center.z, length);

    auto &materialName = std::get<std::string>(list.at("material"));

    primitive = makeCube(center, rotation, length, materialMap.at(materialName));
}


void YAMLSceneRenderer::buildDiscPrimitive(const YAMLList &list, Primitive *&primitive)
{
    auto center = std::get<Point3>(list.at("center"));
    auto normal = std::get<Vector3>(list.at("normal"));
    auto radius = std::get<double>(list.at("radius"));

    auto &materialName = std::get<std::string>(list.at("material"));

    primitive = makeDisc(center, normal, radius, materialMap.at(materialName));
}


void YAMLSceneRenderer::buildCylinderPrimitive(const YAMLList &list, Primitive *&primitive)
{
    auto center = std::get<Point3>(list.at("center"));
    auto rotationAngle = std::get<Vector3>(list.at("rotation"));
    auto radius = std::get<double>(list.at("radius"));
    auto height = std::get<double>(list.at("height"));

    auto &materialName = std::get<std::string>(list.at("material"));

    primitive = makeCylinder(center, rotationAngle, radius, height, materialMap.at(materialName));
}


void YAMLSceneRenderer::buildConePrimitive(const YAMLList &list, Primitive *&primitive)
{
    auto center = std::get<Point3>(list.at("center"));
    auto rotationAngle = std::get<Vector3>(list.at("rotationAngle"));
    auto height = std::get<double>(list.at("height"));

    auto &materialName = std::get<std::string>(list.at("material"));

    primitive = makeCone(center, rotationAngle, height, materialMap.at(materialName));
}


void YAMLSceneRenderer::buildTrianglePrimitive(const YAMLList &list, Primitive *&primitive)
{
    auto p0 = std::get<Point3>(list.at("p0"));
    auto p1 = std::get<Point3>(list.at("p1"));
    auto p2 = std::get<Point3>(list.at("p2"));

    auto &materialName = std::get<std::string>(list.at("material"));

    primitive = makeTriangle(p0, p1, p2, materialMap.at(materialName));
}

void YAMLSceneRenderer::configRenderer()
{
    const YAMLList &outputList = dataFromYAML.at("output").front();

    gRenderSettings.pixelsWide = std::get<long>(outputList.at("width"));
    gRenderSettings.pixelsHigh = std::get<long>(outputList.at("height"));
    gRenderSettings.outputPath = strdup(std::get<std::string>(outputList.at("name")).data());
}

Camera YAMLSceneRenderer::buildCamera()
{
    const YAMLList &cameraList = dataFromYAML.at("camera").front();

    double verticalFOV = std::get<double>(cameraList.at("verticalFOV"));
    double focalLength = std::get<double>(cameraList.at("focalLength"));
    double aperture = std::get<double>(cameraList.at("aperture"));

    Point3 origin = std::get<Point3>(cameraList.at("origin"));
    Point3 target = std::get<Point3>(cameraList.at("target"));

    // Calculate aspect ratio based on image width/height. We assume that these settings have been set (TODO: - be
    // smarter).
    const double aspectRatio = ((double)gRenderSettings.pixelsWide / (double)gRenderSettings.pixelsHigh);

    LogDebug("Building camera: vFOV: %.2lf; aspectRatio: %.2lf; focalLength: %.2lf; aperture: %.2lf; "
             "origin: (%.2lf, %.2lf, %.2lf); target: (%.2lf, %.2lf, %.2lf)",
             verticalFOV, aspectRatio, focalLength, aperture, origin.x, origin.y, origin.z, target.x, target.y,
             target.z);

    return makeCamera(verticalFOV, aspectRatio, focalLength, aperture, origin, target);
}


void YAMLSceneRenderer::buildSolidTexture(const YAMLList &list)
{
    std::string name = std::get<std::string>(list.at("name"));
    Color3 textureColor = std::get<Color3>(list.at("color"));

    LogDebug("Building solid texture: name: %s; color: (%.2lf, %.2lf, %.2lf)", name.c_str(), textureColor.r,
             textureColor.g, textureColor.b);

    textureMap[name] = makeSolidTexture(textureColor);
}


void YAMLSceneRenderer::buildCheckerTexture(const YAMLList &list)
{
    std::string name = std::get<std::string>(list.at("name"));

    auto &texture1Name = std::get<std::string>(list.at("texture1"));
    auto &texture2Name = std::get<std::string>(list.at("texture2"));

    LogDebug("Building checker texture: name: %s; texture1: %s; texture2: %s", name.c_str(), texture1Name.c_str(),
             texture2Name.c_str());

    // We assume that the textures have already been defined! Also cannot allow self-reference.
    textureMap[name] = makeCheckerTexture(textureMap.at(texture1Name), textureMap.at(texture2Name));
}


void YAMLSceneRenderer::buildDielectricMaterial(const YAMLList &list)
{
    std::string name = std::get<std::string>(list.at("name"));
    double indexOfRefraction = std::get<double>(list.at("indexOfRefraction"));

    materialMap[name] = makeDielectric(indexOfRefraction);
}


void YAMLSceneRenderer::buildEmitterMaterial(const YAMLList &list)
{
    std::string name = std::get<std::string>(list.at("name"));
    Color3 color = std::get<Color3>(list.at("color"));

    materialMap[name] = makeEmitter(color);
}


void YAMLSceneRenderer::buildLambertianMaterial(const YAMLList &list)
{
    std::string name = std::get<std::string>(list.at("name"));
    const std::string &textureName = std::get<std::string>(list.at("texture"));

    LogDebug("Building lambertian material: name: %s; textureName: %s", name.c_str(), textureName.c_str());

    // Lookup texture.
    Texture *albedoTexture = textureMap.at(textureName);

    materialMap[name] = makeLambertian(albedoTexture);
}


void YAMLSceneRenderer::buildMetalMaterial(const YAMLList &list)
{
    std::string name = std::get<std::string>(list.at("name"));
    const std::string &textureName = std::get<std::string>(list.at("albedo"));
    auto fuzziness = std::get<double>(list.at("fuzziness"));

    LogDebug("Building metal material: name: %s; textureName: %s", name.c_str(), textureName.c_str());

    materialMap[name] = makeMetal(textureMap.at(textureName), fuzziness);
}


void YAMLSceneRenderer::buildTextureMap()
{
    for (auto &textureList : dataFromYAML.at("textures"))
    {
        auto &textureType = std::get<std::string>(textureList.at("type"));

        if (textureType == "solid")
        {
            buildSolidTexture(textureList);
        }
        else if (textureType == "checker")
        {
            buildCheckerTexture(textureList);
        }
        else
        {
            throw std::runtime_error("unsupported texture type " + textureType);
        }
    }
}


void YAMLSceneRenderer::buildMaterialMap()
{
    // Run the texture map if not already run since required for materials:
    if (textureMap.empty())
    {
        buildTextureMap();
    }

    for (auto &materialList : dataFromYAML.at("materials"))
    {
        auto &materialType = std::get<std::string>(materialList.at("type"));

        if (materialType == "lambertian")
        {
            buildLambertianMaterial(materialList); // TODO: - create map of names to function pointers with std::bind.
        }
        else if (materialType == "metal")
        {
            buildMetalMaterial(materialList);
        }
        else if (materialType == "dielectric")
        {
            buildDielectricMaterial(materialList);
        }
        else if (materialType == "emitter")
        {
            buildEmitterMaterial(materialList);
        }
        else
        {
            throw std::runtime_error("unsupported material type " + materialType);
        }
    }
}
