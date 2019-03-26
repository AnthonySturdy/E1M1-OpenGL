#include "HelloGL.h"


HelloGL::HelloGL(int argc, char* argv[]) {

	InitGL(argc, argv);
	InitObjects();
	glutMainLoop();
}

HelloGL::~HelloGL() {

}

void HelloGL::InitGL(int argc, char* argv[]) {
	GLUTCallbacks::Init(this);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH);
	glutInitDisplayMode(GLUT_DOUBLE);

	//Create window
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutCreateWindow("Simple OpenGL Program");
	
	//Enable Depth (So far objects don't draw in front of near)
	glEnable(GL_DEPTH_TEST);

	//Give glut our functions
	glutDisplayFunc(GLUTCallbacks::Display);
	glutTimerFunc(REFRESHRATE, GLUTCallbacks::Timer, REFRESHRATE);
	glutKeyboardFunc(GLUTCallbacks::Keyboard);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//Set viewport to be entire window
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	//FOV, Aspect Ratio, Near Clipping, Far Clipping
	gluPerspective(90, 1, 0.1, 1000);

	//Go back to Model view
	glMatrixMode(GL_MODELVIEW);

	//Enable back-face culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//Enable texturing
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glEnable(GL_TEXTURE_2D);

	//Enable lighting
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}

void HelloGL::InitObjects() {
	lightPosition = new Vector4(0.0f, 0.0f, 1.0f, 0.0f);
	lightData = new Lighting(Vector4(0.2f, 0.2f, 0.2f, 1.0f), Vector4(0.8f, 0.8f, 0.8f, 1.0f), Vector4(0.2f, 0.2f, 0.2f, 1.0f));

	camera = new Camera();
	camera->eye = Vector3(-5.0f, -5.0f, -5.0f);
	camera->centre = Vector3(0.0f, 0.0f, 0.0f);
	camera->up = Vector3(0.0f, 1.0f, 0.0f);

	Texture2D* texture = new Texture2D();
	texture->LoadFromRAW("Penguins.raw", 512, 512);

	TexturedMesh* cubeMesh = MeshLoader::LoadOBJ("pinkTeapot.obj", texture);
	SceneObject* object = new Cube(cubeMesh, Vector3(0, 0, 0));
	objects.push_back(object);
	/*for (int i = 0; i < 600; i++) {
		SceneObject* object = new Cube(cubeMesh, Vector3((rand() % 400 / 10.0f) - 20.0f,
			(rand() % 200 / 10.0f) - 10.0f,
			-(rand() % 1000)));
		objects.push_back(object);
	}*/
}

void HelloGL::Display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (int i = 0; i < 1; i++) {
		objects[i]->Draw();
	}

	glFlush();
	glutSwapBuffers();
}

void HelloGL::Update() {
	glLoadIdentity();	//Reset matrix

	gluLookAt(camera->eye.x, camera->eye.y, camera->eye.z, 
				camera->centre.x, camera->centre.y, camera->centre.z, 
				camera->up.x, camera->up.y, camera->up.z);

	glutPostRedisplay();

	for (int i = 0; i < 1; i++) {
		objects[i]->Update();
	}

	glLightfv(GL_LIGHT0, GL_AMBIENT, &(lightData->ambient.x));
	glLightfv(GL_LIGHT0, GL_POSITION, &(lightPosition->x));

	POINT mousePos;
	GetCursorPos(&mousePos);
	camera->up.y += mousePos.y - prevMouse.y;
	camera->up.z += mousePos.x - prevMouse.x;

	prevMouse = mousePos;
}

void HelloGL::Keyboard(unsigned char key, int x, int y) {
	if (key == 'd') {
		camera->eye.z += 2.0f;
	}
	if (key == 'a') {
		camera->eye.z -= 2.0f;
	}
	if (key == 'w') {
		camera->eye.x += 2.0f;
	}
	if (key == 's') {
		camera->eye.x -= 2.0f;
	}
}