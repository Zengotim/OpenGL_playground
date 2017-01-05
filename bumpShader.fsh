#version 150

in vec3 fN;		
in vec3 fE;
in vec3 fL;

out vec4 fColor;

in vec2 texCoord;				// NEW!! This is the UV
vec2 newTexCoord;
uniform sampler2D texture;		// NEW!! This is the texture

uniform vec4 ambient, diffuse, specular;	
uniform float shininess;

void main () {
	vec3 bumpN = texture2D(texture, texCoord*5).xyz;
	vec3 N = normalize (fN+bumpN);	// the Normal
	vec3 L = normalize (fL);	// the Light direction
	vec3 E = normalize (fE.xyz);	// the Eye/Camera position
	vec3 H = normalize (L + E); // the Half vector

	// Calculate the diffuse part - but don't go negative!
	float Kd = max (dot(N, L), 0.0);	// Intensity
	vec4 dif = Kd*diffuse;				// ... and then color

	// Calculate the specular part. 
	float Ks = pow (max(dot(N, H), 0.0), shininess);	// Intensity
	vec4 spec = Ks*specular;							// ... and then color
	if (dot(N, L) < 0.0) {								// But only if the light is in front!
		spec = vec4(0.0, 0.0, 0.0, 1.0);
	}

	// The final color is a combination of all of these
	newTexCoord = texCoord*10;
	//fColor = ambient + dif + spec;
	fColor.a = 1.0;
}