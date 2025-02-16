/**
 * @file SceneGenerator.cpp
 * @author Edward Palmer
 * @date 2025-02-11
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "SceneGenerator.hpp"
#include <cstring>
#include <stdexcept>

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

        Logger(LoggerDebug, "building primitive of type: %s", type.c_str());

        Primitive *primitive{nullptr};

        if (type == "plane")
        {
            Point3 p0 = std::get<Point3>(list.at("pNought"));
            Point3 normal = std::get<Point3>(list.at("normal"));

            auto &materialName = std::get<std::string>(list.at("material"));

            primitive = makePlane(p0, normal, materialMap.at(materialName));
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


void YAMLSceneRenderer::configRenderer()
{
    Logger(LoggerDebug, "configuring renderer from YAML parameters");

    const YAMLList &outputList = dataFromYAML.at("output").front();

    gRenderSettings.pixelsWide = std::get<long>(outputList.at("width"));
    gRenderSettings.pixelsHigh = std::get<long>(outputList.at("height"));
    gRenderSettings.outputPath = strdup(std::get<std::string>(outputList.at("name")).data());

    const YAMLList &configList = dataFromYAML.at("config").front();

    gRenderSettings.samplesPerPixel = std::get<long>(configList.at("samplesPerPixel"));
    gRenderSettings.maxDepth = std::get<long>(configList.at("maxDepth"));
    gRenderSettings.nthreads = std::get<long>(configList.at("nthreads"));
}

Camera YAMLSceneRenderer::buildCamera()
{
    Logger(LoggerDebug, "building camera from YAML parameters");

    const YAMLList &cameraList = dataFromYAML.at("camera").front();

    double verticalFOV = std::get<double>(cameraList.at("verticalFOV"));
    double aspectRatio = std::get<double>(cameraList.at("aspectRatio"));
    double focalLength = std::get<double>(cameraList.at("focalLength"));
    double aperture = std::get<double>(cameraList.at("aperture"));

    Point3 origin = std::get<Point3>(cameraList.at("origin"));
    Point3 target = std::get<Point3>(cameraList.at("target"));

    return makeCamera(verticalFOV, aspectRatio, focalLength, aperture, origin, target);
}


void YAMLSceneRenderer::buildSolidTexture(const YAMLList &list)
{
    std::string name = std::get<std::string>(list.at("name"));
    Color3 textureColor = std::get<Color3>(list.at("color"));

    textureMap[name] = makeSolidTexture(textureColor);
}


void YAMLSceneRenderer::buildLambertianMaterial(const YAMLList &list)
{
    std::string name = std::get<std::string>(list.at("name"));
    const std::string &textureName = std::get<std::string>(list.at("texture"));

    // Lookup texture.
    Texture *albedoTexture = textureMap.at(textureName);

    materialMap[name] = makeLambertian(albedoTexture);
}


void YAMLSceneRenderer::buildTextureMap()
{
    Logger(LoggerDebug, "building texture map");

    for (auto &textureList : dataFromYAML.at("textures"))
    {
        auto &textureType = std::get<std::string>(textureList.at("type"));

        Logger(LoggerDebug, "building texture of type: %s", textureType.c_str());

        if (textureType == "solid")
        {
            buildSolidTexture(textureList);
        }
        else
        {
            throw std::runtime_error("unsupported texture type " + textureType);
        }
    }
}


void YAMLSceneRenderer::buildMaterialMap()
{
    Logger(LoggerDebug, "building material map");

    // Run the texture map if not already run since required for materials:
    if (textureMap.empty())
    {
        buildTextureMap();
    }

    for (auto &materialList : dataFromYAML.at("materials"))
    {
        auto &materialType = std::get<std::string>(materialList.at("type"));

        Logger(LoggerDebug, "building material of type: %s", materialType.c_str());

        if (materialType == "lambertian")
        {
            buildLambertianMaterial(materialList);
        }
        else
        {
            throw std::runtime_error("unsupported material type " + materialType);
        }
    }
}
