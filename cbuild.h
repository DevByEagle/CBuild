#ifndef CBUILD_H
#define CBUILD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <json-c/json.h> // Ensure you have the json-c library installed

typedef struct {
    char *name;
    char *build_command;
} Target;

typedef struct {
    Target *targets;
    int target_count;
} BuildConfig;

BuildConfig load_config(const char *filename) {
    BuildConfig config = {0};
    struct json_object *parsed_json;
    struct json_object *targets_json;
    size_t target_count;

    // Load and parse the JSON file
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Unable to open config file");
        return config;
    }
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *data = malloc(length + 1);
    fread(data, 1, length, file);
    fclose(file);
    data[length] = '\0';

    parsed_json = json_tokener_parse(data);
    json_object_object_get_ex(parsed_json, "targets", &targets_json);
    target_count = json_object_array_length(targets_json);
    
    // Allocate memory for targets
    config.targets = malloc(sizeof(Target) * target_count);
    config.target_count = target_count;

    for (size_t i = 0; i < target_count; i++) {
        struct json_object *target_json = json_object_array_get_idx(targets_json, i);
        struct json_object *name_json;
        struct json_object *command_json;

        json_object_object_get_ex(target_json, "name", &name_json);
        json_object_object_get_ex(target_json, "command", &command_json);
        
        config.targets[i].name = strdup(json_object_get_string(name_json));
        config.targets[i].build_command = strdup(json_object_get_string(command_json));
    }
    
    free(data);
    return config;
}

void run_command(const char *command) {
    printf("Running command: %s\n", command);
    int result = system(command);
    if (result == -1) {
        perror("Error executing command");
    }
}

int build_target(BuildConfig config, const char *target) {
    // Check if target exists in the configuration
    for (int i = 0; i < config.target_count; i++) {
        if (strcmp(config.targets[i].name, target) == 0) {
            // Run the build command for the target
            run_command(config.targets[i].build_command);
            return 1; // Success
        }
    }
    return 0; // Target not found
}

void free_config(BuildConfig config) {
    for (int i = 0; i < config.target_count; i++) {
        free(config.targets[i].name);
        free(config.targets[i].build_command);
    }
    free(config.targets);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: CBuild <target>\n");
        return 1;
    }

    // Load configuration
    BuildConfig config = load_config("CBuild.json");
    
    // Build the specified target
    if (!build_target(config, argv[1])) {
        fprintf(stderr, "Error building target: %s\n", argv[1]);
        free_config(config);
        return 1;
    }

    printf("Build completed successfully!\n");
    free_config(config);
    return 0;
}

#endif // CBUILD_H