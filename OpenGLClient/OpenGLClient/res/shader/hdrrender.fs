
varying vec2 V_Texcoord;

uniform sampler2D U_MainTexture;

void main()
{
	vec4 color=texture2D(U_MainTexture,V_Texcoord);
	if(color.r>2.0)
	{
		discard;
	}
	gl_FragColor=color;
}