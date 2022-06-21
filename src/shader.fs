#version 330 core
out vec4 FragColor;
uniform usampler2DArray input_tex;

// Vector to position of the camera
uniform vec3 pos;

// Unit vector of camera direction
uniform vec3 cam;

// Requested FOV
uniform int FOV;

uniform ivec2 screen_size;
uniform ivec3 map_size;
void main()
{

    // TODO: Move calculation outside of shader and into precalculation, or constantize
    vec3 camera_plan_vector = cam * (screen_size.x/2.0)*tan(radians(FOV/2.0));
    vec3 camera_plan_surface_right_vector = normalize(cross(vec3(0,-1,0), camera_plan_vector));
    vec3 camera_plan_surface_up_vector = normalize(cross(camera_plan_surface_right_vector, camera_plan_vector));

    float MAX_UINT=4294967295.0;
    vec2 camera_plan_position = (2*gl_FragCoord.xy/screen_size-vec2(1,1));

    vec3 camera_point = camera_plan_position.x*camera_plan_surface_right_vector + camera_plan_position.y*camera_plan_surface_up_vector;

    vec3 vector_to_point = camera_plan_vector+camera_point*800;
    vec3 ray = normalize(vector_to_point);
    
    bool found = false;
    ivec3 map = ivec3(pos);
    ivec3 step = ivec3(1,1,1);
    vec3 side_dist;
    
    if(ray.x<0) 
        step.x=-1;
    if(ray.y<0) 
        step.y=-1;
    if(ray.z<0) 
        step.z=-1;
    
    vec3 next_voxel_boundry = map+step;
    vec3 t_max = (next_voxel_boundry-pos)/ray;
    vec3 t_delta = 1/ray*step;
    uvec4 color;
    int steps=0;
    if(gl_FragCoord.x<64&& gl_FragCoord.y<64)
    {
        if(abs(gl_FragCoord.xy/2-pos.xz).x<1 && abs(gl_FragCoord.xy/2-pos.xz).y <1)
            FragColor=vec4(0,1,0,1);
        else FragColor = vec4(texelFetch(input_tex, ivec3(gl_FragCoord.xy/2, pos.y),
         0).xyz/MAX_UINT,1);
    }else {
        //FragColor=vec4(ray, 1);
        

while(!found&&steps<=128)
    {
        steps++;
        if(t_max.x<t_max.y)
        {
            if(t_max.x<t_max.z) {
                map.x+=step.x;
                t_max.x += t_delta.x;
            }else {
                map.z +=step.z;
                t_max.z+=t_delta.z;
            }
        }else {
            if(t_max.y < t_max.z) {
                map.y+=step.y;
                t_max.y+=t_delta.y;
            }else {
                map.z+=step.z;
                t_max.z+=t_delta.z;
            }
        }
        if(!(map.x >= map_size.x || map.y >=map_size.y || map.z >= map_size.z || map.x<0 || map.y<0 || map.z<0))
        {
            color = texelFetch(input_tex, map, 0);
            if(color.x+color.y+color.z>0u)
                found=true;
        }
    }
    if(found)
        FragColor=vec4(color.x/MAX_UINT,color.y/MAX_UINT,color.z/MAX_UINT,1);
    else FragColor=vec4(1,0,0,1);
    }
    
} 