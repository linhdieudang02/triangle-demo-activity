#include "ExampleApp.h"

#define FONTSTASH_IMPLEMENTATION
#include <fontstash.h>
#define GLFONTSTASH_IMPLEMENTATION
#include <glfontstash.h>

#include <config/VRDataIndex.h>

#include <glm/gtx/orthonormalize.hpp>

using namespace basicgraphics;
using namespace std;
using namespace glm;

ExampleApp::ExampleApp(int argc, char** argv) : VRApp(argc, argv)
{
	_lastTime = 0.0;
    _curFrameTime = 0.0;
}

ExampleApp::~ExampleApp()
{
	glfonsDelete(fs);
	shutdown();
}

void ExampleApp::onAnalogChange(const VRAnalogEvent &event) {
    // This routine is called for all Analog_Change events.  Check event->getName()
    // to see exactly which analog input has been changed, and then access the
    // new value with event->getValue().
    
	if (event.getName() == "FrameStart") {
		_lastTime = _curFrameTime;
		_curFrameTime = event.getValue();
	}


}

void ExampleApp::onButtonDown(const VRButtonEvent &event) {
    // This routine is called for all Button_Down events.  Check event->getName()
    // to see exactly which button has been pressed down.
	
	//std::cout << "ButtonDown: " << event.getName() << std::endl;
}

void ExampleApp::onButtonUp(const VRButtonEvent &event) {
    // This routine is called for all Button_Up events.  Check event->getName()
    // to see exactly which button has been released.

	//std::cout << "ButtonUp: " << event.getName() << std::endl;
}

void ExampleApp::onCursorMove(const VRCursorEvent &event) {
	// This routine is called for all mouse move events. You can get the absolute position
	// or the relative position within the window scaled 0--1.
	
	//std::cout << "MouseMove: "<< event.getName() << " " << event.getPos()[0] << " " << event.getPos()[1] << std::endl;
}

void ExampleApp::onTrackerMove(const VRTrackerEvent &event) {
    // This routine is called for all Tracker_Move events.  Check event->getName()
    // to see exactly which tracker has moved, and then access the tracker's new
    // 4x4 transformation matrix with event->getTransform().

	// We will use trackers when we do a virtual reality assignment. For now, you can ignore this input type.
}

void ExampleApp::onRenderGraphicsContext(const VRGraphicsState &renderState) {
    // This routine is called once per graphics context at the start of the
    // rendering process.  So, this is the place to initialize textures,
    // load models, or do other operations that you only want to do once per
    // frame.
    
	// Is this the first frame that we are rendering after starting the app?
    if (renderState.isInitialRenderCall()) {

		//For windows, we need to initialize a few more things for it to recognize all of the
		// opengl calls.
		#ifndef __APPLE__
			glewExperimental = GL_TRUE;
			GLenum err = glewInit();
			if (GLEW_OK != err)
			{
				std::cout << "Error initializing GLEW." << std::endl;
			}
		#endif     


        glEnable(GL_DEPTH_TEST);
        glClearDepth(1.0f);
        glDepthFunc(GL_LEQUAL);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		glEnable(GL_MULTISAMPLE);

		// This sets the background color that is used to clear the canvas
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

		// This load shaders from disk, we do it once when the program starts up.
		reloadShaders();

		initializeText();
    

        //TODO: Initialize the _mesh variable with a triangle mesh uploaded to the GPU.

        std::vector<basicgraphics::Mesh::Vertex> cpuVertexArray;
        std::vector<int> cpuIndexArray;
        std::vector<std::shared_ptr<basicgraphics::Texture>> textures;

        auto tex = basicgraphics::Texture::create2DTextureFromFile("tex.jpg");
        textures.push_back(tex);

        // Create 3 triangle vertices
        basicgraphics::Mesh::Vertex v1, v2, v3;

        v1.position = glm::vec3( 0.0f,  0.5f, 0.0f);
        v1.normal = glm::vec3(0.0f, 0.0f, 1.0f);
        v1.texCoord0 = glm::vec2(0.5f, 1.0f);

        v2.position = glm::vec3(-0.5f, -0.5f, 0.0f);
        v2.normal = glm::vec3(0.0f, 0.0f, 1.0f);
        v2.texCoord0 = glm::vec2(0.0f, 0.0f);

        v3.position = glm::vec3( 0.5f, -0.5f, 0.0f);
        v3.normal = glm::vec3(0.0f, 0.0f, 1.0f);
        v3.texCoord0 = glm::vec2(1.0f, 0.0f);

        cpuVertexArray.push_back(v1);
        cpuVertexArray.push_back(v2);
        cpuVertexArray.push_back(v3);

        // Optional index data
        cpuIndexArray = {0, 1, 2};

        int vertexByteSize = static_cast<int>(sizeof(basicgraphics::Mesh::Vertex) * cpuVertexArray.size());
        int indexByteSize = static_cast<int>(sizeof(int) * cpuIndexArray.size());

        _mesh.reset(new basicgraphics::Mesh(
            textures,
            GL_TRIANGLES,
            GL_STATIC_DRAW,
            vertexByteSize,
            indexByteSize,
            0,                    // vertex offset
            cpuVertexArray,       // vertex data (vector passed by ref)
            static_cast<int>(cpuIndexArray.size()),
            indexByteSize,
            cpuIndexArray.data()  // pointer to index data
        ));
        
//        // BRET'S WAY: Initialize the _mesh variable with a triangle mesh uploaded to the GPU.
//        std::vector<Mesh::Vertex> cpuVertexArray;
//        std::vector<int> cpuIndexArray;
//        std::vector<std::shared_ptr<Texture>> textures;
//
//        vec3 normal(0, 0, 1);
//
//        Mesh::Vertex vert;
//        vert.position = vec3(0, 0, 0);
//        vert.normal = normal;
//        vert.texCoord0 = glm::vec2(0, 0);
//        cpuVertexArray.push_back(vert);
//        cpuIndexArray.push_back(0);
//
//        vert.position = vec3(1, 0, 0);
//        vert.normal = normal;
//        vert.texCoord0 = glm::vec2(1, 0);
//        cpuVertexArray.push_back(vert);
//        cpuIndexArray.push_back(1);
//
//        vert.position = vec3(1, 1, 0);
//        vert.normal = normal;
//        vert.texCoord0 = glm::vec2(1, 1);
//        cpuVertexArray.push_back(vert);
//        cpuIndexArray.push_back(2);
//
//        // std::shared_ptr<Texture> tex = Texture::create2DTextureFromFile("tex.jpg");
//        // textures.push_back(tex);
//
//        const int numVertices = cpuVertexArray.size();
//        const int cpuVertexByteSize = sizeof(Mesh::Vertex) * numVertices;
//        const int cpuIndexByteSize = sizeof(int) * cpuIndexArray.size();
//
//        _mesh.reset(new Mesh(
//            textures,
//            GL_TRIANGLE_STRIP,
//            GL_STATIC_DRAW,
//            cpuVertexByteSize,
//            cpuIndexByteSize,
//            0,
//            cpuVertexArray,
//            cpuIndexArray.size(),
//            cpuIndexByteSize,
//            &cpuIndexArray[0]
//        ));
//
//        _mesh->setMaterialColor(vec4(1, 0, 0, 1.0));
    }
}

