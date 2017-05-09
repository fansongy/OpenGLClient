
uniform vec4 U_AmbientLightColor;
uniform vec4 U_AmbientMaterial;

void main()
{
	vec4 ambientColor=U_AmbientLightColor*U_AmbientMaterial;
	gl_FragColor=ambientColor;
}