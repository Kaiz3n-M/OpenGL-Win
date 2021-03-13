#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

#include <stdlib.h>

const int32_t WIDTH = 800;
const int32_t HEIGHT = 600;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

GLFWwindow* initWindow();
void initGlad();

uint32_t setupShaders();
uint32_t setupVertex(std::vector<char>&);
uint32_t setupFragment(std::vector<char>&);

uint32_t setupBuffers();
void draw(uint32_t, uint32_t);

int main() 
{
	GLFWwindow* window = initWindow();
	
	// Make the OpenGL contex current
	glfwMakeContextCurrent(window);
	// Register callback that is called each time a window will be resized
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// GLAD manages function pointers for OpenGL so we want to initialize GLAD
	// before we call any OpenGL function
	initGlad();

	// Setup Viewport
	// glViewport(0, 0, WIDTH, HEIGHT);

	uint32_t shaderProgram = setupShaders();
	uint32_t VAO = setupBuffers();

	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	// Render loop
	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		// rendering
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		/* draw our first triangle */
		draw(shaderProgram, VAO);
		
		// check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	// Kill the window that GLFW has created
	// and frees the memory and all recoruces connected to it
	glfwTerminate();
	return 0;
}

GLFWwindow* initWindow()
{
	// Init GLFW
	glfwInit();
	// Infrom GLFW that we are using OpenGL 3.3 core
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create GLFW window and OpenGL context
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create the GLFW window" << std::endl;
		glfwTerminate();
		exit(-1);
	}

	return window;
}

void initGlad() 
{
	// Init GLAD
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

		exit(-1);
	}
}

uint32_t setupShaders()
{
	std::vector<char> infoLog(512); // this vector is used to store the log message coming from the GPU
	uint32_t vertexShader = setupVertex(infoLog);
	uint32_t fragmentShader = setupFragment(infoLog);

	// Create Shader program
	uint32_t shaderProgram;
	shaderProgram = glCreateProgram();

	// Attach the previously compiled shaders to the program object and then link them 
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// Check if the linking was successful
	int32_t success;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, infoLog.size(), nullptr, infoLog.data());
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog.data() << std::endl;
	}
	
	// Delete shader objects
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

uint32_t setupVertex(std::vector<char>& infoLog)
{
	// Create Vertex Shader
	uint32_t vertexShader; //variable that stores the ID of the shader object that will created by GPU
	vertexShader = glCreateShader(GL_VERTEX_SHADER); // create an shader object in the GPU so we don't see it but we get an ID back
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	
	// Compile vertex shader
	glCompileShader(vertexShader); 

	// Check if the compilation was successful
	int32_t success;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) 
	{
		glGetShaderInfoLog(vertexShader, infoLog.size(), nullptr, infoLog.data());
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog.data() << std::endl;
	}

	return vertexShader;
}

uint32_t setupFragment(std::vector<char>& infoLog)
{
	// Create Fragment Shader
	uint32_t fragmentShader; // variable that stores the ID of the shader object that will created by GPU
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // create an shader object in the GPU so we don't see it but we get an ID back
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	
	// Compile fragment shader
	glCompileShader(fragmentShader);

	// Check if the compilation was successful
	int32_t success;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, infoLog.size(), nullptr, infoLog.data());
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog.data() << std::endl;
	}

	return fragmentShader;
}

