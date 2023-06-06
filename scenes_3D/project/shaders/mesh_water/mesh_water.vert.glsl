#version 330 core

// Vertex shader - this code is executed for every vertex of the shape

// Inputs coming from VBOs
layout (location = 0) in vec3 vertex_position; // vertex position in local space (x,y,z)
layout (location = 1) in vec3 vertex_normal;   // vertex normal in local space   (nx,ny,nz)
layout (location = 2) in vec3 vertex_color;    // vertex color      (r,g,b)
layout (location = 3) in vec2 vertex_uv;       // vertex uv-texture (u,v)

// Output variables sent to the fragment shader
out struct fragment_data
{
    vec3 position; // vertex position in world space
    vec3 normal;   // normal position in world space
    vec3 color;    // vertex color
    vec2 uv;       // vertex uv
} fragment;

// Uniform variables expected to receive from the C++ program
uniform mat4 model; // Model affine transform matrix associated to the current shape
uniform mat4 view;  // View matrix (rigid transform) of the camera
uniform mat4 projection; // Projection (perspective or orthogonal) matrix of the camera

uniform mat4 modelNormal; // Model without scaling used for the normal. modelNormal = transpose(inverse(model))
uniform float time;

vec3 vague(vec3 pos)
{
    float A = 1;
    float B = 1;
    float k = 3;
    float w = 1;
    vec3 p = vec3(pos.x, pos.y, pos.z+A * sin(k*pos.x - w*time) + B * sin(k*pos.y - w*time)); // z(x, y, t) = A * sin(kx - wt) + B * sin(ky - wt)
	return p;
}

vec3 vague2(vec3 position){
    // Paramètres du mouvement d'eau
    float amplitudeX = 10; // Amplitude du mouvement d'eau selon l'axe X
    float amplitudeY = 10; // Amplitude du mouvement d'eau selon l'axe Y
    float amplitudeZ = 3; // Amplitude du mouvement d'eau selon l'axe Z
    float wavelength = 5.0; // Longueur d'onde du mouvement d'eau
    float speed = 0.2; // Vitesse du mouvement d'eau

    // Calcul des coordonnées de chaque point du plan avec le mouvement d'eau
    float waterX = position.x + amplitudeX * sin(2.0 * 3.14159 * (position.x / wavelength + time * speed));
    float waterY = position.y + amplitudeY * sin(2.0 * 3.14159 * (position.y / wavelength + time * speed));
    float waterZ = position.z + amplitudeZ * sin(2.0 * 3.14159 * (position.x / wavelength + position.y / wavelength + time * speed));

    return vec3(waterX, waterY, waterZ);
}

void main()
{
    vec3 n_position = vague2(vertex_position);
    vec4 position = model * vec4(n_position, 1.0);
	// The position of the vertex in the world space
	// vec4 position = model * vec4(vertex_position, 1.0);
	
	// The normal of the vertex in the world space
	vec4 normal = modelNormal * vec4(vertex_normal, 0.0);

	// The projected position of the vertex in the normalized device coordinates:
	vec4 position_projected = projection * view * position;

	// Fill the parameters sent to the fragment shader
	fragment.position = position.xyz;
	fragment.normal   = normal.xyz;
	fragment.color = vertex_color;
	fragment.uv = vertex_uv;

	// gl_Position is a built-in variable which is the expected output of the vertex shader
	gl_Position = position_projected; // gl_Position is the projected vertex position (in normalized device coordinates)
}
