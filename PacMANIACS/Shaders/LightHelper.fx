
struct DirectionalLight
{
	float4 	Color;
	
	float	AmbientIntensity;
	float	DiffuseIntensity;
	float	SpecularIntensity;
	
	float3 	Direction;	
};

struct PointLight
{
	float4 	Color;
	
	float	AmbientIntensity;
	float	DiffuseIntensity;
	float	SpecularIntensity;
	
	float3 	Position;
	float 	Length;
};

struct SpotLight
{
	float4 	Color;
	
	float	AmbientIntensity;
	float	DiffuseIntensity;
	float	SpecularIntensity;
	
	float3 	Position;
	float 	Length;
	float 	Angle;
};

//-----------------------------------------------------------------------------------------
// ComputeDirectionalLight
//-----------------------------------------------------------------------------------------
void ComputeDirectionalLight(float3 posW, float3 normalW, float3 cameraPos, DirectionalLight light, float Shininess, float SpecularIntensity,
								out float4 AmbientDiffuse, out float4 Specular)
{	

	float3 pos 		= posW;
	float3 normal	= normalW;

	float3 toEyeW 	= normalize(cameraPos - pos);
	float3 toLight 	= normalize(-light.Direction);
	
	float4 ambient 	= float4(0,0,0,0);
	float4 diffuse 	= float4(0,0,0,0);
	Specular 		= float4(0,0,0,0);
	
	ambient = saturate(light.AmbientIntensity * light.Color);
	
	float diffuseFactor = dot(toLight, normal);	
	if(diffuseFactor > 0.0f)
	{
		float Ns			= Shininess;
		float Ks 			= SpecularIntensity;
		float3 reflectionW	= reflect(-toLight, normal);
		float specFactor	= pow(max(dot(reflectionW, toEyeW), 0.0f), Ns);
				
		diffuse				=	saturate(light.DiffuseIntensity * light.Color * diffuseFactor);			
		Specular			=	saturate(light.SpecularIntensity * light.Color * Ks * specFactor);
	}
	
	AmbientDiffuse = saturate(ambient + diffuse);
}