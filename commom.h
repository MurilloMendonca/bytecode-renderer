#ifndef _COMMOM_H_
#include "raylib/include/raylib.h"
enum RENDER_COMMAND_TYPE {
    DRAW_RECT = 1,
    DRAW_SQR,
    DRAW_LINE,
    DRAW_SPRITE,
    DRAW_X
};

struct RenderXArgs {
    Vector2 begin;
    int size;
    Color color;
};

struct RenderRectArgs {
    Vector2 begin;
    Vector2 dimention;
    Color color;
};
struct RenderSqrArgs {
    Vector2 begin;
    float size;
    Color color;
};
struct RenderLineArgs {
    Vector2 origin;
    Vector2 end;
    Color color;
};

union RenderCommandUnion {
    struct RenderRectArgs rect;
    struct RenderSqrArgs sqr;
    struct RenderLineArgs line;
    struct RenderXArgs x;
};

struct RenderCommand {
    enum RENDER_COMMAND_TYPE type;
    union RenderCommandUnion args;
};
#endif
