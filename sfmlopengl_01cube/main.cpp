#define GL_GLEXT_PROTOTYPES

#include <iostream>
#include <chrono>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

GLuint loadShaders(const char* vertexShaderFile, const char* fragmentShaderFile);

int main()
{
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 24;
    //settings.antialiasingLevel = 8;

    sf::Window window(sf::VideoMode(800, 600, 32), ":]", sf::Style::Titlebar | sf::Style::Close, settings);
    
    sf::Image res_texture;
	if (!res_texture.loadFromFile("texture.jpg"))
		return -1;

    GLuint shaderProgram = loadShaders("vertex.vert", "fragment.frag");
    
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLfloat vertices[] = {
		// front
		-1.0, -1.0,  1.0,
		 1.0, -1.0,  1.0,
		 1.0,  1.0,  1.0,
		-1.0,  1.0,  1.0,
		// top
		-1.0,  1.0,  1.0,
		 1.0,  1.0,  1.0,
		 1.0,  1.0, -1.0,
		-1.0,  1.0, -1.0,
		// back
		 1.0, -1.0, -1.0,
		-1.0, -1.0, -1.0,
		-1.0,  1.0, -1.0,
		 1.0,  1.0, -1.0,
		// bottom
		-1.0, -1.0, -1.0,
		 1.0, -1.0, -1.0,
		 1.0, -1.0,  1.0,
		-1.0, -1.0,  1.0,
		// left
		-1.0, -1.0, -1.0,
		-1.0, -1.0,  1.0,
		-1.0,  1.0,  1.0,
		-1.0,  1.0, -1.0,
		// right
		 1.0, -1.0,  1.0,
		 1.0, -1.0, -1.0,
		 1.0,  1.0, -1.0,
		 1.0,  1.0,  1.0, 
    };
    
    GLushort indices[] = {
		// front
		 0,  1,  2,
		 2,  3,  0,
		// top
		 4,  5,  6,
		 6,  7,  4,
		// back
		 8,  9, 10,
		10, 11,  8,
		// bottom
		12, 13, 14,
		14, 15, 12,
		// left
		16, 17, 18,
		18, 19, 16,
		// right
		20, 21, 22,
		22, 23, 20,
    };
    
    GLfloat cube_texcoords[] = {
		// front
		0.0, 0.0,
		1.0, 0.0,
		1.0, 1.0,
		0.0, 1.0,
		//top
		0.0, 0.0,
		1.0, 0.0,
		1.0, 1.0,
		0.0, 1.0,
		//back
		0.0, 0.0,
		1.0, 0.0,
		1.0, 1.0,
		0.0, 1.0,
		//bottom
		0.0, 0.0,
		1.0, 0.0,
		1.0, 1.0,
		0.0, 1.0,
		//left
		0.0, 0.0,
		1.0, 0.0,
		1.0, 1.0,
		0.0, 1.0,
		//right
		0.0, 0.0,
		1.0, 0.0,
		1.0, 1.0,
		0.0, 1.0,
	};
	
	GLuint vbo_cube_texcoords;
	glGenBuffers(1, &vbo_cube_texcoords);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_texcoords);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_texcoords), cube_texcoords, GL_STATIC_DRAW);
    
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
    
    GLuint ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);
   
    GLint position = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(position);
    glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

    GLint attribute_v_color = glGetAttribLocation(shaderProgram, "v_color");
    glEnableVertexAttribArray(attribute_v_color);
    glVertexAttribPointer(attribute_v_color, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*) (3 * sizeof(GLfloat)));
    
    GLuint texture_id, program_id;
	GLint uniform_mytexture;
	
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGBA,
		res_texture.getSize().x,
		res_texture.getSize().y,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		res_texture.getPixelsPtr());
		
	GLint attribute_texcoord = glGetAttribLocation(shaderProgram, "texcoord");
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_texcoords);
	glEnableVertexAttribArray(attribute_texcoord);
    glVertexAttribPointer(attribute_texcoord, 2, GL_FLOAT, GL_FALSE, 0, 0);
 
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(uniform_mytexture, GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id);

    GLint uniform_m_mvp = glGetUniformLocation(shaderProgram, "mvp");
    
	glm::mat4 projection = glm::perspective(45.0f, 800.0f / 600.0f, 0.1f, 10.0f);
	glm::mat4 view = glm::lookAt(glm::vec3(0.0, 2.0, 0.0), glm::vec3(0.0, 0.0, -4.0), glm::vec3(0.0, 1.0, 0.0));

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_MULTISAMPLE); 

	bool wireframe = true;
	
	float angle = 0;

    auto t_start = std::chrono::high_resolution_clock::now();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
				case sf::Event::Closed:
					window.close();
					break;
				
				case sf::Event::KeyPressed:
				{
					if(event.key.code == sf::Keyboard::Escape)
						window.close();
					if(event.key.code == sf::Keyboard::Right)
					{
						angle -= 15;
					}
					if(event.key.code == sf::Keyboard::Left)
					{
						angle += 15;
					}
					if(event.key.code == sf::Keyboard::W)
					{
						if(wireframe) {
							glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
							wireframe = false;
						}
						else
						{
							glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
							wireframe = true;
						}
					}
				}
            }
        }
   
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        
        auto t_now = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();
		//glUniform3f(outColor, 0.0f, (sin(time * 8.0f) + 1.0f) / 2.0f, 0.0f);
		
		//float move = (sin(time * 1.0f) + 1.0f) / 2.0f;
		float move = 0;
		
		//float angle = time * 45;
		//float angle = 0;
		
		float angl = time * 0.1;
		  glm::mat4 anim = 
			glm::rotate(glm::mat4(1.0f), angl*2.0f, glm::vec3(1, 0, 0)) *
			glm::rotate(glm::mat4(1.0f), angl*2.0f, glm::vec3(0, 1, 0)) *
			glm::rotate(glm::mat4(1.0f), angl*2.0f, glm::vec3(0, 0, 1));
		
		glm::vec3 axis_z(0.0, 1.0, 0.0);
		glm::mat4 model =
			glm::translate(glm::mat4(1.0f), glm::vec3(move, 0.0, -4.0)) *
			glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis_z) *
			glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		
		glm::mat4 mvp = projection * view * model /* * anim */;
		
		glUniformMatrix4fv(uniform_m_mvp, 1, GL_FALSE, glm::value_ptr(mvp));
		
		glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_SHORT, 0);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

        window.display();
    }

	glDeleteTextures(1, &texture_id);
	
    glDeleteProgram(shaderProgram);

    glDeleteBuffers(1, &vbo);

    glDeleteVertexArrays(1, &vao);
}
