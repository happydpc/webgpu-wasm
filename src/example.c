#define SPIDER_DEBUG 1
#include "spider.h"

vec3 plane_vertices[] = {
    {-0.5f,  0.0f,  0.5f},
    { 0.5f,  0.0f,  0.5f},
    { 0.5f,  0.0f, -0.5f},
    {-0.5f,  0.0f, -0.5f},
};

vec2 plane_tex_coords[] = {
    {0.0f, 0.0f},
    {1.0f, 0.0f},
    {1.0f, 1.0f},
    {0.0f, 1.0f}
};

SPTriangle plane_faces[] = {
    {
        .vertex_indices = {0, 1, 2},
        .tex_coord_indices = {0, 1, 2}
    },
    {
        .vertex_indices = {2, 3, 0},
        .tex_coord_indices = {2, 3, 0}
    },
};

const SPMeshInitializer plane = {
    .vertices = {
        .data = plane_vertices,
        .count = ARRAY_LEN(plane_vertices),
    },
    .tex_coords = {
        .data = plane_tex_coords,
        .count = ARRAY_LEN(plane_tex_coords),
    },
    .faces = {
        .data = plane_faces,
        .count = ARRAY_LEN(plane_faces)
    }
};

vec3 cube_vertices[] = {
    {-0.5f,  0.5f,  0.5f}, // LTB 0
    { 0.5f,  0.5f,  0.5f}, // RTB 1
    { 0.5f,  0.5f, -0.5f}, // RTF 2
    {-0.5f,  0.5f, -0.5f}, // LTF 3
    {-0.5f, -0.5f,  0.5f}, // LBB 4
    { 0.5f, -0.5f,  0.5f}, // RBB 5
    { 0.5f, -0.5f, -0.5f}, // RBF 6
    {-0.5f, -0.5f, -0.5f}, // LBF 7
};

vec2 cube_tex_coords[] = {
    {0.0f, 0.0f},
    {1.0f, 0.0f},
    {1.0f, 1.0f},
    {0.0f, 1.0f}
};

SPTriangle cube_faces[] = {
    // Top
    {
        .vertex_indices = {0, 1, 2},
        .tex_coord_indices = {0, 1, 2}
    },
    {
        .vertex_indices = {2, 3, 0},
        .tex_coord_indices = {2, 3, 0}
    },
    // Front
    {
        .vertex_indices = {1, 0, 4},
        .tex_coord_indices = {0, 1, 2}
    },
    {
        .vertex_indices = {4, 5, 1},
        .tex_coord_indices = {2, 3, 0}
    },
    // Left
    {
        .vertex_indices = {0, 3, 7},
        .tex_coord_indices = {0, 1, 2}
    },
    {
        .vertex_indices = {7, 4, 0},
        .tex_coord_indices = {2, 3, 0}
    },
    // Back
    {
        .vertex_indices = {3, 2, 6},
        .tex_coord_indices = {0, 1, 2}
    },
    {
        .vertex_indices = {6, 7, 3},
        .tex_coord_indices = {2, 3, 0}
    },
    // Right
    {
        .vertex_indices = {2, 1, 5},
        .tex_coord_indices = {0, 1, 2}
    },
    {
        .vertex_indices = {5, 6, 2},
        .tex_coord_indices = {2, 3, 0}
    },
    // Bottom
    {
        .vertex_indices = {6, 5, 4},
        .tex_coord_indices = {0, 1, 2}
    },
    {
        .vertex_indices = {4, 7, 6},
        .tex_coord_indices = {2, 3, 0}
    },
};

const SPMeshInitializer cube = {
    .vertices = {
        .data = cube_vertices,
        .count = ARRAY_LEN(cube_vertices),
    },
    .tex_coords = {
        .data = cube_tex_coords,
        .count = ARRAY_LEN(cube_tex_coords),
    },
    .faces = {
        .data = cube_faces,
        .count = ARRAY_LEN(cube_faces)
    }
};

#define INSTANCES_COUNT 40
SPInstanceID instance_ids[INSTANCES_COUNT];
SPLightID spot_light_id;
clock_t start_clock;

