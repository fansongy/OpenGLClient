#pragma  once


struct VertexData
{
	float position[3];
	float textoord[2];
	float noraml[3];
};

VertexData* LoadObjModel(const char* path,unsigned int **indexes,int &vertexCount,int &indexCount);