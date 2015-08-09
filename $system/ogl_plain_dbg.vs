[VertexShader]
#version 330

uniform mat4 mVPMat;

layout(location = 0) in vec3 position;

void main(void)
{
	gl_Position = vec4(position, 1.0) * mVPMat;
}

[FragmentShader]
#version 330

uniform vec4 InColor;

out vec4 color;

void main(void)
{
	color = InColor;
}

