attribute vec3 pos;

uniform mat4 M;
uniform mat4 P;
uniform mat4 V;

void main()
{
	gl_Position=P*V*M*vec4(pos,1.0);
}