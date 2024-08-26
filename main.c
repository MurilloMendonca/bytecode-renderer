#include "raylib/include/raylib.h"
#include <stdio.h>

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

struct RenderCommand commands[] = {
    {.type= DRAW_RECT, .args=
        {.rect.begin={50,50}, 
         .rect.dimention={100,100}, 
         .rect.color=GREEN}
    },
    {.type=DRAW_LINE, .args=
        {.line.origin={0,0},
         .line.end={300,300},
         .line.color=RED}
    },
    {.type=DRAW_X, .args=
        {.x.begin={200,250},
         .x.size=50, // Adjusted to a more reasonable size
         .x.color=RED}
    }
};

void svg_render(const char* filename, struct RenderCommand comms[], int size) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        fprintf(stderr, "Error opening file for writing\n");
        return;
    }
    
    fprintf(file, "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"800\" height=\"800\">\n");

    for (int i = 0; i < size; i++) {
        struct RenderCommand com = comms[i];
        switch (com.type) {
            case DRAW_RECT:
                fprintf(file, "<rect x=\"%f\" y=\"%f\" width=\"%f\" height=\"%f\" fill=\"rgb(%d,%d,%d)\" />\n",
                        com.args.rect.begin.x, com.args.rect.begin.y,
                        com.args.rect.dimention.x, com.args.rect.dimention.y,
                        com.args.rect.color.r, com.args.rect.color.g, com.args.rect.color.b);
                break;
            case DRAW_LINE:
                fprintf(file, "<line x1=\"%f\" y1=\"%f\" x2=\"%f\" y2=\"%f\" stroke=\"rgb(%d,%d,%d)\" stroke-width=\"2\" />\n",
                        com.args.line.origin.x, com.args.line.origin.y,
                        com.args.line.end.x, com.args.line.end.y,
                        com.args.line.color.r, com.args.line.color.g, com.args.line.color.b);
                break;
            case DRAW_X:
                fprintf(file, "<line x1=\"%f\" y1=\"%f\" x2=\"%f\" y2=\"%f\" stroke=\"rgb(%d,%d,%d)\" stroke-width=\"2\" />\n",
                        com.args.x.begin.x - com.args.x.size / 2, com.args.x.begin.y - com.args.x.size / 2,
                        com.args.x.begin.x + com.args.x.size / 2, com.args.x.begin.y + com.args.x.size / 2,
                        com.args.x.color.r, com.args.x.color.g, com.args.x.color.b);
                fprintf(file, "<line x1=\"%f\" y1=\"%f\" x2=\"%f\" y2=\"%f\" stroke=\"rgb(%d,%d,%d)\" stroke-width=\"2\" />\n",
                        com.args.x.begin.x - com.args.x.size / 2, com.args.x.begin.y + com.args.x.size / 2,
                        com.args.x.begin.x + com.args.x.size / 2, com.args.x.begin.y - com.args.x.size / 2,
                        com.args.x.color.r, com.args.x.color.g, com.args.x.color.b);
                break;
        }
    }

    fprintf(file, "</svg>\n");
    fclose(file);
}

void raylib_render(struct RenderCommand comms[], int size) {
    InitWindow(800, 800, "Hello World");
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(GRAY);
        for (int i = 0; i < size; i++) {
            struct RenderCommand com = comms[i];
            switch (com.type) {
                case DRAW_RECT:
                    DrawRectangleV(com.args.rect.begin,
                                   com.args.rect.dimention,
                                   com.args.rect.color);
                    break;
                case DRAW_LINE:
                    DrawLineV(com.args.line.origin,
                              com.args.line.end,
                              com.args.line.color);
                    break;
                case DRAW_X:
                    // Corrected to use com.args.x.size
                    DrawLine(com.args.x.begin.x - com.args.x.size / 2,
                             com.args.x.begin.y - com.args.x.size / 2,
                             com.args.x.begin.x + com.args.x.size / 2,
                             com.args.x.begin.y + com.args.x.size / 2,
                             com.args.x.color);
                    DrawLine(com.args.x.begin.x - com.args.x.size / 2,
                             com.args.x.begin.y + com.args.x.size / 2,
                             com.args.x.begin.x + com.args.x.size / 2,
                             com.args.x.begin.y - com.args.x.size / 2,
                             com.args.x.color);
                    break;
            }
        }
        EndDrawing();
    }
    CloseWindow();
}

int main() {
    raylib_render(commands, sizeof(commands) / sizeof(commands[0]));
    svg_render("teste.svg", commands, sizeof(commands) / sizeof(commands[0]));
    return 0;
}