float randFloat(void) {
    return (float) rand() / (float) RAND_MAX;
}

float randFloatRange(float min, float max) {
    return min + randFloat() * (max - min);
} 

void createObjects(void) {
    SPMeshID mesh = spCreateMeshFromInit(&plane);
    SPMeshID mesh_cube = spCreateMeshFromInit(&cube);
    
    // TODO: lights have to be created before materials right now 
    vec3 light_direction = {0.0f, -1.0f, 0.0f};
    glm_vec3_normalize(light_direction);

    spot_light_id = spCreateSpotLight(&(SPSpotLightDesc){
            .pos = {0.0f, 6.0f, 0.0f},
            .range = 20.0f,
            .color = {.r = 255, .g = 255, .b = 255},
            .dir = {light_direction[0], light_direction[1], light_direction[2]},
            .fov = glm_rad(90.0f),
            .power = 150.0f,
            .shadow_casting = &(SPLightShadowCastDesc){
                .shadow_map_size = 1024,
            },
        }
    );
    SPIDER_ASSERT(spot_light_id.id != SP_INVALID_ID);

    SPMaterialID metal = spCreateMaterial(&(SPMaterialDesc){
            .albedo = {
                .name = "assets/textures/Metal003_2K/Metal003_2K_Color.jpg",
                .width = 2048,
                .height = 2048,
                .channel_count = 3,
            },
            .normal = {
                .name = "assets/textures/Metal003_2K/Metal003_2K_Normal.jpg",
                .width = 2048,
                .height = 2048,
                .channel_count = 3,
            },
            .roughness = {
                .name = "assets/textures/Metal003_2K/Metal003_2K_Roughness.jpg",
                .width = 2048,
                .height = 2048,
                .channel_count = 1,
            },
            .metallic = {
                .name = "assets/textures/Metal003_2K/Metal003_2K_Metalness.jpg",
                .width = 2048,
                .height = 2048,
                .channel_count = 1,
            },
        }
    );

    SPMaterialID marble = spCreateMaterial(&(SPMaterialDesc){
            .albedo = {
                .name = "assets/textures/Marble006_2K/Marble006_2K_Color.jpg",
                .width = 2048,
                .height = 2048,
                .channel_count = 3,
            },
            .normal = {
                .name = "assets/textures/Marble006_2K/Marble006_2K_Normal.jpg",
                .width = 2048,
                .height = 2048,
                .channel_count = 3,
            },
            .roughness = {
                .name = "assets/textures/Marble006_2K/Marble006_2K_Roughness.jpg",
                .width = 2048,
                .height = 2048,
                .channel_count = 1,
            },
        }
    );

    SPMaterialID bricks = spCreateMaterial(&(SPMaterialDesc){
            .albedo = {
                .name = "assets/textures/Bricks038_2K/Bricks038_2K_Color.jpg",
                .width = 2048,
                .height = 2048,
                .channel_count = 3,
            },
            .normal = {
                .name = "assets/textures/Bricks038_2K/Bricks038_2K_Normal.jpg",
                .width = 2048,
                .height = 2048,
                .channel_count = 3,
            },
            .roughness = {
                .name = "assets/textures/Bricks038_2K/Bricks038_2K_Roughness.jpg",
                .width = 2048,
                .height = 2048,
                .channel_count = 1,
            },
            .ao = {
                .name = "assets/textures/Bricks038_2K/Bricks038_2K_AmbientOcclusion.jpg",
                .width = 2048,
                .height = 2048,
                .channel_count = 1,
            },
        }
    );

    SPMaterialID planks = spCreateMaterial(&(SPMaterialDesc){
            .albedo = {
                .name = "assets/textures/Planks021_2K/Planks021_2K_Color.jpg",
                .width = 2048,
                .height = 2048,
                .channel_count = 3,
            },
            .normal = {
                .name = "assets/textures/Planks021_2K/Planks021_2K_Normal.jpg",
                .width = 2048,
                .height = 2048,
                .channel_count = 3,
            },
            .roughness = {
                .name = "assets/textures/Planks021_2K/Planks021_2K_Roughness.jpg",
                .width = 2048,
                .height = 2048,
                .channel_count = 1,
            },
            .ao = {
                .name = "assets/textures/Planks021_2K/Planks021_2K_AmbientOcclusion.jpg",
                .width = 2048,
                .height = 2048,
                .channel_count = 1,
            },
        }
    );
   
    const float spacing = 4.0f;

    SPMaterialID mats[] = {
        marble,
        metal,
        bricks,
        planks
    };

    for(int i = 0; i < INSTANCES_COUNT; i++) {
        float scale = randFloatRange(0.5f, 1.0f);
        instance_ids[i] = spCreateInstance(&(SPInstanceDesc){
                .mesh = mesh_cube, 
                .material = mats[rand() % ARRAY_LEN(mats)],
                .transform = &(SPTransform){
                    .pos = {randFloatRange(-spacing, spacing), randFloatRange(-spacing, spacing), randFloatRange(-spacing, spacing)},
                    .scale = {scale, scale, scale},
                    .rot = {randFloatRange(0.0f, 360.0f), randFloatRange(0.0f, 360.0f), randFloatRange(0.0f, 360.0f)},
                }
            }
        );
    }

    spCreateInstance(&(SPInstanceDesc){
            .mesh = mesh, 
            .material = bricks,
            .transform = &(SPTransform){
                .pos = {0.0f, -spacing, 0.0f},
                .scale = {spacing * 4.0f, 1.0f, spacing * 4.0f},
                .rot = {0.0f, 0.0f, 0.0f},
            }
        }
    );
}

