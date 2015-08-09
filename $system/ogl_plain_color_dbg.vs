[VertexShader]
#version 330

uniform mat4 mVPMat;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

varying vec3 vecColor;

void main(void)
{
	vecColor = color;
	gl_Position = vec4(position, 1.0) * mVPMat;
}

[FragmentShader]
#version 330

out vec4 color;

varying vec3 vecColor;

void main(void)
{
	color = vec4(vecColor.x, vecColor.y, vecColor.z, 1.f);
}
