#include <stdio.h>
#include <stdlib.h>
#include <math.h>
typedef struct {
    float x, y, z;
} Vertex;

typedef struct {
    float u, v;
} UV;

typedef struct {
    float x, y, z;
} Normal;

typedef struct {
    int vertex_index;
    int uv_index;
    int normal_index;
} FaceVertex;

int main() {
    FILE *file = fopen("capsule.obj", "r");
    if (file == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    Vertex *vertices = NULL;
    UV *uvs = NULL;
    Normal *normals = NULL;
    FaceVertex *faceVertices = NULL;

    int vertexCount = 0, uvCount = 0, normalCount = 0, faceCount = 0;

    char line[128];
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == 'v' && line[1] == ' ') {
            vertices = realloc(vertices, (vertexCount + 1) * sizeof(Vertex));
            if (vertices == NULL) {
                perror("Memory allocation failed");
                return EXIT_FAILURE;
            }
            sscanf(line, "v %f %f %f", &vertices[vertexCount].x, &vertices[vertexCount].y, &vertices[vertexCount].z);
            vertexCount++;
        } else if (line[0] == 'v' && line[1] == 't') {
            uvs = realloc(uvs, (uvCount + 1) * sizeof(UV));
            if (uvs == NULL) {
                perror("Memory allocation failed");
                return EXIT_FAILURE;
            }
            sscanf(line, "vt %f %f", &uvs[uvCount].u, &uvs[uvCount].v);
            uvCount++;
        } else if (line[0] == 'v' && line[1] == 'n') {
            normals = realloc(normals, (normalCount + 1) * sizeof(Normal));
            if (normals == NULL) {
                perror("Memory allocation failed");
                return EXIT_FAILURE;
            }
            sscanf(line, "vn %f %f %f", &normals[normalCount].x, &normals[normalCount].y, &normals[normalCount].z);
            normalCount++;
        } else if (line[0] == 'f' && line[1] == ' ') {
            faceVertices = realloc(faceVertices, (faceCount + 3) * sizeof(FaceVertex));
            if (faceVertices == NULL) {
                perror("Memory allocation failed");
                return EXIT_FAILURE;
            }
            sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
                   &faceVertices[faceCount].vertex_index, &faceVertices[faceCount].uv_index, &faceVertices[faceCount].normal_index,
                   &faceVertices[faceCount + 1].vertex_index, &faceVertices[faceCount + 1].uv_index, &faceVertices[faceCount + 1].normal_index,
                   &faceVertices[faceCount + 2].vertex_index, &faceVertices[faceCount + 2].uv_index, &faceVertices[faceCount + 2].normal_index);
            faceCount += 3;
        }
    }

    FILE *outputFile = fopen("output.txt", "w");
    if (outputFile == NULL) {
        perror("Error creating output file");
        return EXIT_FAILURE;
    }
    // Output the extracted data at respective face indices
    fprintf(outputFile, "{\n");
    for (int i = 0; i < faceCount; ++i) {
        fprintf(outputFile, "    { {%f, %f, %f},", i + 1, vertices[faceVertices[i].vertex_index - 1].x, vertices[faceVertices[i].vertex_index - 1].y, vertices[faceVertices[i].vertex_index - 1].z);
        fprintf(outputFile, " {%f, %f},", i + 1, uvs[faceVertices[i].uv_index - 1].u, uvs[faceVertices[i].uv_index - 1].v);
        fprintf(outputFile, " {%f, %f, %f} },\n", i + 1, normals[faceVertices[i].normal_index - 1].x, normals[faceVertices[i].normal_index - 1].y, normals[faceVertices[i].normal_index - 1].z);
    }
    fprintf(outputFile, "}");

    // Free allocated memory
    free(vertices);
    free(uvs);
    free(normals);
    free(faceVertices);

    fclose(file);
    return EXIT_SUCCESS;
}
