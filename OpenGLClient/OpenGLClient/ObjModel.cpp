#include "ObjModel.h"
#include "utils.h"
#include <stdio.h>
#include <sstream>
#include <string>
#include <vector>

void ObjModel::Init(const char*modelFilePath)
{
	struct VertexInfo
	{
		float v[3];
		VertexInfo()
		{
			memset(v, 0, sizeof(float) * 3);
		}
	};

	struct VertexDefine
	{
		int positionIndex;
		int texcoordIndex;
		int normalIndex;
	};
	std::vector<VertexInfo> positions;
	std::vector<VertexInfo> texcoords;
	std::vector<VertexInfo> normals;

	std::vector<VertexDefine> vertices;
	std::vector<unsigned int> faces;

	//load model from file
	char*fileContent = LoadFileContent(modelFilePath);
	//decode model
	std::stringstream ssFileContent(fileContent);
	char szOneLine[256];
	std::string temp;
	while (!ssFileContent.eof())
	{
		memset(szOneLine, 0, 256);
		ssFileContent.getline(szOneLine, 256);
		if (strlen(szOneLine)>0)
		{
			std::stringstream ssOneLine(szOneLine);
			if (szOneLine[0]=='v')
			{
				if (szOneLine[1]=='t')
				{
					VertexInfo vi;
					ssOneLine >> temp;//vt
					ssOneLine >> vi.v[0];
					ssOneLine >> vi.v[1];
					texcoords.push_back(vi);
				}
				else if (szOneLine[1]=='n')
				{
					VertexInfo vi;
					ssOneLine >> temp;//vn
					ssOneLine >> vi.v[0];
					ssOneLine >> vi.v[1];
					ssOneLine >> vi.v[2];
					normals.push_back(vi);
				}
				else
				{
					VertexInfo vi;
					ssOneLine >> temp;//v
					ssOneLine >> vi.v[0];
					ssOneLine >> vi.v[1];
					ssOneLine >> vi.v[2];
					positions.push_back(vi);
				}
			}
			else if(szOneLine[0]=='f')
			{
				//
				ssOneLine >> temp;//f
				std::string vertexStr;
				for (int i = 0; i < 3; ++i)
				{
					ssOneLine >> vertexStr;
					size_t pos = vertexStr.find_first_of('/');
					std::string positionIndexStr = vertexStr.substr(0, pos);
					size_t pos2 = vertexStr.find_first_of('/', pos + 1);
					std::string texcoordIndexStr = vertexStr.substr(pos + 1, pos2 - pos - 1);
					std::string normalIndexStr = vertexStr.substr(pos2 + 1, vertexStr.length() - pos2 - 1);
					VertexDefine vd;
					vd.positionIndex = atoi(positionIndexStr.c_str())-1;
					vd.texcoordIndex = atoi(texcoordIndexStr.c_str())-1;
					vd.normalIndex = atoi(normalIndexStr.c_str())-1;
					//trim the same vertice
					int nCurrentVertexIndex = -1;
					size_t nCurrentVerticeCount = vertices.size();
					for(int j=0;j<nCurrentVerticeCount;++j)
					{
						if (vertices[j].positionIndex==vd.positionIndex&&
							vertices[j].texcoordIndex == vd.texcoordIndex&&
							vertices[j].normalIndex == vd.normalIndex)
						{
							nCurrentVertexIndex = j;
							break;
						}
					}
					if (nCurrentVertexIndex==-1)
					{
						nCurrentVertexIndex = (int)vertices.size();
						vertices.push_back(vd);
					}
					faces.push_back(nCurrentVertexIndex);
				}
			}
		}
	}
	//convert to opengl vbo & ibo
	int vertexCount = (int)vertices.size();
	VertexData*vertexes = new VertexData[vertexCount];
	for (int i=0;i<vertexCount;++i)
	{
		memcpy(vertexes[i].position, positions[vertices[i].positionIndex].v, sizeof(float) * 3);
		memcpy(vertexes[i].texcoord, texcoords[vertices[i].texcoordIndex].v, sizeof(float) * 2);
		memcpy(vertexes[i].normal, normals[vertices[i].normalIndex].v, sizeof(float) * 3);
	}
	//create vbo
	glGenBuffers(1, &mVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER,sizeof(VertexData)*vertexCount,vertexes,GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	delete vertexes;

	//ibo
	mIndexCount = (int)faces.size();
	unsigned int *indexes = new unsigned int[mIndexCount];
	for (int i=0;i<mIndexCount;++i)
	{
		indexes[i] = faces[i];
	}
	glGenBuffers(1, &mIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*mIndexCount, indexes, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	delete indexes;

	delete fileContent;
}

void ObjModel::Bind(GLint posLoc, GLint texcoordLoc, GLint normalLoc)
{
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glEnableVertexAttribArray(posLoc);
	glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), 0);

	glEnableVertexAttribArray(texcoordLoc);
	glVertexAttribPointer(texcoordLoc, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(sizeof(float) * 3));

	glEnableVertexAttribArray(normalLoc);
	glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(sizeof(float) * 5));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ObjModel::Draw()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
	glDrawElements(GL_TRIANGLES, mIndexCount, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}