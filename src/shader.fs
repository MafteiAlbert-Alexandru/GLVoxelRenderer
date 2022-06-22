#version 330 core
out vec4 FragColor;
uniform usampler2DArray input_tex;

// Vector to position of the camera
uniform vec3 pos;

// Unit vector of camera direction
uniform vec3 cam;

// Requested FOV
uniform vec3 camera_plan_vector;
uniform vec3 camera_plan_surface_right_vector;
uniform vec3 camera_plan_surface_up_vector;
uniform ivec2 screen_size;
uniform ivec3 map_size;
void main()
{


    float MAX_UINT=4294967295.0;
    vec2 camera_plan_position = (2*gl_FragCoord.xy/screen_size-vec2(1,1));

    vec3 camera_point = camera_plan_position.x*camera_plan_surface_right_vector + camera_plan_position.y*camera_plan_surface_up_vector;

    vec3 vector_to_point = camera_plan_vector+camera_point*800;
    vec3 ray = normalize(vector_to_point);
    
    bool found = false;
    ivec3 map = ivec3(floor(pos));
    ivec3 step = ivec3(sign(ray));
    vec3 delta_dist = abs(1/ray);
    vec3 side_dist = (sign(ray) * (vec3(map)-pos)+sign(ray)*0.5 + 0.5) * delta_dist;
    uvec4 color;
    bvec3 mask;
    for(int i=0;!found&&i<=256;i++)
    {
        if (side_dist.x < side_dist.y) {
				if (side_dist.x < side_dist.z) {
					side_dist.x += delta_dist.x;
					map.x += step.x;
					mask = bvec3(true, false, false);
				}
				else {
					side_dist.z += delta_dist.z;
					map.z += step.z;
				    mask = bvec3(false, false, true);
				}
			}
			else {
				if (side_dist.y < side_dist.z) {
					side_dist.y += delta_dist.y;
					map.y += step.y;
					mask = bvec3(false, true, false);
				}
				else {
					side_dist.z += delta_dist.z;
					map.z += step.z;
					mask = bvec3(false, false, true);
				}
			}
            if(!(map.x >= map_size.x || map.y >=map_size.y || map.z >= map_size.z || map.x<0 || map.y<0 || map.z<0))
        {
            
            
            color = texelFetch(input_tex, map, 0);
            if(color.x+color.y+color.z>0u)
                found=true;
        }
    }
    const vec3 colors = vec3(0.3,0.2,0.4);
    vec3 product =  vec3(mask)*colors;
    vec3 len = vec3(mask)*(side_dist-delta_dist);
    
    float blend = product.x+product.y+product.z+length(len)/140.0;
    
    if(found)
        FragColor=mix(vec4(color.x/MAX_UINT,color.y/MAX_UINT,color.z/MAX_UINT,1), vec4(0,0,0,1), blend);
    else FragColor=vec4(0,0,0,1);
    
    
} 