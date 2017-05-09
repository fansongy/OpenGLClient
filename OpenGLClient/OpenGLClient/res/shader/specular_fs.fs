

uniform vec3 U_LightPos;
uniform vec3 U_EyePos;
uniform vec4 U_AmbientLightColor;
uniform vec4 U_AmbientMaterial;
uniform vec4 U_DiffuseLightColor;
uniform vec4 U_DiffuseMaterial;
uniform vec4 U_SpecularLightColor;
uniform vec4 U_SpecularMaterial;

varying vec3 V_Normal;
varying vec3 V_WorldPos;

void main()
{
	//ambient
	vec4 ambientColor=U_AmbientLightColor*U_AmbientMaterial;

	//diffuse
	//L vector
	vec3 L=U_LightPos;
	L=normalize(L);
	//N vector
	vec3 n=normalize(V_Normal);

	float diffuseIntensity=max(0.0,dot(L,n));
	vec4 diffuseColor=U_DiffuseLightColor*U_DiffuseMaterial*diffuseIntensity;

	
	//specular
	//reflection
	vec3 reflectDir=reflect(-L,n);
	reflectDir=normalize(reflectDir);
	//inverse view direction : object->eye

	vec3 viewDir=U_EyePos-V_WorldPos;
	viewDir=normalize(viewDir);

	vec4 specularColor=U_SpecularLightColor*U_SpecularMaterial*pow(max(0.0,dot(viewDir,reflectDir)),32.0);

	gl_FragColor=ambientColor+diffuseColor+specularColor;
}