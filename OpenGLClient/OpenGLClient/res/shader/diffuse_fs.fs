

uniform vec3 U_LightPos;
uniform vec4 U_AmbientLightColor;
uniform vec4 U_AmbientMaterial;
uniform vec4 U_DiffuseLightColor;
uniform vec4 U_DiffuseMaterial;

varying vec3 V_Normal;

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


	gl_FragColor=ambientColor+diffuseColor;
}