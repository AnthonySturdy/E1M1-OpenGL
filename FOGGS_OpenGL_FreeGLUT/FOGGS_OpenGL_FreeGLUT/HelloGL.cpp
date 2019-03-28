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
	gluPerspective(90, float(SCREEN_WIDTH) / float(SCREEN_HEIGHT), 0.01, 1000);

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
	SceneObject* object = new MeshObject(levelMesh, Vector3(0, 0, 0));
	objects.push_back(object);

	Texture2D* spaceTexture = new Texture2D();
	spaceTexture->LoadFromData(BMPLoader::LoadBitMap("Assets/Textures/SPACE.bmp"), 256, 256);
	navigationMesh = MeshLoader::LoadOBJ("Assets/Models/E1M1_Navigation.obj", spaceTexture);
	SceneObject* navigationObject = new MeshObject(navigationMesh, Vector3(0, 0.01f, 0));
	objects.push_back(navigationObject);
}

void HelloGL::Display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (int i = 0; i < 1; i++) {
		objects[i]->Draw();
	}

	//DebugNavigationMesh();

	glFlush();
	glutSwapBuffers();
}

void HelloGL::Update() {
	glLoadIdentity();	//Reset matrix

	Movement();
	CameraLook();

	glutPostRedisplay();

	for (int i = 0; i < 1; i++) {
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

	//https://community.khronos.org/t/about-glulookat-function-and-how-to-rotate-the-camera/67868/2
	glRotatef(yAngle, 1.0f, 0.0f, 0.0f);
	glRotatef(xAngle, 0.0f, 1.0f, 0.0f);
	glTranslatef(-camera->eye.x, -(camera->eye.y + PLAYER_HEIGHT + groundHeight), -camera->eye.z);

	prevGroundHeight = groundHeight;

	SetCursorPos(1920 - (SCREEN_WIDTH / 2), SCREEN_HEIGHT / 2);
}

void HelloGL::DebugNavigationMesh() {
	glBindTexture(GL_TEXTURE_2D, 0);	//Bind the texture to the ID
	Triangle* t = nullptr;
	for (int i = 0; i < navigationMesh->indexCount / 3; i++) {
		if (IsPointInsideTriangle(camera->eye, navigationMesh->tris[i])) {
			t = &navigationMesh->tris[i];
		}

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glDisable(GL_DEPTH_TEST);
		glBegin(GL_TRIANGLES);
		glVertex3f(navigationMesh->tris[i].v1->x, navigationMesh->tris[i].v1->y, navigationMesh->tris[i].v1->z);
		glVertex3f(navigationMesh->tris[i].v2->x, navigationMesh->tris[i].v2->y, navigationMesh->tris[i].v2->z);
		glVertex3f(navigationMesh->tris[i].v3->x, navigationMesh->tris[i].v3->y, navigationMesh->tris[i].v3->z);
		glEnd();
		glEnable(GL_DEPTH_TEST);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if (t != nullptr) {
		glDisable(GL_DEPTH_TEST);
		glBegin(GL_TRIANGLES);
		glVertex3f(t->v1->x, t->v1->y, t->v1->z);
		glVertex3f(t->v2->x, t->v2->y, t->v2->z);
		glVertex3f(t->v3->x, t->v3->y, t->v3->z);
		glEnd();
		glEnable(GL_DEPTH_TEST);
	}
}

void HelloGL::Movement() {
	if (isMovingForward) {
		//Convert current player rotation to radians, then vector. Add to position.
		camera->centre.x += cos((xAngle - 90) * 3.141f / 180.0f) * PLAYER_WALK_SPEED;
		camera->centre.z += sin((xAngle - 90) * 3.141f / 180.0f) * PLAYER_WALK_SPEED;
		camera->eye.x += cos((xAngle - 90) * 3.141f / 180.0) * PLAYER_WALK_SPEED;
		camera->eye.z += sin((xAngle - 90) * 3.141f / 180.0) * PLAYER_WALK_SPEED;

		//Check if player goes out of bounds, if they do, reset position back (Wall collision)
		if (!CanPlayerMove()) {
			camera->centre.x -= cos((xAngle - 90) * 3.141f / 180.0f) * PLAYER_WALK_SPEED;
			camera->centre.z -= sin((xAngle - 90) * 3.141f / 180.0f) * PLAYER_WALK_SPEED;
			camera->eye.x -= cos((xAngle - 90) * 3.141f / 180.0) * PLAYER_WALK_SPEED;
			camera->eye.z -= sin((xAngle - 90) * 3.141f / 180.0) * PLAYER_WALK_SPEED;
		}
	}
	if (isMovingBackward) {
		//Convert current player rotation to radians, then vector. Subtract from position.
		camera->centre.x -= cos((xAngle - 90) * 3.141f / 180.0f) * PLAYER_WALK_SPEED;
		camera->centre.z -= sin((xAngle - 90) * 3.141f / 180.0f) * PLAYER_WALK_SPEED;
		camera->eye.x -= cos((xAngle - 90) * 3.141f / 180.0) * PLAYER_WALK_SPEED;
		camera->eye.z -= sin((xAngle - 90) * 3.141f / 180.0) * PLAYER_WALK_SPEED;

		//Check if player goes out of bounds, if they do, reset position back (Wall collision)
		if (!CanPlayerMove()) {
			camera->centre.x += cos((xAngle - 90) * 3.141f / 180.0f) * PLAYER_WALK_SPEED;
			camera->centre.z += sin((xAngle - 90) * 3.141f / 180.0f) * PLAYER_WALK_SPEED;
			camera->eye.x += cos((xAngle - 90) * 3.141f / 180.0) * PLAYER_WALK_SPEED;
			camera->eye.z += sin((xAngle - 90) * 3.141f / 180.0) * PLAYER_WALK_SPEED;
		}
	}
	if (isMovingRight) {
		//Convert current player rotation to radians, then vector. Add to position.
		camera->centre.x += cos(xAngle * 3.141f / 180.0f) * PLAYER_WALK_SPEED;
		camera->centre.z += sin(xAngle * 3.141f / 180.0f) * PLAYER_WALK_SPEED;
		camera->eye.x += cos(xAngle * 3.141f / 180.0) * PLAYER_WALK_SPEED;
		camera->eye.z += sin(xAngle * 3.141f / 180.0) * PLAYER_WALK_SPEED;

		//Check if player goes out of bounds, if they do, reset position back (Wall collision)
		if (!CanPlayerMove()) {
			camera->centre.x -= cos(xAngle * 3.141f / 180.0f) * PLAYER_WALK_SPEED;
			camera->centre.z -= sin(xAngle * 3.141f / 180.0f) * PLAYER_WALK_SPEED;
			camera->eye.x -= cos(xAngle * 3.141f / 180.0) * PLAYER_WALK_SPEED;
			camera->eye.z -= sin(xAngle * 3.141f / 180.0) * PLAYER_WALK_SPEED;
		}
	}
	if (isMovingLeft) {
		//Convert current player rotation to radians, then vector. Subtract from position.
		camera->centre.x -= cos(xAngle * 3.141f / 180.0f) * PLAYER_WALK_SPEED;
		camera->centre.z -= sin(xAngle * 3.141f / 180.0f) * PLAYER_WALK_SPEED;
		camera->eye.x -= cos(xAngle * 3.141f / 180.0) * PLAYER_WALK_SPEED;
		camera->eye.z -= sin(xAngle * 3.141f / 180.0) * PLAYER_WALK_SPEED;

		//Check if player goes out of bounds, if they do, reset position back (Wall collision)
		if (!CanPlayerMove()) {
			camera->centre.x += cos(xAngle * 3.141f / 180.0f) * PLAYER_WALK_SPEED;
			camera->centre.z += sin(xAngle * 3.141f / 180.0f) * PLAYER_WALK_SPEED;
			camera->eye.x += cos(xAngle * 3.141f / 180.0) * PLAYER_WALK_SPEED;
			camera->eye.z += sin(xAngle * 3.141f / 180.0) * PLAYER_WALK_SPEED;
		}
	}
}

bool HelloGL::CanPlayerMove() {
	bool cancelMovement = true;

	for (int i = 0; i < navigationMesh->indexCount / 3; i++) {
		if (IsPointInsideTriangle(camera->eye, navigationMesh->tris[i])) {
			cancelMovement = false;
			break;
		} else {
			cancelMovement = true;
		}
	}

	if ((GetGroundHeightAtPoint(camera->eye, navigationMesh) - prevGroundHeight) > PLAYER_MAX_STEP_HEIGHT) {
		cancelMovement = true;
	}

	return !cancelMovement;
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

float sign(Vector3 p1, Vertex* p2, Vertex* p3) {
	return (p1.x - p3->x) * (p2->z - p3->z) - (p2->x - p3->x) * (p1.z - p3->z);
}

//Solution found here: https://stackoverflow.com/questions/2049582/how-to-determine-if-a-point-is-in-a-2d-triangle
bool HelloGL::IsPointInsideTriangle(Vector3 point, Triangle tri) {
	float d1, d2, d3;
	bool has_neg, has_pos;

	d1 = sign(point, tri.v1, tri.v2);
	d2 = sign(point, tri.v2, tri.v3);
	d3 = sign(point, tri.v3, tri.v1);

	has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
	has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

	return !(has_neg && has_pos);
}

float HelloGL::GetGroundHeightAtPoint(Vector3 point, TexturedMesh* mesh) {
	for (int i = 0; i < mesh->indexCount / 3; i++) {
		if (IsPointInsideTriangle(point, mesh->tris[i])) {
			return GetTriangleHeight(mesh->tris[i]);
		}
	}
	return 0.0f;
}