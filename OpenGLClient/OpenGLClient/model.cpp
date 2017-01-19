#include "model.h"
#include "misc.h"
#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>


struct VertexInfo
{
	float v[3];
};

struct VertexDefine
{
	int positionIndex;
	int texcoordIndex;
	int normalIndex;
};

VertexData* LoadObjModel(const char* path,unsigned int **indexes,int &vertexCount,int &indexCount)
{
	char* fileContent = LoadFileContent(path);
	if(fileContent !=nullptr){

		std::vector<VertexInfo> position;
		std::vector<VertexInfo> texcoord;
		std::vector<VertexInfo> normals;

		std::vector<unsigned int> objIndexes;
		std::vector<VertexDefine> vertices;

		//obj decode
		std::stringstream ssObjFile(fileContent);
		char szOneLine[256];
		std::string temp;
		while(!ssObjFile.eof()){
			memset(szOneLine,0,256);
			ssObjFile.getline(szOneLine,256);
			if(strlen(szOneLine) >0)
			{
				std::stringstream ssOneLine(szOneLine);

				if (szOneLine[0] == 'v')
				{
					if (szOneLine[1] == 't')
					{
						//coord
						//printf("texcoord %s\n",szOneLine);
						ssOneLine>>temp;
						VertexInfo vi ;
						ssOneLine>>vi.v[0];
						ssOneLine>>vi.v[1];
						texcoord.push_back(vi);
						//printf("%s %f %f \n",temp.c_str(),vi.v[0],vi.v[1]);
					} 
					else if(szOneLine[1] == 'n')
					{
						//normal
						//printf("normal %s\n",szOneLine);
						ssOneLine>>temp;
						VertexInfo vi ;
						ssOneLine>>vi.v[0];
						ssOneLine>>vi.v[1];
						ssOneLine>>vi.v[2];
						normals.push_back(vi);
						//printf("%s %f %f %f\n",temp.c_str(),vi.v[0],vi.v[1],vi.v[2]);
					}
					else
					{
						//position
						//printf("position %s\n",szOneLine);
						ssOneLine>>temp;
						VertexInfo vi ;
						ssOneLine>>vi.v[0];
						ssOneLine>>vi.v[1];
						ssOneLine>>vi.v[2];
						position.push_back(vi);
						//printf("%s %f %f %f\n",temp.c_str(),vi.v[0],vi.v[1],vi.v[2]);
					}
				}
				else if(szOneLine[0] == 'f'){
					//printf("face %s\n",szOneLine);
					ssOneLine>>temp;
					std::string vertexStr;
					for (int i = 0;i<3;++i)
					{
						ssOneLine>>vertexStr;
						size_t pos = vertexStr.find_first_of('/');
						std::string positionIndexStr = vertexStr.substr(0,pos);
						size_t pos2 = vertexStr.find_first_of('/',pos+1);
						std::string texcoordIndexStr= vertexStr.substr(pos+1,pos2-pos -1);
						std::string normalIndexStr = vertexStr.substr(pos2+1,vertexStr.length()-pos2 -1);
						VertexDefine vd;
						vd.positionIndex  = atoi(positionIndexStr.c_str())-1;
						vd.texcoordIndex = atoi(texcoordIndexStr.c_str())-1;
						vd.normalIndex = atoi(normalIndexStr.c_str())-1;

						int nCurrentIndex = -1;
						size_t nCurrentVertexCount = vertices.size();
						for(size_t j=0 ;j<nCurrentVertexCount;++j){
							if(vertices[j].positionIndex == vd.positionIndex &&
								vertices[j].texcoordIndex == vd.texcoordIndex &&
								vertices[j].normalIndex == vd.normalIndex)
							{
								nCurrentIndex = j;
								break;
							}
						}

						if(nCurrentIndex == -1){
							nCurrentIndex = vertices.size();
							vertices.push_back(vd);
						}
						objIndexes.push_back(nCurrentIndex);
					}
					
				}

			}
		}
		printf("face count %u\n",objIndexes.size()/3);
		//objIndex ->buffer->ibo
		//vertexIndex->vbo
		indexCount =(int) objIndexes.size();
		*indexes = new unsigned int[indexCount];
		for(int i = 0;i<indexCount;++i){
			(*indexes)[i] = objIndexes[i];
		}

		vertexCount = (int)vertices.size();
		VertexData* vertexs = new VertexData[vertexCount];
		for (int i = 0;i<vertexCount;++i)
		{
			memcpy(vertexs[i].position,position[vertices[i].positionIndex].v,sizeof(float)*3);
			memcpy(vertexs[i].textoord,texcoord[vertices[i].texcoordIndex].v,sizeof(float)*2);
			memcpy(vertexs[i].noraml,normals[vertices[i].normalIndex].v,sizeof(float)*3);
		}
		return vertexs;
	}
	return nullptr;
}