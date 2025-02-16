#include "SceneGenerator.hpp"
#include "YAMLParser.hpp"

extern "C"
{
#include "engine/Primitive.h"
#include "engine/RayTracer.h"
#include "engine/RenderSettings.h"
#include "utility/PPMWriter.h"
}

int main(void)
{
    YAMLSceneRenderer::doRender("TestFile.yaml");
    return 0;
}