float time_elapsed_total_s = 0.0f;

void frame(void) {
    clock_t cur_clock = clock();
    float delta_time_s = ((float)(cur_clock - start_clock) / CLOCKS_PER_SEC);
    start_clock = clock();
    time_elapsed_total_s += delta_time_s;
    for(int i = 0; i < INSTANCES_COUNT; i++) {
        SPInstance* instance = spGetInstance(instance_ids[i]);
        if(!instance) {
            continue;
        }
        instance->transform.rot[1] += (180.0f / INSTANCES_COUNT) * i * delta_time_s;
        if(instance->transform.rot[1] >= 360.0f) {
            instance->transform.rot[1] -= 360.0f; 
        }
    }
    SPCamera* camera = spGetActiveCamera();
    const float radius = 10.0f;
    const float depth = sin(time_elapsed_total_s * 0.12f) * 6.0f;

    SPLight* spot_light = spGetLight(spot_light_id);
    float angle = sin(glm_rad(30.0f));
    if(spot_light) {
        spot_light->pos[0] = sin(time_elapsed_total_s * 0.4f) * 2.0f;
        spot_light->pos[2] = sin(time_elapsed_total_s * 1.0f) * 5.0f;
    }

    spUpdate();
    spRender();
}

int main() {
    srand(0);
    const uint16_t surface_width = 1280;
    const uint16_t surface_height = 720;
    vec3 dir = {0.0f, -1.0f, -1.0f};
    vec3 pos = {0.0f, 5.0f, 8.0f};
    vec3 center = {0.0f, -4.0f, 0.0f};
    //glm_vec3_sub(center, pos, dir);
    glm_vec3_normalize(dir);

    SPInitDesc init = {
        .surface_size = {
            .width = surface_width,
            .height = surface_height
        },
        .camera = {
            .pos = {pos[0], pos[1], pos[2]},
            .dir = {dir[0], dir[1], dir[2]},
            .look_at = {center[0], center[1], center[2]},
            .mode = SPCameraMode_Direction,
            .fovy = glm_rad(60.0f),
            .aspect = (float)surface_width / (float) surface_height,
            .near = 0.1f,
            .far = 100.0f
        },
        .pools.capacities = {
            .meshes = 8,
            .materials = 8,
            .instances = 4096,
            .lights = 1,
        },
    };
    spInit(&init);
    createObjects();
    start_clock = clock();

    emscripten_set_main_loop(frame, 60, false);
    return 0;
}