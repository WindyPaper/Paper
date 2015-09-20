#version 330  

in vec3 Position;  
in vec2 TexCood;  
in vec3 Normal; 
uniform mat4 g_View; 
uniform mat4 g_Proj; 
uniform mat4 g_Model; 

void main ()  
{  
	gl_Position = vec4(Position, 1.0) * (g_Proj * g_View * g_Model);  
}