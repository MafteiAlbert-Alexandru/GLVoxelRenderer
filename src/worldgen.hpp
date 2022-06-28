#ifndef WORLDGEN_H
#define WORLDGEN_H
#include <PerlinNoise.hpp>
#include "texture3d.hpp"
#include <cstdlib>
#include "octree.hpp"
#include <glm/vec3.hpp>
class WorldGen
{
public:
    static void generate(Texture3D *tex, long double density, int octaves, siv::PerlinNoise::seed_type seed = 0)
    {
        seed = time(NULL);
        const siv::BasicPerlinNoise<float> perlin{seed};
        for (int i = 0; i < tex->m_width; i++)
            for (int j = 0; j < tex->m_height; j++)
                for (int k = 0; k < tex->m_depth; k++)
                    if (perlin.octave3D_01(i * 0.01, j * 0.01, k * 0.01, octaves) > density)

                    {
                        tex->setTexel(
                            {(unsigned int)(perlin.octave3D_01((i * 0.02), (j * 0.01), (k * 0.01), octaves) * 0xffffffff),
                             (unsigned int)(perlin.octave3D_01((i * 0.01), (j * 0.02), (k * 0.01), octaves) * 0xffffffff),
                             (unsigned int)(perlin.octave3D_01((i * 0.01), (j * 0.01), (k * 0.02), octaves) * 0xffffffff),
                             0xffffffff},
                            glm::ivec3(i, j, k));
                    }
    }
    static void generate(Octree *octree, long double density, int octaves, siv::PerlinNoise::seed_type seed = 0)
    {
        seed = time(NULL);
        glm::ivec3 size = octree->size();
        const siv::BasicPerlinNoise<float> perlin{seed};
        for (int i = 0; i < size.x; i++)
            for (int j = 0; j < size.y; j++)
                for (int k = 0; k < size.z; k++)
                    if (perlin.octave3D_01(i * 0.01, j * 0.01, k * 0.01, octaves) > density)

                    {
                                octree->set(
                                    {(unsigned char)(perlin.octave3D_01((i*0.02), (j*0.01), (k*0.01), octaves)*0xff),
                                 (unsigned char)(perlin.octave3D_01((i*0.01), (j*0.02), (k*0.01), octaves)*0xff),
                                  (unsigned char)(perlin.octave3D_01((i*0.01), (j*0.01), (k*0.02), octaves)*0xff), 
                                  0xff
                                  }, 
                                  glm::ivec3(i,j,k));
                    }
    }
};
#endif