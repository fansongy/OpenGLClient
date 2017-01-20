uniform vec4 U_AmbientLightColor;
uniform vec4 U_AmbientMaterial;

uniform sampler2D U_MainTexture;

varying vec3 V_Normal;
varying vec3 V_WorldPos;
varying vec2 V_Texcoord;

void main()
{
    vec3 lightPos = vec3(10.0,10.0,0.0);
    vec3 L = lightPos;

    L = normalize(L);
    vec3 n = normalize(V_Normal);

    //ambient
    U_AmbientLightColor = vec4(0.2,0.2,0.2,1.0);
    U_AmbientMaterial = vec4(0.2,0.2,0.2,1.0);
    
    vec4 ambientColor = U_AmbientLightColor * U_AmbientMaterial;
    
    //diffuse
    vec4 DiffuseLightColor = vec4(1.0,1.0,1.0,1.0);
    vec4 DiffuseMaterial = vec4(0.8,0.8,0.8,1.0);
    vec4 diffuseColor = DiffuseLightColor * DiffuseMaterial * max(0.0,dot(L,n));

    //specular
    vec3 reflectDir = normalize(reflect(-L,n));

    vec3 viewDir = normalize(vec3(0.0)-V_WorldPos.xyz);
    vec4 SpecularLightColor = vec4(1.0,1.0,1.0,1.0);
    vec4 SpecularMaterial = vec4(1.0,1.0,1.0,1.0);
    vec4 specularColor = SpecularLightColor*SpecularMaterial*pow(max(0.0,dot(viewDir,reflectDir)),128);

    if(diffuseColor.r == 0.0)
    {
        gl_FragColor= texture2D(U_MainTexture,V_Texcoord)*(ambientColor+diffuseColor);
    }
    else
    {
        gl_FragColor= ambientColor+texture2D(U_MainTexture,V_Texcoord)*diffuseColor+specularColor;
    }


}