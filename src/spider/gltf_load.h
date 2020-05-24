#ifndef SPIDER_GLTF_LOAD_H_
#define SPIDER_GLTF_LOAD_H_

typedef struct cgltf_data cgltf_data;

#include "mesh.h"
#include "material.h"
#include "instance.h"

/*
Loads mesh and material from a .gltf file, creates respective mesh and material from the data and 
returns an object with identifiers to the mesh/material
*/
SPObject spLoadGltf(const char* file);

/*
Creates a mesh from the loaded gltf file (it loads the buffers) and returns an ID to it
*/
SPMeshID _spLoadMeshFromGltf(cgltf_data* data, const char* gltf_path);
/*
Creates a material from the loaded gltf file (it loads the images) and returns an ID to it
*/
SPMaterialID _spLoadMaterialFromGltf(const cgltf_data* data, const char* gltf_path);

/*
Combines two paths and saves it in result (used for loading gltf materials that store images relative to the base file)
Example:
base: foo/bar/xyz.txt
new: abc.bin
result: foo/bar/abc.bin
*/
void _spModifyRelativeFilePath(const char* base_path, const char* new_path, char* result);

#endif // SPIDER_GLTF_LOAD_H_