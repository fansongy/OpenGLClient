attribute vec3 pos;
attribute vec2 texcoord;

varying vec2 V_Texcoord;
void main()
{
	vec4 worldPos=vec4(pos,1.0);
	V_Texcoord=texcoord;
	worldPos.x*=2.0;
	worldPos.y*=2.0;
	gl_Position=worldPos;
}