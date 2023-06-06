
#version 330 core

// Inputs coming from the vertex shader
in struct fragment_data
{
    vec3 position; // position in the world space
    vec3 normal;   // normal in the world space
    vec3 color;    // current color on the fragment
    vec2 uv;       // current uv-texture on the fragment

} fragment;

// Output of the fragment shader - output color
layout(location = 0) out vec4 FragColor;

// Uniform values that must be send from the C++ code
// ***************************************************** //

uniform sampler2D image_texture; // Texture image identifiant

uniform mat4 view; // View matrix (rigid transform) of the camera - to compute the camera position

uniform vec3 light; // position of the light

// Coefficients of phong illumination model
struct phong_structure
{
    float ambient;
    float diffuse;
    float specular;
    float specular_exponent;
};

// Settings for texture display
struct texture_settings_structure
{
    bool use_texture;       // Switch the use of texture on/off
    bool texture_inverse_v; // Reverse the texture in the v component (1-v)
    bool two_sided;         // Display a two-sided illuminated surface (doesn't work on Mac)
};

// Material of the mesh (using a Phong model)
struct material_structure
{
    vec3 color;  // Uniform color of the object
    float alpha; // alpha coefficient

    phong_structure phong;                       // Phong coefficients
    texture_settings_structure texture_settings; // Additional settings for the texture
};

uniform material_structure material;

// AJOUT
//  Ambiant uniform controled from the GUI
uniform float ambiant;
uniform vec3 light_color;
uniform vec3 light0_position;
uniform vec3 light1_position;
uniform float diffuse;
uniform float specular;
uniform float specular_exp;

// AJOUT
const int nlight = 2;
vec3 light_positions[nlight];
vec3 current_colors[nlight];
vec4 FragColors[nlight];

void main()
{
	
	light_positions[0] = light0_position;
	light_positions[1] = light1_position;

	for(int i=0; i<2; i++){
    	vec3 current_color = vec3(0.0);
        vec3 light_position = light_positions[i] ;
        vec3 L = normalize(light_position - fragment.position);
        vec3 N = normalize(fragment.normal);

        float diffuse_value = max(dot(N, L), 0.0);
        float diffuse_magnitude = diffuse * diffuse_value;


        // Compute the position of the center of the camera
        mat3 O = transpose(mat3(view));                   // get the orientation matrix
        vec3 last_col = vec3(view * vec4(0.0, 0.0, 0.0, 1.0)); // get the last column
        vec3 camera_position = -O * last_col;

        // Specular coefficient
        float specular_magnitude = 0.0;
        vec3 u_r = reflect(-L, N); // reflection of light vector relative to the normal.
        vec3 u_v = normalize(camera_position - fragment.position);
        specular_magnitude = specular * pow(max(dot(u_r, u_v), 0.0), specular_exp);

    // Texture
        // *************************************** //

        // Current uv coordinates
        vec2 uv_image = vec2(fragment.uv.x, fragment.uv.y);
        if(material.texture_settings.texture_inverse_v) {
            uv_image.y = 1.0-uv_image.y;
        }

        // Get the current texture color
        vec4 color_image_texture = texture(image_texture, uv_image);
        if(material.texture_settings.use_texture == false) {
            color_image_texture=vec4(1.0,1.0,1.0,1.0);
        }

        vec3 color_object  = fragment.color * material.color * color_image_texture.rgb;

        //////
        current_color = (ambiant + diffuse_magnitude) * color_object  * light_color + specular_magnitude * light_color;

    	FragColors[i] = vec4(current_color , material.alpha * color_image_texture.a );
	}
	//FragColor = 0.9f*FragColors[0]+0.1f*FragColors[1];
    FragColor = FragColors[0];
}

/*


#version 330 core

// Inputs coming from the vertex shader
in struct fragment_data
{
    vec3 position; // position in the world space
    vec3 normal;   // normal in the world space
    vec3 color;    // current color on the fragment
    vec2 uv;       // current uv-texture on the fragment

} fragment;

// Output of the fragment shader - output color
layout(location=0) out vec4 FragColor;


// Uniform values that must be send from the C++ code
// ***************************************************** //

uniform sampler2D image_texture;   // Texture image identifiant

uniform mat4 view;       // View matrix (rigid transform) of the camera - to compute the camera position

uniform vec3 light; // position of the light


// Coefficients of phong illumination model
struct phong_structure {
    float ambient;
    float diffuse;
    float specular;
    float specular_exponent;
};

// Settings for texture display
struct texture_settings_structure {
    bool use_texture;       // Switch the use of texture on/off
    bool texture_inverse_v; // Reverse the texture in the v component (1-v)
    bool two_sided;         // Display a two-sided illuminated surface (doesn't work on Mac)
};

// Material of the mesh (using a Phong model)
struct material_structure
{
    vec3 color;  // Uniform color of the object
    float alpha; // alpha coefficient

    phong_structure phong;                       // Phong coefficients
    texture_settings_structure texture_settings; // Additional settings for the texture
};

uniform material_structure material;

//AJOUT
// Ambiant uniform controled from the GUI
uniform float ambiant;
uniform vec3 light_color;
uniform vec3 light0_position;
uniform vec3 light1_position;
uniform float diffuse;
uniform float specular;
uniform float specular_exp;

//AJOUT
const int nlight = 2;
//vec3 light_positions[nlight];

void main()
{
    vec3 current_color = vec3(0.0);



    for(int i=0; i<nlight; i++){

        vec3 light_position = light0_position ;
        vec3 L = normalize(light_position - fragment.position);
        vec3 N = normalize(fragment.normal);

        float diffuse_value = max(dot(N, L), 0.0);
        float diffuse_magnitude = diffuse * diffuse_value;


        // Compute the position of the center of the camera
        mat3 O = transpose(mat3(view));                   // get the orientation matrix
        vec3 last_col = vec3(view * vec4(0.0, 0.0, 0.0, 1.0)); // get the last column
        vec3 camera_position = -O * last_col;

        // Specular coefficient
        float specular_magnitude = 0.0;
        vec3 u_r = reflect(-L, N); // reflection of light vector relative to the normal.
        vec3 u_v = normalize(camera_position - fragment.position);
        specular_magnitude = specular * pow(max(dot(u_r, u_v), 0.0), specular_exp);

    // Texture
        // *************************************** //

        // Current uv coordinates
        vec2 uv_image = vec2(fragment.uv.x, fragment.uv.y);
        if(material.texture_settings.texture_inverse_v) {
            uv_image.y = 1.0-uv_image.y;
        }

        // Get the current texture color
        vec4 color_image_texture = texture(image_texture, uv_image);
        if(material.texture_settings.use_texture == false) {
            color_image_texture=vec4(1.0,1.0,1.0,1.0);
        }

        vec3 color_object  = fragment.color * material.color * color_image_texture.rgb;

        //////
        current_color = (ambiant + diffuse_magnitude) * color_object  * light_color + specular_magnitude * light_color;

    }

    FragColor = vec4(current_color , material.alpha * color_image_texture.a);

}

*/