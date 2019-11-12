#pragma once
#include <glad/glad.h>
#include "shader.h"


class Mesh {

public:
	virtual ~Mesh() {
		glDeleteVertexArrays(1, &m_VAO);
		glDeleteBuffers(1, &m_VBO);
		glDeleteBuffers(1, &m_EBO);
	}

	virtual void init() {
		std::cout << "Mesh init" << std::endl;
	}

	void draw(Shader* shader) {
		glBindVertexArray(m_VAO);
		shader->use();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	void setTexture(Shader* shader, unsigned int texture_id) {
		shader->use();
		shader->setInt("material.diffuse", texture_id);
		shader->setInt("material.specular", texture_id);
		shader->setFloat("material.shininess", 32.0f);
	}

protected:
	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_EBO;
};