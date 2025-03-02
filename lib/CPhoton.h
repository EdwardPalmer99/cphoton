/**
 * @file CPhoton.h
 * @author Edward Palmer
 * @date 2025-02-05
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include "engine/HitRec.h"
#include "engine/Material.h"
#include "engine/Primitive.h"
#include "engine/Ray.h"
#include "engine/RenderSettings.h"
#include "engine/Scene.h"
#include "engine/Texture.h"
#include "logger/Logger.h"
#include "models/Fractals3D.h"
#include "models/MengerCube.h"
#include "models/PolyhedralModels.h"
#include "threadpool/ThreadPool.h"
#include "threadpool/ThreadTask.h"
#include "utility/Matrix3.h"
#include "utility/PPMWriter.h"
#include "utility/Utility.h"
#include "utility/Vector3.h"