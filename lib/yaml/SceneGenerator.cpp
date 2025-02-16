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
#include "engine/RenderSettings.h"
#include "logger/Logger.h"
}

SceneGenerator::SceneGenerator(const std::string pathToYAML_)
    : pathToYAML(std::move(pathToYAML_)), dataFromYAML(YAMLParser::parse(pathToYAML))
{
}

Scene *SceneGenerator::buildScene()
{
    buildTextureMap();
    buildMaterialMap();

    Scene *scene = makeScene();

    // Create the primitives here.
    for (auto &[name, subblock] : dataFromYAML.at("primitives"))
    {
        const YAMLSubBlock &primitiveBlock = std::get<YAMLSubBlock>(subblock);

        const std::string &primitiveType = std::get<std::string>(primitiveBlock.at("type"));

        Logger(LoggerDebug, "adding primitive: name: %s; type: %s", name.c_str(), primitiveType.c_str());

        Primitive *primitive{nullptr};

        if (primitiveType == "plane")
        {
            Double3 p0 = std::get<Double3>(primitiveBlock.at("pNought"));
            Double3 normal = std::get<Double3>(primitiveBlock.at("normal"));
            std::string materialName = std::get<std::string>(primitiveBlock.at("material"));

            primitive =
                makePlane(convertDouble3ToPoint3(p0), convertDouble3ToPoint3(normal), materialMap.at(materialName));
        }
        else
        {
            throw std::runtime_error("unsupported primitive type: " + primitiveType);
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


void SceneGenerator::configRenderer()
{
    Logger(LoggerDebug, "configuring renderer from YAML parameters");

    gRenderSettings.pixelsWide = getBlockValue<long>("output", "width");
    gRenderSettings.pixelsHigh = getBlockValue<long>("output", "height");

    // TODO: - need to copy.
    gRenderSettings.outputPath = strdup(getBlockValue<std::string>("output", "name").data());

    gRenderSettings.samplesPerPixel = getBlockValue<long>("config", "samplesPerPixel");
    gRenderSettings.maxDepth = getBlockValue<long>("config", "maxDepth");
    gRenderSettings.nthreads = getBlockValue<long>("config", "nthreads");
}

Camera SceneGenerator::buildCamera()
{
    Logger(LoggerDebug, "building camera from YAML parameters");

    double verticalFOV = getBlockValue<double>("camera", "verticalFOV");
    double aspectRatio = getBlockValue<double>("camera", "aspectRatio");
    double focalLength = getBlockValue<double>("camera", "focalLength");
    double aperture = getBlockValue<double>("camera", "aperture");
    Double3 origin = getBlockValue<Double3>("camera", "origin");
    Double3 target = getBlockValue<Double3>("camera", "target");

    Point3 originC = point3(origin[0], origin[1], origin[2]);
    Point3 targetC = point3(target[0], target[1], target[2]);

    return makeCamera(verticalFOV, aspectRatio, focalLength, aperture, originC, targetC);
}


Point3 SceneGenerator::convertDouble3ToPoint3(const Double3 &theValue)
{
    return point3(theValue.at(0), theValue.at(1), theValue.at(2));
}


Texture *SceneGenerator::buildSolidTexture(const YAMLSubBlock &textureBlock)
{
    Color3 textureColor = convertDouble3ToPoint3(std::get<Double3>(textureBlock.at("color")));

    return makeSolidTexture(std::move(textureColor));
}


Material *SceneGenerator::buildLambertianMaterial(const YAMLSubBlock &materialBlock)
{
    const std::string &albedoTextureName = std::get<std::string>(materialBlock.at("texture"));

    // Lookup texture.
    Texture *albedoTexture = textureMap.at(albedoTextureName);

    return makeLambertian(albedoTexture);
}


void SceneGenerator::buildTextureMap()
{
    Logger(LoggerDebug, "building texture map");

    for (auto &[name, subblock] : dataFromYAML.at("textures"))
    {
        const YAMLSubBlock &textureBlock = std::get<YAMLSubBlock>(subblock);
        const std::string &textureType = std::get<std::string>(textureBlock.at("type"));

        Logger(LoggerDebug, "adding texture: name: %s; type: %s", name.c_str(), textureType.c_str());

        Texture *theTexture{nullptr};

        if (textureType == "solid")
        {
            theTexture = buildSolidTexture(textureBlock);
        }
        else
        {
            throw std::runtime_error("unsupported texture type " + textureType);
        }

        textureMap[name] = theTexture;
    }
}


void SceneGenerator::buildMaterialMap()
{
    Logger(LoggerDebug, "building material map");

    // NB: require textures to already have been run.

    for (auto &[name, subblock] : dataFromYAML.at("materials"))
    {
        const YAMLSubBlock &materialBlock = std::get<YAMLSubBlock>(subblock);
        const std::string &materialType = std::get<std::string>(materialBlock.at("type"));

        Logger(LoggerDebug, "adding material: name: %s; type: %s", name.c_str(), materialType.c_str());

        Material *theMaterial{nullptr};

        if (materialType == "lambertian")
        {
            theMaterial = buildLambertianMaterial(materialBlock);
        }
        else
        {
            throw std::runtime_error("unsupported material type " + materialType);
        }

        materialMap[name] = theMaterial;
    }
}


template <typename T> T SceneGenerator::getBlockValue(std::string blockName, std::string paramName) const
{
    const YAMLBlock &block = dataFromYAML.at(blockName);

    const YAMLValue &value = std::get<YAMLValue>(block.at(paramName));

    return std::get<T>(value);
}
