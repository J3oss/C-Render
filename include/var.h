#pragma once

#define WIDTH 800
#define HEIGHT 800
#define DEPTH 255

enum class FocusType {SCENES, CAMERAS, MESHES, LIGHTS};

#define CLAMP(x,lo,hi) { x = x < lo ? lo : x > hi ? hi : x}
