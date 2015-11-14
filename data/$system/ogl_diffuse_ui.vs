[VertexShader]
#version 330

// матрица преобразования координат, получаемая из программы
uniform mat4 mVPMat;
uniform mat4 mVMat;
uniform mat4 mUVMat;
uniform vec2 UVOffset;
uniform float fogDensity;
uniform bool  bFog;

// входящие вершинные атрибуты
layout(location = 0) in vec3 position; 
layout(location = 1) in vec3 normal; 
layout(location = 2) in vec3 color; 
layout(location = 3) in vec2 texcoord0; 
layout(location = 4) in vec2 texcoord1; 
layout(location = 5) in vec3 S;
layout(location = 6) in vec3 T;
layout(location = 7) in vec3 SxT;
layout(location = 8) in vec4 Weights;
layout(location = 9) in vec4 Indexes;

// исходящие параметры, которые будут переданы в фрагментный шейдер
out vec2 fragTexcoord;

varying float atten;
varying float fogFactor;

void main(void)
{
	// перевод позиции вершины из локальных координат в однородные
	gl_Position = vec4(position, 1.0) * mVPMat;

	// передадим текстурные координаты в фрагментный шейдер
	vec4 fTexcoord = vec4(texcoord0.x, texcoord0.y, 0, 0) * mUVMat;
	fragTexcoord = vec2(fTexcoord.x + UVOffset.x, fTexcoord.y + UVOffset.y);

	
	if (bFog)
	{
		const float LOG2 = 1.442695f;
		vec3 vVertex = vec3(vec4(position, 1.0) * mVMat);
		float vlength = length(vVertex);
	
		fogFactor = exp2(-fogDensity * fogDensity * vlength * vlength * LOG2);
		fogFactor = clamp(fogFactor, 0.0, 1.0);
	}
}

[FragmentShader]
#version 330

// текстура
uniform sampler2D colorTexture;
uniform vec4	  fogColor;
uniform bool  	  bFog;

// входящие параметры, которые получены из вершинного шейдера
in vec2 fragTexcoord;

// результирующий цвет пикселя на экране
out vec4 color;

varying float fogFactor;

void main(void)
{
	// получим цвет пикселя из текстуры по текстурным координатам
	color = texture(colorTexture, fragTexcoord);
	
	if (bFog)
	{
		color = mix(fogColor, color, fogFactor);
	}
}
