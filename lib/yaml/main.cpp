#include "SceneGenerator.hpp"
#include <cstdio>

static void printUsage(const char *programName);

int main(int argc, const char *argv[])
{
    if (argc != 2 || (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0))
    {
        printUsage(argv[0]);
        return 0;
    }

    YAMLSceneRenderer::doRender(argv[1]);
    return 0;
}

// Prints usage for the program.
static void printUsage(const char *programName)
{
    fprintf(stdout, "usage: %s [YAML ray-tracer input file]\n", programName);
}