void ExampleApp::onRenderGraphicsScene(const VRGraphicsState &renderState) {
    // This routine is called once per eye/camera.  This is the place to actually
    // draw the scene.
    
	// clear the canvas and other buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// Setup the view matrix to set where the camera is located in the scene
    glm::vec3 eye_world = glm::vec3(0, 0, 5);
    glm::mat4 view = glm::lookAt(eye_world, glm::vec3(0,0,0), glm::vec3(0,1,0));

	// Setup the projection matrix so that things are rendered in perspective
	GLfloat windowHeight = renderState.index().getValue("FramebufferHeight");
	GLfloat windowWidth = renderState.index().getValue("FramebufferWidth");
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), windowWidth / windowHeight, 0.01f, 100.0f);
	
	// Setup the model matrix
	glm::mat4 model = glm::mat4(1.0);
    
	// Tell opengl we want to use this specific shader.
	_shader.use();
	
	_shader.setUniform("view_mat", view);
	_shader.setUniform("projection_mat", projection);
	
	_shader.setUniform("model_mat", model);
	_shader.setUniform("normal_mat", mat3(transpose(inverse(model))));
	_shader.setUniform("eye_world", eye_world);
    
    // Draw the mesh
    _mesh->draw(_shader);

}

void ExampleApp::drawText(const std::string text, float xPos, float yPos, GLfloat windowHeight, GLfloat windowWidth) {
	//float lh = 0;
	//fonsVertMetrics(fs, NULL, NULL, &lh);
	//double width = fonsTextBounds(fs, text.c_str(), NULL, NULL) + 40;
	//double height = lh + 40;

	_textShader.use();
	_textShader.setUniform("projection_mat", glm::ortho(0.f, windowWidth, windowHeight, 0.f, -1.f, 1.f));
	_textShader.setUniform("view_mat", glm::mat4(1.0));
	_textShader.setUniform("model_mat", glm::mat4(1.0));
	_textShader.setUniform("lambertian_texture", 0);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	fonsDrawText(fs, xPos, yPos, text.c_str(), NULL);

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	_shader.use();

}

void ExampleApp::reloadShaders()
{
	_shader.compileShader("texture.vert", GLSLShader::VERTEX);
	_shader.compileShader("texture.frag", GLSLShader::FRAGMENT);
	_shader.link();
	_shader.use();
}

void ExampleApp::initializeText() {
	int fontNormal = FONS_INVALID;
	fs = nullptr;

	fs = glfonsCreate(512, 512, FONS_ZERO_TOPLEFT);
	if (fs == NULL) {
		assert(false);//Could not create stash
	}

	fontNormal = fonsAddFont(fs, "sans", "DroidSansMono.ttf");
	if (fontNormal == FONS_INVALID) {
		assert(false);// Could not add font normal.
	}

	unsigned int black = glfonsRGBA(0, 0, 0, 255);

	fonsClearState(fs);
	fonsSetSize(fs, 20);
	fonsSetFont(fs, fontNormal);
	fonsSetColor(fs, black);
	fonsSetAlign(fs, FONS_ALIGN_LEFT | FONS_ALIGN_TOP);

	_textShader.compileShader("textRendering.vert", GLSLShader::VERTEX);
	_textShader.compileShader("textRendering.frag", GLSLShader::FRAGMENT);
	_textShader.link();
}
