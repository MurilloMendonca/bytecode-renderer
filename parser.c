#include "commom.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_LINE_LENGTH 100

struct RenderCommand* parse_command_line(const char* line, int* command_count) {
    char command[MAX_LINE_LENGTH];
    struct RenderCommand* commands = malloc(sizeof(struct RenderCommand));
    if (!commands) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }

    *command_count = 0;

    while (fgets(command, MAX_LINE_LENGTH, stdin)) {
        struct RenderCommand com;
        char type[10];
        sscanf(command, "%s", type);

        if (strcmp(type, "RECT") == 0) {
            com.type = DRAW_RECT;
            sscanf(command, "%*s %f %f %f %f %hhu %hhu %hhu %hhu",
                   &com.args.rect.begin.x, &com.args.rect.begin.y,
                   &com.args.rect.dimention.x, &com.args.rect.dimention.y,
                   &com.args.rect.color.r, &com.args.rect.color.g, &com.args.rect.color.b,&com.args.rect.color.a);
        } else if (strcmp(type, "SQR") == 0) {
            com.type = DRAW_SQR;
            sscanf(command, "%*s %f %f %f %hhu %hhu %hhu %hhu",
                   &com.args.sqr.begin.x, &com.args.sqr.begin.y,
                   &com.args.sqr.size,
                   &com.args.sqr.color.r, &com.args.sqr.color.g, &com.args.sqr.color.b, &com.args.sqr.color.a);
        } else if (strcmp(type, "LINE") == 0) {
            com.type = DRAW_LINE;
            sscanf(command, "%*s %f %f %f %f %hhu %hhu %hhu %hhu",
                   &com.args.line.origin.x, &com.args.line.origin.y,
                   &com.args.line.end.x, &com.args.line.end.y,
                   &com.args.line.color.r, &com.args.line.color.g, &com.args.line.color.b, &com.args.line.color.a);
        } else if (strcmp(type, "X") == 0) {
            com.type = DRAW_X;
            sscanf(command, "%*s %f %f %i %hhu %hhu %hhu %hhu",
                   &com.args.x.begin.x, &com.args.x.begin.y,
                   &com.args.x.size,
                   &com.args.x.color.r, &com.args.x.color.g, &com.args.x.color.b, &com.args.x.color.a);
        } else {
            continue; // Ignore unrecognized commands
        }

        commands = realloc(commands, (*command_count + 1) * sizeof(struct RenderCommand));
        if (!commands) {
            fprintf(stderr, "Memory allocation failed\n");
            return NULL;
        }
        commands[*command_count] = com;
        (*command_count)++;
    }

    return commands;
}
void save_commands_to_binary(const char* filename, struct RenderCommand comms[], int size) {
    FILE* file = fopen(filename, "wb");
    if (!file) {
        fprintf(stderr, "Error opening file for writing\n");
        return;
    }

    // Write the size of the array first
    fwrite(&size, sizeof(int), 1, file);

    // Write each command to the binary file
    for (int i = 0; i < size; i++) {
        fwrite(&comms[i], sizeof(struct RenderCommand), 1, file);
    }

    fclose(file);
}

int main(int argc, char** argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_text_file> <output_binary_file>\n", argv[0]);
        return 1;
    }

    const char* input_filename = argv[1];
    const char* output_filename = argv[2];

    FILE* input_file = fopen(input_filename, "r");
    if (!input_file) {
        fprintf(stderr, "Error opening input file: %s\n", input_filename);
        return 1;
    }

    stdin = input_file;

    int command_count;
    struct RenderCommand* commands = parse_command_line(NULL, &command_count);

    fclose(input_file);

    if (!commands) {
        fprintf(stderr, "Failed to parse commands.\n");
        return 1;
    }

    save_commands_to_binary(output_filename, commands, command_count);

    free(commands);

    printf("Commands successfully compiled to %s\n", output_filename);

    return 0;
}
