[VertexShader]
#version 330

// матрица преобразования координат, получаемая из программы
uniform mat4 mVPMat;
uniform mat4 mVMat;

// входящие вершинные атрибуты
layout(location = 0) in vec3 position; 
layout(location = 1) in vec2 texcoord0;

// исходящие параметры, которые будут переданы в фрагментный шейдер
out vec2 fragTexcoord;

varying float atten;

void main(void)
{
	// перевод позиции вершины из локальных координат в однородные
	gl_Position = vec4(position, 1.0) * mVPMat;

	// передадим текстурные координаты в фрагментный шейдер
	fragTexcoord = texcoord0;
}

[FragmentShader]
#version 330

uniform sampler2D colorTexture;

in vec2 fragTexcoord;
uniform vec4 InColor;

out vec4 color;

void main(void)
{
	// получим цвет пикселя из текстуры по текстурным координатам
	color = texture(colorTexture, fragTexcoord) * InColor;
}
