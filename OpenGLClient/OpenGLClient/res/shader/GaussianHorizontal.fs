
varying vec2 V_Texcoord;

uniform sampler2D U_MainTexture;

void main()
{
	float texelOffset=1/300.0;
	float weight[5]=float[](0.22,0.19,0.12,0.08,0.01);
	vec4 color=texture2D(U_MainTexture,V_Texcoord)*weight[0];

	for(int i=1;i<5;i++)
	{
		color+=texture2D(U_MainTexture,vec2(V_Texcoord.x+texelOffset*i,V_Texcoord.y))*weight[i];
		color+=texture2D(U_MainTexture,vec2(V_Texcoord.x-texelOffset*i,V_Texcoord.y))*weight[i];
	}

	gl_FragColor=color;
}