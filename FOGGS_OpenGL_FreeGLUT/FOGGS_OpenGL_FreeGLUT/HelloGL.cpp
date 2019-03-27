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
	glutFullScreen();
	
	//Enable Depth (So far objects don't draw in front of near)
	glEnable(GL_DEPTH_TEST);

	//Give glut our functions
	glutDisplayFunc(GLUTCallbacks::Display);
	glutTimerFunc(REFRESHRATE, GLUTCallbacks::Timer, REFRESHRATE);
	glutKeyboardFunc(GLUTCallbacks::KeyboardDown);
	glutKeyboardUpFunc(GLUTCallbacks::KeyboardUp);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//Set viewport to be entire window
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	//FOV, Aspect Ratio, Near Clipping, Far Clipping
	gluPerspective(90, float(SCREEN_WIDTH) / float(SCREEN_HEIGHT), 0.1, 1000);

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
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);

	//Disable Mouse
	glutSetCursor(GLUT_CURSOR_NONE);
}

void HelloGL::InitObjects() {
	lightPosition = new Vector4(0.0f, 0.0f, 1.0f, 0.0f);
	lightData = new Lighting(Vector4(0.2f, 0.2f, 0.2f, 1.0f), Vector4(0.8f, 0.8f, 0.8f, 1.0f), Vector4(0.2f, 0.2f, 0.2f, 1.0f));

	camera = new Camera();
	camera->eye = Vector3(-5.0f, -5.0f, -5.0f);
	camera->centre = Vector3(0.0f, 0.0f, 0.0f);
	camera->up = Vector3(0.0f, 1.0f, 0.0f);

	Texture2D* penguinTexture = new Texture2D();
	penguinTexture->LoadFromData(BMPLoader::LoadBitMap("Assets/Textures/Banana.bmp"), 400, 400);
	TexturedMesh* teapotMesh = MeshLoader::LoadOBJ("Assets/Models/doom_E1M1.obj", penguinTexture);
	SceneObject* object = new MeshObject(teapotMesh, Vector3(0, 0, 0));
	objects.push_back(object);

	TexturedMesh* treeMesh = MeshLoader::LoadOBJ("Assets/Models/Lowpoly_tree_sample.obj", penguinTexture);
	SceneObject* treeObject = new MeshObject(treeMesh, Vector3(-30, 0, -30));
	objects.push_back(treeObject);
}

void HelloGL::Display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (int i = 0; i < 2; i++) {
		objects[i]->Draw();
	}

	glFlush();
	glutSwapBuffers();
}

void HelloGL::Update() {
	glLoadIdentity();	//Reset matrix

	//First Person Camera Rotation
	POINT mousePos;
	GetCursorPos(&mousePos);
	//Reset Angle on full rotation
	if (xAngle > 360) {
		xAngle -= 360;
	} else if (xAngle < 0) {
		xAngle += 360;
	}

	//Lock Up/Down rotation
	if (yAngle > 90) {
		yAngle = 90;
	} else if (yAngle < -90) {
		yAngle = -90;
	}
	xAngle += (mousePos.x - (SCREEN_WIDTH / 2)) * MOUSE_SENSITIVITY;
	yAngle += (mousePos.y - (SCREEN_HEIGHT / 2)) * MOUSE_SENSITIVITY;

	//https://community.khronos.org/t/about-glulookat-function-and-how-to-rotate-the-camera/67868/2
	glRotatef(yAngle, 1.0f, 0.0f, 0.0f);
	glRotatef(xAngle, 0.0f, 1.0f, 0.0f);
	glTranslatef(-camera->eye.x, -(camera->eye.y + PLAYER_HEIGHT), -camera->eye.z);

	SetCursorPos(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);


	Movement();

	glutPostRedisplay();

	for (int i = 0; i < 2; i++) {
		objects[i]->Update();
	}

	glLightfv(GL_LIGHT0, GL_AMBIENT, &(lightData->ambient.x));
	glLightfv(GL_LIGHT0, GL_POSITION, &(lightPosition->x));
}

void HelloGL::KeyboardDown(unsigned char key, int x, int y) {
	if (key == 'w') {
		//Convert current player rotation to radians, then vector. Add to position.
		isMovingForward = true;
	} 
	if (key == 's') {
		//Convert current player rotation to radians, then vector. Subtract from position.
		isMovingBackward = true;
	} 
	if (key == 'd') {
		//Convert current player rotation to radians, then vector. Add to position.
		isMovingRight = true;
	} 
	if (key == 'a') {
		//Convert current player rotation to radians, then vector. Subtract from position.
		isMovingLeft = true;
	} 
}

void HelloGL::KeyboardUp(unsigned char key, int x, int y) {
	if (key == 'w') {
		//Convert current player rotation to radians, then vector. Add to position.
		isMovingForward = false;
	}
	if (key == 's') {
		//Convert current player rotation to radians, then vector. Subtract from position.
		isMovingBackward = false;
	}
	if (key == 'd') {
		//Convert current player rotation to radians, then vector. Add to position.
		isMovingRight = false;
	}
	if (key == 'a') {
		//Convert current player rotation to radians, then vector. Subtract from position.
		isMovingLeft = false;
	}
}

void HelloGL::Movement() {
	if (isMovingForward) {
		//Convert current player rotation to radians, then vector. Add to position.
		camera->centre.x += cos((xAngle - 90) * 3.141f / 180.0f) * PLAYER_WALK_SPEED;
		camera->centre.z += sin((xAngle - 90) * 3.141f / 180.0) * PLAYER_WALK_SPEED;
		camera->eye.x += cos((xAngle - 90) * 3.141f / 180.0) * PLAYER_WALK_SPEED;
		camera->eye.z += sin((xAngle - 90) * 3.141f / 180.0) * PLAYER_WALK_SPEED;
	}
	if (isMovingBackward) {
		//Convert current player rotation to radians, then vector. Subtract from position.
		camera->centre.x -= cos((xAngle - 90) * 3.141f / 180.0f) * PLAYER_WALK_SPEED;
		camera->centre.z -= sin((xAngle - 90) * 3.141f / 180.0) * PLAYER_WALK_SPEED;
		camera->eye.x -= cos((xAngle - 90) * 3.141f / 180.0) * PLAYER_WALK_SPEED;
		camera->eye.z -= sin((xAngle - 90) * 3.141f / 180.0) * PLAYER_WALK_SPEED;
	}
	if (isMovingRight) {
		//Convert current player rotation to radians, then vector. Add to position.
		camera->centre.x += cos(xAngle * 3.141f / 180.0f) * PLAYER_WALK_SPEED;
		camera->centre.z += sin(xAngle * 3.141f / 180.0) * PLAYER_WALK_SPEED;
		camera->eye.x += cos(xAngle * 3.141f / 180.0) * PLAYER_WALK_SPEED;
		camera->eye.z += sin(xAngle * 3.141f / 180.0) * PLAYER_WALK_SPEED;
	}
	if (isMovingLeft) {
		//Convert current player rotation to radians, then vector. Subtract from position.
		camera->centre.x -= cos(xAngle * 3.141f / 180.0f) * PLAYER_WALK_SPEED;
		camera->centre.z -= sin(xAngle * 3.141f / 180.0) * PLAYER_WALK_SPEED;
		camera->eye.x -= cos(xAngle * 3.141f / 180.0) * PLAYER_WALK_SPEED;
		camera->eye.z -= sin(xAngle * 3.141f / 180.0) * PLAYER_WALK_SPEED;
	}
}