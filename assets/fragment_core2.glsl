#version 330 core
out vec4 FragColor;

in vec3 ourColor;

void main()
{
	FragColor = vec4(0.5f, 0.82f,0.11f, 1.0f);
	// FragColor = vec4(ourColor, 1.0f);
}
