#include "raylib/include/raylib.h"
#include <stdio.h>
#include "commom.h"
#include <stdlib.h>
#include <string.h>


struct RenderCommand example_commands[] = {
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
         .x.size=50,
         .x.color=RED}
    }
};

void tikz_render(const char* filename, struct RenderCommand comms[], int size) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        fprintf(stderr, "Error opening file for writing\n");
        return;
    }

    const float canvas_height = 8.0;

    fprintf(file, "\\documentclass{standalone}\n");
    fprintf(file, "\\usepackage{tikz}\n");
    fprintf(file, "\\begin{document}\n");
    fprintf(file, "\\begin{tikzpicture}\n");

    for (int i = 0; i < size; i++) {
        struct RenderCommand com = comms[i];
        switch (com.type) {
            case DRAW_RECT:
                fprintf(file, "\\fill[fill={rgb,255:red,%d;green,%d;blue,%d}] (%.2f, %.2f) rectangle (%.2f, %.2f);\n",
                        com.args.rect.color.r, com.args.rect.color.g, com.args.rect.color.b,
                        com.args.rect.begin.x / 100.0, (canvas_height - com.args.rect.begin.y / 100.0),
                        (com.args.rect.begin.x + com.args.rect.dimention.x) / 100.0,
                        (canvas_height - (com.args.rect.begin.y + com.args.rect.dimention.y) / 100.0));
                break;
            case DRAW_LINE:
                fprintf(file, "\\draw[line width=0.5mm, color={rgb,255:red,%d;green,%d;blue,%d}] (%.2f, %.2f) -- (%.2f, %.2f);\n",
                        com.args.line.color.r, com.args.line.color.g, com.args.line.color.b,
                        com.args.line.origin.x / 100.0, (canvas_height - com.args.line.origin.y / 100.0),
                        com.args.line.end.x / 100.0, (canvas_height - com.args.line.end.y / 100.0));
                break;
            case DRAW_X:
                fprintf(file, "\\draw[line width=0.5mm, color={rgb,255:red,%d;green,%d;blue,%d}] (%.2f, %.2f) -- (%.2f, %.2f);\n",
                        com.args.x.color.r, com.args.x.color.g, com.args.x.color.b,
                        (com.args.x.begin.x - com.args.x.size / 2) / 100.0, (canvas_height - (com.args.x.begin.y - com.args.x.size / 2) / 100.0),
                        (com.args.x.begin.x + com.args.x.size / 2) / 100.0, (canvas_height - (com.args.x.begin.y + com.args.x.size / 2) / 100.0));
                fprintf(file, "\\draw[line width=0.5mm, color={rgb,255:red,%d;green,%d;blue,%d}] (%.2f, %.2f) -- (%.2f, %.2f);\n",
                        com.args.x.color.r, com.args.x.color.g, com.args.x.color.b,
                        (com.args.x.begin.x - com.args.x.size / 2) / 100.0, (canvas_height - (com.args.x.begin.y + com.args.x.size / 2) / 100.0),
                        (com.args.x.begin.x + com.args.x.size / 2) / 100.0, (canvas_height - (com.args.x.begin.y - com.args.x.size / 2) / 100.0));
                break;
        }
    }

    fprintf(file, "\\end{tikzpicture}\n");
    fprintf(file, "\\end{document}\n");
    fclose(file);
}
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
        ClearBackground(WHITE);
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

struct RenderCommand* read_commands_from_binary(const char* filename, int* size) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Error opening file for reading\n");
        return NULL;
    }

    // Read the size of the array first
    fread(size, sizeof(int), 1, file);

    // Allocate memory for the commands
    struct RenderCommand* comms = (struct RenderCommand*)malloc((*size) * sizeof(struct RenderCommand));
    if (!comms) {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(file);
        return NULL;
    }

    // Read each command from the binary file
    fread(comms, sizeof(struct RenderCommand), *size, file);

    fclose(file);
    return comms;
}

int main(int argc, char** argv) {
    struct RenderCommand* loaded_commands;
    int size = 0;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <subcommand> [<input_binary_file>]\n", argv[0]);
        fprintf(stderr, "Subcommands:\n");
        fprintf(stderr, "  run-tikz    - Render using TikZ\n");
        fprintf(stderr, "  run-svg     - Render using SVG\n");
        fprintf(stderr, "  run-raylib  - Render using Raylib\n");
        fprintf(stderr, "  run-all     - Render using all renderers\n");
        return 1;
    }

    const char* subcommand = argv[1];

    if (argc > 2) {
        loaded_commands = read_commands_from_binary(argv[2], &size);
        if (!loaded_commands) {
            fprintf(stderr, "Failed to load commands from binary file: %s\n", argv[2]);
            return 1;
        }
    } else {
        // Use example commands if no binary file is provided
        loaded_commands = example_commands;
        size = sizeof(example_commands) / sizeof(example_commands[0]);
    }

    if (strcmp(subcommand, "run-tikz") == 0) {
        tikz_render("output.tex", loaded_commands, size);
        printf("Rendered using TikZ and saved to output.tex\n");
    } else if (strcmp(subcommand, "run-svg") == 0) {
        svg_render("output.svg", loaded_commands, size);
        printf("Rendered using SVG and saved to output.svg\n");
    } else if (strcmp(subcommand, "run-raylib") == 0) {
        raylib_render(loaded_commands, size);
        printf("Rendered using Raylib\n");
    } else if (strcmp(subcommand, "run-all") == 0) {
        tikz_render("output.tex", loaded_commands, size);
        svg_render("output.svg", loaded_commands, size);
        raylib_render(loaded_commands, size);
        printf("Rendered using all renderers and saved outputs\n");
    } else {
        fprintf(stderr, "Unknown subcommand: %s\n", subcommand);
        fprintf(stderr, "Available subcommands: run-tikz, run-svg, run-raylib, run-all\n");
        if (loaded_commands != example_commands) {
            free(loaded_commands);
        }
        return 1;
    }

    if (loaded_commands != example_commands) {
        free(loaded_commands);
    }

    return 0;
}

