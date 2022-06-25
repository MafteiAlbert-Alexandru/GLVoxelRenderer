#version 330 core
out vec4 FragColor;
uniform usampler2DArray input_tex;

// Vector to position of the camera
uniform vec3 pos;

// Unit vector of camera direction
uniform vec3 cam;

uniform vec3 camera_plan_vector;
uniform vec3 camera_plan_surface_right_vector;
uniform vec3 camera_plan_surface_up_vector;
uniform ivec2 screen_size;

bool is_in_bounds(in ivec3 point)
{
    ivec3 map_size=textureSize(input_tex, 0);
    return !(point.x >= map_size.x || point.y >=map_size.y || point.z >= map_size.z || point.x<0 || point.y<0 || point.z<0);
}

// TODO: Implement octree
uvec4 get_data(ivec3 point)
{
    return texelFetch(input_tex, point, 0);
}

bool _ray_hit_voxel;
ivec3 _ray_side_hit;
uvec4 _ray_voxel;



struct ray_results { 
    bool hit;
    uvec4 data;
    ivec3 normal;
    float distance;
};
// TODO(After #24): Implement octree traversal and spacial optimization
#define MAX_STEPS 256
ray_results ray(in vec3 pos, in vec3 dir)
{
    ray_results results;
    results.data=uvec4(0,0,0,0);
    results.hit=false;
    ivec3 map = ivec3(floor(pos));
    ivec3 step = ivec3(sign(dir));
    vec3 delta_dist = abs(1/dir);
    vec3 side_dist = (sign(dir)*(vec3(map)-pos+0.5)+0.5) * delta_dist;
    bvec3 mask;
    for(int i=0; !results.hit && i <=MAX_STEPS; i++)
    {
        if (side_dist.x < side_dist.y) {
			if (side_dist.x < side_dist.z) {
				side_dist.x += delta_dist.x;
				map.x += step.x;
				results.normal = ivec3(-1,0,0)*step;
			}
			else {
				side_dist.z += delta_dist.z;
				map.z += step.z;
				results.normal = ivec3(0,0,-1)*step;
			}
		}
		else {
            if (side_dist.y < side_dist.z) {
				side_dist.y += delta_dist.y;
				map.y += step.y;
				results.normal = ivec3(0,-1,0)*step;
			}
            else {
				side_dist.z += delta_dist.z;
				map.z += step.z;
				results.normal = ivec3(0,0,-1)*step;
			}
		}
        if(is_in_bounds(map))
        {
            results.data = get_data(map);
            if(results.data.x+results.data.y+results.data.z>0u)
            {
                results.hit=true;
                results.distance=length(results.normal*(side_dist-delta_dist));
            }
        }
    }
    return results;
}

#define REFLECTIONS 3

void main()
{


    float MAX_UINT=4294967295.0;
    vec2 camera_plan_position = (2*gl_FragCoord.xy/screen_size-vec2(1,1));
    vec3 camera_point = camera_plan_position.x*camera_plan_surface_right_vector + camera_plan_position.y*camera_plan_surface_up_vector;
    vec3 vector_to_point = camera_plan_vector+camera_point*800;

    ray_results results =ray(pos, normalize(vector_to_point));



    if(results.hit)
        FragColor=vec4(results.data.xyz/MAX_UINT, 1);
    else FragColor=vec4(0,0,0,1);
    
    
} 