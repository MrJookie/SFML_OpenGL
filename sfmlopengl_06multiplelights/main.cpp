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
    settings.stencilBits = 8;

    sf::RenderWindow window(sf::VideoMode(800, 600, 32), ":]", sf::Style::Titlebar | sf::Style::Close, settings);
    window.setKeyRepeatEnabled(true);
    
    sf::Clock clock;
    sf::Time updateTime;
    std::size_t updateNumFrames;
    
    GLuint shaderProgram = loadShaders("vertex.vert", "fragment.frag");
    GLuint shaderProgramLamp = loadShaders("vertexLamp.vert", "fragmentLamp.frag");
    
    GLuint indices[] = {
        0, 1, 3,
        1, 2, 3,
    };
    
    GLfloat vertices[] = {
		// Positions           // Normals           // Texture Coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };
    
    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };
    
    glm::vec3 pointLightPositions[] = {
        glm::vec3( 2.7f,  0.2f,  2.0f),
        glm::vec3( 2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -7.0f),
        glm::vec3( 1.5f,  0.0f, -6.0f)
    };
	
	GLuint vao;
	glGenVertexArrays(1, &vao);
    
    GLuint vbo;
    glGenBuffers(1, &vbo);
    
    GLuint ebo;
    glGenBuffers(1, &ebo);
    
	glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		//position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		
		//normal
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		
		//texcoord
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);
	glBindVertexArray(0);
	
	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	
	glBindVertexArray(lightVAO);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		
		//position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
	glBindVertexArray(0);
    
	glEnable(GL_DEPTH_TEST);

	bool wireframe = true;
	
	glm::vec3 lightPos(1.2f, 0.0f, 0.0f);
	glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
	
	glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
	GLfloat cameraSpeed = 0.1f;
	
	GLfloat yaw    = -90.0f;
	GLfloat pitch  =  0.0f;
	GLfloat lastX  =  800  / 2.0;
	GLfloat lastY  =  600 / 2.0;
	
	sf::Image diffuseMap;
	if (!diffuseMap.loadFromFile("container2.png"))
		return -1;
	
	GLuint diffuseMapID;
	GLint uniform_diffuseMap;
	
	glGenTextures(1, &diffuseMapID);
	glBindTexture(GL_TEXTURE_2D, diffuseMapID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, diffuseMap.getSize().x, diffuseMap.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, diffuseMap.getPixelsPtr());
	glGenerateMipmap(GL_TEXTURE_2D);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
     
	sf::Image specularMap;
	if (!specularMap.loadFromFile("container2_specular.png"))
		return -1;
	
	GLuint specularMapID;
	GLint uniform_specularMap;
	
	glGenTextures(1, &specularMapID);
	glBindTexture(GL_TEXTURE_2D, specularMapID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, specularMap.getSize().x, specularMap.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, specularMap.getPixelsPtr());
	glGenerateMipmap(GL_TEXTURE_2D);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

	bool firstMouse = true;
	
	auto t_start = std::chrono::high_resolution_clock::now();
	
	const sf::Vector2i windowCenter( 400, 300 );

    while (window.isOpen())
    {
		sf::Time elapsed = clock.restart();
		float dt = elapsed.asSeconds();
		
		updateTime += elapsed;
		updateNumFrames += 1;
		if(updateTime >= sf::seconds(1.0f))
		{
			std::cout << "FPS: " << updateNumFrames << std::endl;
			
			updateTime -= sf::seconds(1.0f);
			updateNumFrames = 0;
		}
        
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
					if(event.key.code == sf::Keyboard::E)
					{
						if(wireframe) {
							glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
							wireframe = false;
						}
						else
						{
							glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
							wireframe = true;
						}
					}
					if(event.key.code == sf::Keyboard::F)
						lightPos.y += 0.1;
					if(event.key.code == sf::Keyboard::R)
						lightPos.y -= 0.1;
				}
            }
        }

        auto t_now = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();
        
        sf::Vector2f windowSize = static_cast<sf::Vector2f>(window.getSize());
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        
		float xpos = mousePosition.x;
		float ypos = mousePosition.y;
        
        if(firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}
  
		GLfloat xoffset = xpos - lastX;
		GLfloat yoffset = lastY - ypos; 
		lastX = xpos;
		lastY = ypos;

		GLfloat sensitivity = 0.1;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		yaw   += xoffset;
		pitch += yoffset;

		if(pitch > 89.0f)
			pitch = 89.0f;
		if(pitch < -89.0f)
			pitch = -89.0f;

		glm::vec3 front;
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		cameraFront = glm::normalize(front);
		
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			cameraPos += 2.0f * dt * cameraFront;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			cameraPos -= 2.0f * dt * cameraFront;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			cameraPos -= 2.0f * dt * glm::normalize(glm::cross(cameraFront, cameraUp));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			cameraPos += 2.0f * dt * glm::normalize(glm::cross(cameraFront, cameraUp));
		}

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//camera + cubes shader
		glUseProgram(shaderProgram);

        GLint viewPosLoc = glGetUniformLocation(shaderProgram, "viewPos");

        glUniform3f(viewPosLoc, cameraPos.x, cameraPos.y, cameraPos.z);
        
        float moveX = sin(time) * 0.8f;
        float moveY = cos(time ) * 0.8f;
		
		glUniform1f(glGetUniformLocation(shaderProgram, "material.shininess"), 32.0f);
        // == ==========================
        // Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index 
        // the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
        // by defining light types as classes and set their values in there, or by using a more efficient uniform approach
        // by using 'Uniform buffer objects', but that is something we discuss in the 'Advanced GLSL' tutorial.
        // == ==========================
        // Directional light
        glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
        glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.ambient"), 0.05f, 0.05f, 0.05f);
        glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.diffuse"), 0.4f, 0.4f, 0.4f);
        glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.specular"), 0.5f, 0.5f, 0.5f);

        // Point light 1
        glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[0].position"), pointLightPositions[0].x * moveX, pointLightPositions[0].y, pointLightPositions[0].z * moveY);
        glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[0].ambient"), 0.05f, 0.05f, 0.05f);
        glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[0].diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[0].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[0].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[0].linear"), 0.09);
        glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[0].quadratic"), 0.032);
        // Point light 2
        glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[1].position"), pointLightPositions[1].x * moveX, pointLightPositions[1].y, pointLightPositions[1].z * moveY);
        glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[1].ambient"), 0.05f, 0.05f, 0.05f);
        glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[1].diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[1].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[1].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[1].linear"), 0.09);
        glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[1].quadratic"), 0.032);
        // Point light 3
        glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[2].position"), pointLightPositions[2].x * moveX, pointLightPositions[2].y, pointLightPositions[2].z * moveY);
        glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[2].ambient"), 0.05f, 0.05f, 0.05f);
        glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[2].diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[2].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[2].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[2].linear"), 0.09);
        glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[2].quadratic"), 0.032);
        // Point light 4
        glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[3].position"), pointLightPositions[3].x * moveX, pointLightPositions[3].y, pointLightPositions[3].z * moveY);
        glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[3].ambient"), 0.05f, 0.05f, 0.05f);
        glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[3].diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[3].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[3].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[3].linear"), 0.09);
        glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[3].quadratic"), 0.032);
        
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(uniform_diffuseMap, GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMapID);
			
		glActiveTexture(GL_TEXTURE1);
		glUniform1i(uniform_specularMap, GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMapID);

        glm::mat4 view;
        glm::mat4 projection;

		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        projection = glm::perspective(45.0f, 800.0f/600.0f, 0.1f, 100.0f);
        
        GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
        GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
        GLint projLoc = glGetUniformLocation(shaderProgram, "projection");

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glm::mat4 model;
        glBindVertexArray(vao);
        for (GLuint i = 0; i < 10; ++i)
        {
			model = glm::mat4();
            model = glm::translate(model, cubePositions[i]);
            model = glm::rotate(model, 20.0f * i, glm::vec3(1.0f, 0.3f, 0.5f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glBindVertexArray(0);
        
        //LAMP shader
        glUseProgram(shaderProgramLamp);

        // Get location objects for the matrices on the lamp shader (these could be different on a different shader)
        modelLoc = glGetUniformLocation(shaderProgramLamp, "model");
        viewLoc  = glGetUniformLocation(shaderProgramLamp, "view");
        projLoc  = glGetUniformLocation(shaderProgramLamp, "projection");
        // Set matrices
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        
        // We now draw as many light bulbs as we have point lights.
        glBindVertexArray(lightVAO);
        for (GLuint i = 0; i < 4; i++)
        {
            model = glm::mat4();
            model = glm::translate(model, glm::vec3(pointLightPositions[i].x * moveX, pointLightPositions[i].y, pointLightPositions[i].z * moveY));
            model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glBindVertexArray(0);

        window.display();
    }

    glDeleteProgram(shaderProgram);
    glDeleteBuffers(1, &vbo);
}
