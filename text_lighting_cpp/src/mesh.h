#pragma once
#include <glad/glad.h>
#include "shader.h"
#include <memory>


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

	void draw(std::shared_ptr<Shader> shader) {
		shader->use();
		glBindVertexArray(m_VAO);
		glActiveTexture(GL_TEXTURE0 + m_texture_id);
		glBindTexture(GL_TEXTURE_2D, m_texture_id);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	void setTexture(std::shared_ptr<Shader> shader, unsigned int texture_id) {
		shader->use();
		glActiveTexture(GL_TEXTURE0 + texture_id);
		glBindTexture(GL_TEXTURE_2D, texture_id);
		m_texture_id = texture_id;
	}

protected:
	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_EBO;

	unsigned int m_texture_id;
};