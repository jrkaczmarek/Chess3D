#ifndef MY_MODEL_H
#define MY_MODEL_H

#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include "constants.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include "constants.h"
#include "allmodels.h"
#include "lodepng.h"
#include "shaderprogram.h"
#include "myCube.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include <string>

using namespace std;

class my_model
{
public:
	vector<glm::vec4> verts;
	vector<glm::vec4> norms;
	vector<glm::vec2> texCoords;
	vector<unsigned int> indices;

	my_model(string plik, int nr_mesh)
	{
		//wgranie modelu
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(plik, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);
		cout << importer.GetErrorString() << endl;

		aiMesh* mesh = scene->mMeshes[nr_mesh];

		for (int i = 0; i < scene->mNumMeshes; i++)
			cout << "nazwa modelu nr " << i << " " << scene->mMeshes[i]->mName.C_Str() << endl;

		for (int i = 0; i < mesh->mNumVertices; i++)
		{
			aiVector3D vertex = mesh->mVertices[i];
			verts.push_back(glm::vec4(vertex.x, vertex.y, vertex.z, 1));

			aiVector3D normal = mesh->mNormals[i];
			norms.push_back(glm::vec4(normal.x, normal.y, normal.z, 0));

			aiVector3D texCoord = mesh->mTextureCoords[0][i];
			texCoords.push_back(glm::vec2(texCoord.x, texCoord.y));
		}

		for (int i = 0; i < mesh->mNumFaces; i++) {
			aiFace& face = mesh->mFaces[i];
			for (int j = 0; j < face.mNumIndices; j++) {
				indices.push_back(face.mIndices[j]);
			}
		}

		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		for (int i = 0; i < 19; i++) {
			cout << i << " " << material->GetTextureCount((aiTextureType)i) << endl;
		}

		for (int i = 0; i < material->GetTextureCount(aiTextureType_DIFFUSE); i++) {
			aiString str;
			material->GetTexture(aiTextureType_DIFFUSE, i, &str);
			cout << str.C_Str() << endl;
		}
	}

	void draw(glm::mat4 M, GLuint& tex)
	{
		glUniformMatrix4fv(spLambertTextured->u("M"), 1, false, glm::value_ptr(M));
		glEnableVertexAttribArray(spLambertTextured->a("vertex"));
		glVertexAttribPointer(spLambertTextured->a("vertex"), 4, GL_FLOAT, false, 0, verts.data());
		glEnableVertexAttribArray(spLambertTextured->a("normal"));
		glVertexAttribPointer(spLambertTextured->a("normal"), 4, GL_FLOAT, false, 0, norms.data());
		glEnableVertexAttribArray(spLambertTextured->a("texCoord"));
		glVertexAttribPointer(spLambertTextured->a("texCoord"), 2, GL_FLOAT, false, 0, texCoords.data());
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex);
		glUniform1i(spLambertTextured->u("tex"), 0);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, indices.data());
		glDisableVertexAttribArray(spLambertTextured->a("vertex"));
		glDisableVertexAttribArray(spLambertTextured->a("normal"));
		glDisableVertexAttribArray(spLambertTextured->a("texCoord"));
	}
};


#endif