uint32_t setupBuffers()
{
	// Triangle verices
	std::vector<float> triangleVertices = {
		//     x  |   y  |  z
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			 0.0f,  0.5f, 0.0f
	};

	std::vector<float>  quadVertices = {
		0.5f,  0.5f, 0.0f,  // top right
		0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left 
	};

	std::vector<uint32_t> indices = { // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	std::vector<float> mVertices = {
		-0.25f, -0.5f, 0.0f, // 1_bottom right
		-0.5f, -0.5f, 0.0f, // 1_bottom left
		-0.5f, 0.5f, 0.0f, // 1_top left
		-0.25f, 0.5f, 0.0f, // 1_top right
		
		 0.1f, 0.1f, 0.0f, // 2_top right
	    -0.25f, 0.27, 0.0f, // 2_bottom left

		0.45f, 0.5f, 0.0f, // 3_top left
	    0.45f, 0.27, 0.0f, // 2_bottom right

		0.45f, -0.5f, 0.0f, // 3_bottom left
		0.7f, 0.5f, 0.0f, // 3_top right
		0.7f, -0.5f, 0.0f, // 3_bottom right
	};

	std::vector<uint32_t> mIndices = { 
		0, 1, 3,   // first triangle
		1, 2, 3,    // second triangle
		3, 4, 5,
		7, 6, 4,
		6, 9, 8,
		8, 10, 9
	};


	/* 1. Create and bind Vertex Array Object */
	uint32_t VAO;
	glGenVertexArrays(1, &VAO); // // Create buffer in graphic card's memory and return buffer handle into VAO
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	/* 2. Create and copy our vertices array in a vertex buffer for OpenGL to use */
	// Create Vertex Buffer Object and Vertex Array Object
	uint32_t VBO;
	// Create buffer in graphic card's memory and return buffer handle into VBO
	glGenBuffers(1, &VBO); 

	// Binding buffer in OpenGL means "select" a buffer. Since OpenGL is contextual when create a buffer
	// and we want to work with it we have to select it. We do that with glBindBuffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // Bind buffer by VBO handle to the object create by glGenBuffer call
	glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(float), mVertices.data(), GL_STATIC_DRAW); // Write inside VBO buffer

	/* 3. Create and copy our index array in a element buffer for OpenGL to use */
	uint32_t EBO;
	glGenBuffers(1, &EBO);// Create buffer in graphic card's memory and return buffer handle into EBO

	// Bind Element Buffer Object
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(int), mIndices.data(), GL_STATIC_DRAW);
	
	/* 4. then set the vertex attributes pointers*/

	// We need to tell OpenGL how to interpret the vertex buffer to draw a triangle
	// <First Param>: A vertex is a blob of data, it doesn't just have position data, it can contain also, UV data, normals and etc.
	//				  We ara using a buffer to specify position data for this vertex. So we use attribute 0 to tell we are specifying only position data,
	//				  we did specify that in the layout (location = 0) in the vertex shader
	// <Second Param>: specifies the size of the vertex attribute. The vertex attribute is a vec3 so it is composed of 3 values.
	// <Third Param>: specifies the type of the data which is GL_FLOAT
	// <Fourth Param>: specifies if we want the data to be normalized
	// <Fifth Param>: is known as the stride which is the amount of bites between each vertex. So for example if we have a position
	//				  buffer with 3 floats per vertices, plus a UV (buffer of two element) and normal (buffer of three element) buffers
	//				  we will have 3 floats per vertex data so 12 bytes + 2 floats for UV so 8 bytes + 3 floats for the normals so 12 bytes again
	//				  so 32 bytes total for the stride		
	// <Sixth Param>: is the offset of where the position data begins in the buffer.
	//				  The position data is at the start of the data array so 0.
	//				  This takes the vertex buffer that was bound by glBindBuffer and it is implicit, so the first call to
	//				  glBindBuffer setup a buffer that as location of '0', in case of a second call to glBindBuffer we would get
	//				  a location of '1'
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	return VAO;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// Each time a window is resized by the user this callback is called
	// and we need to resize the viewport to the new dimensions
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	// GLFW helps us to find out if the user is clicking any key on the keyboard 
	// In this case we are chekcking if the ESCAPE key has been pressed 
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		// if the ESCAPE key has been pressed we can signal GLFW that we want to close this window
		// so GLFW can start deallocating all the resources associated to it
		glfwSetWindowShouldClose(window, true);
	}
}

void draw(uint32_t shaderProgram, uint32_t VAO)
{
	// Activated the program object
	glUseProgram(shaderProgram);
	glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
}