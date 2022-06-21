#version 330 core
out vec4 FragColor;
uniform usampler2DArray input_tex;
uniform vec3 pos;
uniform vec3 cam;
uniform ivec2 resolution_size;
uniform int time;
void main()
{

    
    //uvec4 color =   texelFetch(input_tex, gl_FragCoord.xyz/vec3(800,600,1));
    uvec4 color = texelFetch(input_tex, ivec3(ivec2(gl_FragCoord.xy)%32, time),0);
    FragColor=vec4(color.x/4294967295.0,color.y/4294967295.0,color.z/4294967295.0,1);
   
} 