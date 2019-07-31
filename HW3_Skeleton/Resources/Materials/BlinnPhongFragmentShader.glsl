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
uniform vec3 specular_reflectance;
uniform vec3 ambient_reflectance;

uniform vec3 fog_color = vec3(0.5, 0.5, 0.5);

const vec3 ambient_illuminance = vec3(0.2, 0.2, 0.2);

void main()
{
	vec3 intensity = vec3(0);
	vec3 n_normal = normalize(fragmentNormal.xyz);
	
	mat4 worldToCamera = inverse(cameraTransform);
	for (int i = 0; i < numLights; i++)
	{
		if (lights[i].type == 1)
		{
			// Point Light
			vec3 light_pos         = (worldToCamera * vec4(lights[i].pos, 1)).xyz;
			vec3 light_direction   = light_pos - fragmentPosition.xyz;
			float light_length     = length(light_direction);
			vec3 n_light_direction = normalize(light_direction);
			vec3 n_half_angle      = normalize( n_light_direction - normalize(fragmentPosition.xyz) );
			
			float diffuse_factor   = clamp( dot(n_normal, n_light_direction), 0, 1);
			float specular_factor  = pow( dot(n_normal, n_half_angle), 50.0);

			intensity += (
				   diffuse_factor  * lights[i].diffuse_illuminance  * diffuse_reflectance 
				 + specular_factor * lights[i].specular_illuminance * specular_reflectance
			) / pow(light_length, 2);
		}
		else if(lights[i].type == 2)
		{
			// Directional Light
			vec3 n_light_direction = normalize((worldToCamera * vec4(lights[i].light_direction, 0)).xyz);
			vec3 n_half_angle      = normalize( n_light_direction - normalize(fragmentPosition.xyz) );
			
			float diffuse_factor   = clamp( dot(n_normal, n_light_direction), 0, 1);
			float specular_factor  = pow( dot(n_normal, n_half_angle), 50.0);

			intensity += (
				   diffuse_factor  * lights[i].diffuse_illuminance  * diffuse_reflectance 
				 + specular_factor * lights[i].specular_illuminance * specular_reflectance
			);
		}
		else if(lights[i].type == 3)
		{
			// Spotlight
			vec3 light_pos         = (worldToCamera * vec4(lights[i].pos, 1)).xyz;
			vec3 n_light_vector    = normalize(light_pos - fragmentPosition.xyz);
			vec3 n_light_direction = normalize((worldToCamera * vec4(lights[i].light_direction, 0)).xyz);
			vec3 n_half_angle      = normalize( n_light_vector - normalize(fragmentPosition.xyz) );

			float diffuse_factor   = clamp( dot(n_normal, n_light_vector), 0, 1);
			float specular_factor  = pow( dot(n_normal, n_half_angle), 50.0);
			
			float attenuation      = pow( dot(n_light_vector, n_light_direction), 50.0 );
			intensity += (
				   diffuse_factor  * lights[i].diffuse_illuminance  * diffuse_reflectance 
				 + specular_factor * lights[i].specular_illuminance * specular_reflectance
			) * attenuation;
		}
	}
	intensity += ambient_illuminance * ambient_reflectance;

	out_color = vec4(intensity, 0.5);

	float fog = clamp( 0.033 * length(fragmentPosition) - 1.0 , 0, 1);
    out_color = mix(out_color, vec4(fog_color, 1), fog);
}
