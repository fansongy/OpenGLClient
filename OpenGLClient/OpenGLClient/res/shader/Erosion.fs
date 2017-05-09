
varying vec2 V_Texcoord;

uniform sampler2D U_MainTexture;

void main()
{
	vec4 minValue=vec4(1.0);
	int coreSize=6;
	int halfCoreSize=coreSize/2;
	float texelOffset=1/100.0;
	for(int y=0;y<coreSize;y++)
	{
		for(int x=0;x<coreSize;x++)
		{
			vec4 currentAlpha=texture2D(U_MainTexture,V_Texcoord+vec2((-halfCoreSize+x)*texelOffset,(-halfCoreSize+y)*texelOffset));
			minValue=min(minValue,currentAlpha);
		}
	}
	gl_FragColor=minValue;
}