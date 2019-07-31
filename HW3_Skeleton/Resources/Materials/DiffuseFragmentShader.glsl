#version 330 core

in vec4 fragmentPosition;
in vec4 fragmentNormal;

out vec4 out_color;

struct Light
{
	int type;
	vec3 diffuse_illuminance;
	vec3 specular_illuminance;
	vec3 pos;
	vec3 light_direction;
};

uniform mat4 cameraTransform;
uniform int numLights;
uniform Light lights[3];

uniform vec3 diffuse_reflectance;

void main()
{
	vec3 intensity = vec3(0);
	vec3 n_normal = normalize(fragmentNormal.xyz);

	mat4 worldToCamera = inverse(cameraTransform);
	for (int i=0; i<numLights; i++)
	{
		vec3 n_light_direction = normalize((worldToCamera * vec4(lights[i].light_direction, 0)).xyz);

		if (lights[i].type == 1)
		{
			// Point Light
			vec3 light_pos         = (worldToCamera * vec4(lights[i].pos, 1)).xyz;
			vec3 light_direction   = light_pos - fragmentPosition.xyz;
			float light_length     = length(light_direction);
			
			float diffuse_factor   = clamp( dot(n_normal, n_light_direction), 0, 1);
			
			intensity += (
				   diffuse_factor  * lights[i].diffuse_illuminance  * diffuse_reflectance
			) / pow(light_length, 2);
		}

		else if(lights[i].type == 2)
		{
			// Directional Light
			float diffuse_factor = dot(n_normal, n_light_direction);
			intensity += diffuse_factor * (lights[i].diffuse_illuminance * diffuse_reflectance);
		}

		else if(lights[i].type == 3)
		{
			// Spotlight
			vec3 light_pos         = (worldToCamera * vec4(lights[i].pos, 1)).xyz;
			vec3 n_light_vector    = normalize(light_pos - fragmentPosition.xyz);
			vec3 n_light_direction = normalize((worldToCamera * vec4(lights[i].light_direction, 0)).xyz);
			
			float diffuse_factor   = clamp( dot(n_normal, n_light_vector), 0, 1);
			
			float attenuation      = pow( dot(n_light_vector, n_light_direction), 50.0 );
			intensity += (
				   diffuse_factor  * lights[i].diffuse_illuminance  * diffuse_reflectance 
			) * attenuation;
		}
	}

	out_color = vec4(intensity, 1);
	out_color.rgb = pow(out_color.rgb, vec3(1.0/2.2));

	
}
