attribute vec3 pos;
attribute vec2 texcoord;
attribute vec3 normal;

uniform mat4 M;
uniform mat4 P;
uniform mat4 V;
uniform mat4 NM;

uniform vec3 U_LightPos;
uniform vec4 U_DiffuseLightColor;
uniform vec4 U_DiffuseMaterial;

varying vec4 V_DiffuseColor;

void main()
{
	//L vector
	vec3 L=U_LightPos;
	L=normalize(L);
	//N vector
	vec3 n=normalize(mat3(NM)*normal);

	float diffuseIntensity=max(0.0,dot(L,n));
	V_DiffuseColor=U_DiffuseLightColor*U_DiffuseMaterial*diffuseIntensity;
	gl_Position=P*V*M*vec4(pos,1.0);
}