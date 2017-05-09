
uniform vec4 U_AmbientLightColor;
uniform vec4 U_AmbientMaterial;

varying vec4 V_DiffuseColor;

void main()
{
	vec4 ambientColor=U_AmbientLightColor*U_AmbientMaterial;
	gl_FragColor=ambientColor+V_DiffuseColor;
}