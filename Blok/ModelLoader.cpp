#include "ModelLoader.h"


void LoadModelElem(const char *file, GLuint* vertexBuffer, GLuint* UVBuffer, GLuint* normalBuffer, GLuint* elementBuffer, GLuint* numTriangles)
{
	const aiScene *scene = aiImportFile(file, aiProcessPreset_TargetRealtime_MaxQuality);

	unsigned int *faceArray;
	faceArray = (unsigned int*)malloc(sizeof(unsigned int)* scene->mMeshes[0]->mNumFaces * 3);
	unsigned int faceIndex = 0;

	for (unsigned int t = 0; t < scene->mMeshes[0]->mNumFaces; ++t)
	{
		const aiFace* face = &scene->mMeshes[0]->mFaces[t];

		memcpy(&faceArray[faceIndex], face->mIndices, 3 * sizeof(unsigned int));
		faceIndex += 3;
	}

	glGenBuffers(1, vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, *vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, scene->mMeshes[0]->mNumVertices * 3 * sizeof(GLfloat), scene->mMeshes[0]->mVertices, GL_STATIC_DRAW);

	glGenBuffers(1, UVBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, *UVBuffer);
	glBufferData(GL_ARRAY_BUFFER, scene->mMeshes[0]->mNumVertices * 3 * sizeof(GLfloat), scene->mMeshes[0]->mTextureCoords[0], GL_STATIC_DRAW);

	glGenBuffers(1, normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, *normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, scene->mMeshes[0]->mNumVertices * 3 * sizeof(GLfloat), scene->mMeshes[0]->mNormals, GL_STATIC_DRAW);

	glGenBuffers(1, elementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *elementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, scene->mMeshes[0]->mNumFaces * 3 * sizeof(GLfloat), faceArray, GL_STATIC_DRAW);

	*numTriangles = scene->mMeshes[0]->mNumFaces;
}
