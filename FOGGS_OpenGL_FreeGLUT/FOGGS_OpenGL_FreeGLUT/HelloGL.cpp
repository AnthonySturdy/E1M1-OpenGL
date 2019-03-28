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
	glutInitWindowPosition(1920-1280, 0);
	glutCreateWindow("Simple OpenGL Program");
	//glutFullScreen();

	
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
	camera->eye = Vector3(0, 0, 0);
	camera->centre = Vector3(0.0f, 0.0f, 0.0f);
	camera->up = Vector3(0.0f, 1.0f, 0.0f);

	Texture2D* bananaTexture = new Texture2D();
	bananaTexture->LoadFromData(BMPLoader::LoadBitMap("Assets/Textures/Banana.bmp"), 400, 400);
	TexturedMesh* levelMesh = MeshLoader::LoadOBJ("Assets/Models/E1M1_Edited.obj", bananaTexture);
	SceneObject* object = new MeshObject(levelMesh, Vector3(0, 10.0f, 0));
	objects.push_back(object);

	Texture2D* spaceTexture = new Texture2D();
	spaceTexture->LoadFromData(BMPLoader::LoadBitMap("Assets/Textures/SPACE.bmp"), 256, 256);
	navigationMesh = MeshLoader::LoadOBJ("Assets/Models/E1M1_Navigation.obj", spaceTexture);
	SceneObject* navigationObject = new MeshObject(navigationMesh, Vector3(0, 10.01f, 0));
	objects.push_back(navigationObject);
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

	CameraLook();
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

void HelloGL::CameraLook() {
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
	xAngle += (mousePos.x - (1920 - (SCREEN_WIDTH / 2))) * MOUSE_SENSITIVITY;
	yAngle += (mousePos.y - (SCREEN_HEIGHT / 2)) * MOUSE_SENSITIVITY;

	float groundHeight = GetGroundHeightAtPoint(camera->eye, navigationMesh);
	std::cout << "CameraPos: (" << camera->eye.x << ", " << camera->eye.y << ", " << camera->eye.z << ")" << std::endl;
	std::cout << "GroundHeight: " << groundHeight << std::endl << std::endl;

	//https://community.khronos.org/t/about-glulookat-function-and-how-to-rotate-the-camera/67868/2
	glRotatef(yAngle, 1.0f, 0.0f, 0.0f);
	glRotatef(xAngle, 0.0f, 1.0f, 0.0f);
	glTranslatef(-camera->eye.x, -(camera->eye.y + PLAYER_HEIGHT + groundHeight), -camera->eye.z);

	SetCursorPos(1920 - (SCREEN_WIDTH / 2), SCREEN_HEIGHT / 2);
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

float HelloGL::GetTriangleHeight(Triangle tri) {
	//Although theres only horizontal triangles on navigation mesh, often one of the vertices' Y position is different to the others (Not sure why),
	//so this ensures the anomaly vertex isn't used.

	if (tri.v1->y == tri.v2->y) {
		//If v1 and v2 are the same, neither of them are the anomaly. 
		return tri.v1->y;
	} else {
		//Otherwise, we know v3 isn't the anomaly.
		return tri.v3->y;
	}
}

//Solution found here: https://stackoverflow.com/questions/2049582/how-to-determine-if-a-point-is-in-a-2d-triangle. Used answer by user "John Bananas".
bool HelloGL::IsPointInsideTriangle(Vector3 point, Triangle tri) {
	int x = (point.x);
	int z = (point.z);

	bool s_ab = (tri.v2->x - tri.v1->x) * z - (tri.v2->z - tri.v1->z) * x > 0;

	if (((tri.v3->x - tri.v1->x) * z - (tri.v3->z - tri.v1->z) * x > 0) == s_ab)
		return false;

	if (((tri.v3->x - tri.v2->x)*(point.z - tri.v2->z) - (tri.v3->z - tri.v2->z)*(point.x - tri.v2->x) > 0) != s_ab)
		return false;

	return true;
}

float HelloGL::GetGroundHeightAtPoint(Vector3 point, TexturedMesh* mesh) {
	for (int i = 0; i < mesh->vertexCount / 3; i++) {
		if (IsPointInsideTriangle(point, mesh->tris[i])) {
			std::cout << "CurTri v1: " << mesh->tris[i].v1->x << ", " << mesh->tris[i].v1->z << std::endl;
			std::cout << "CurTri v2: " << mesh->tris[i].v2->x << ", " << mesh->tris[i].v2->z << std::endl;
			std::cout << "CurTri v3: " << mesh->tris[i].v3->x << ", " << mesh->tris[i].v3->z << std::endl;
			return GetTriangleHeight(mesh->tris[i]);
		}
	}

	return 0.0f;
}
