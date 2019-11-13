#pragma once
#include "mesh.h"

static float qm_vertices[] = {
	// positions          // texture coords
	1.0f,  1.0f,   1.0f, 1.0f, // top right
	1.0f, -1.0f,   1.0f, 0.0f, // bottom right
   -1.0f, -1.0f,   0.0f, 0.0f, // bottom left
   -1.0f,  1.0f,   0.0f, 1.0f  // top left 
};

static unsigned int qm_indices[] = {
	   0, 1, 3, // first triangle
	   1, 2, 3  // second triangle
};

class QuadMesh : public Mesh {
public:
	void init() {
		glGenVertexArrays(1, &m_VAO);
		glGenBuffers(1, &m_VBO);
		glGenBuffers(1, &m_EBO);

		glBindVertexArray(m_VAO);

		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(qm_vertices), qm_vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(qm_indices), qm_indices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);	//pos

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));	//texcoord
	}
};