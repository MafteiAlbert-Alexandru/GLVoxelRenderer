#ifndef WORLDGEN_H
#define WORLDGEN_H
#include <PerlinNoise.hpp>
#include "texture3d.hpp"
class WorldGen
{
public:
    static void generate(Texture3D *tex, long double density, int octaves, siv::PerlinNoise::seed_type seed = 0)
    {
        const siv::BasicPerlinNoise<float> perlin{seed};
        for(int i=0;i<tex->m_width;i++)
            for(int j=0;j<tex->m_height;j++)
                for(int k=0;k<tex->m_depth;k++)
                    if(perlin.octave3D_01(i*0.01, j*0.01, k*0.01, octaves)>density)
                        
                        {
                            tex->setTexel(
                                {(unsigned int)(perlin.octave3D_01((i*0.02), (j*0.01), (k*0.01), octaves)*0xffffffff),
                                 (unsigned int)(perlin.octave3D_01((i*0.01), (j*0.02), (k*0.01), octaves)*0xffffffff),
                                  (unsigned int)(perlin.octave3D_01((i*0.01), (j*0.01), (k*0.02), octaves)*0xffffffff), 
                                  0xffffffff
                                  }, 
                                  glm::ivec3(i,j,k));
                       
                        }

    }
};
#endif