#pragma once

#include "../engine/core/engine.h"
#include "../engine/render/camera.h"
#include "../engine/render/command_buffer.h"

#include "../engine/nodes/node.h"
#include "../engine/nodes/node_ecs.h"

#define NANOVG_GL3_IMPLEMENTATION
#include "nanovg.h"
#include "nanovg_gl.h"

namespace game {
    namespace e = engine;
    namespace er = engine::render;
    namespace ecs = engine::ecs;
}