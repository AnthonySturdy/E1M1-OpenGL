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
	gluPerspective(90, float(SCREEN_WIDTH) / float(SCREEN_HEIGHT), 0.01, 1000);

	//Go back to Model view
	glMatrixMode(GL_MODELVIEW);

	//Enable back-face culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//Enable texturing
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
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
	camera->eye = Vector3(0, PLAYER_HEIGHT, 0);
	camera->centre = Vector3(0.0f, 0.0f, 0.0f);
	camera->up = Vector3(0.0f, 1.0f, 0.0f);

	Texture2D* bananaTexture = new Texture2D();
	bananaTexture->LoadFromData(BMPLoader::LoadBitMap("Assets/Textures/E1M1_Texture.bmp"), 2048, 2048);
	TexturedMesh* levelMesh = MeshLoader::LoadOBJ("Assets/Models/E1M1_Textured.obj", bananaTexture);
	SceneObject* object = new MeshObject(levelMesh, Vector3(0, 0, 0));
	objects.push_back(object);

	navigationMesh = MeshLoader::LoadOBJ("Assets/Models/E1M1_Navigation.obj", bananaTexture);

	Texture2D* cubemapTexture = new Texture2D();
	cubemapTexture->LoadFromData(BMPLoader::LoadBitMap("Assets/Textures/skybox.bmp"), 1024, 1024);
	TexturedMesh* skyboxMesh = MeshLoader::LoadOBJ("Assets/Models/Skybox.obj", cubemapTexture);
	SceneObject* cubemapObject = new MeshObject(skyboxMesh, Vector3(0, 0, 0));
	objects.push_back(cubemapObject);
}

void HelloGL::Display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (int i = 0; i < 1; i++) {
		objects[i]->Draw();
	}

	//Draw skybox
	glPushMatrix();
	glTranslatef(camera->eye.x, camera->eye.y, camera->eye.z);
	objects[1]->Draw();
	glPopMatrix();

	//DebugNavigationMesh();

	glFlush();
	glutSwapBuffers();
}

void HelloGL::Update() {
	deltaTime = (glutGet(GLUT_ELAPSED_TIME) / 1000.0f) - prevElapsedTime;
	prevElapsedTime = (glutGet(GLUT_ELAPSED_TIME) / 1000.0f);

	glLoadIdentity();	//Reset matrix

	Movement();
	CameraLook();

	glutPostRedisplay();

	for (int i = 0; i < 2; i++) {
		objects[i]->Update();
	}

	glLightfv(GL_LIGHT0, GL_AMBIENT, &(lightData->ambient.x));
	glLightfv(GL_LIGHT0, GL_POSITION, &(lightPosition->x));
}

void HelloGL::KeyboardDown(unsigned char key, int x, int y) {
	unsigned char k = tolower(key);	//Fix problem where input is different when caps lock on

	if (k == 'w') {
		//Convert current player rotation to radians, then vector. Add to position.
		isMovingForward = true;
	} 
	if (k == 's') {
		//Convert current player rotation to radians, then vector. Subtract from position.
		isMovingBackward = true;
	} 
	if (k == 'd') {
		//Convert current player rotation to radians, then vector. Add to position.
		isMovingRight = true;
	} 
	if (k == 'a') {
		//Convert current player rotation to radians, then vector. Subtract from position.
		isMovingLeft = true;
	} 
}

void HelloGL::KeyboardUp(unsigned char key, int x, int y) {
	unsigned char k = tolower(key);	//Fix problem where input is different when caps lock on

	if (k == 'w') {
		//Convert current player rotation to radians, then vector. Add to position.
		isMovingForward = false;
	}
	if (k == 's') {
		//Convert current player rotation to radians, then vector. Subtract from position.
		isMovingBackward = false;
	}
	if (k == 'd') {
		//Convert current player rotation to radians, then vector. Add to position.
		isMovingRight = false;
	}
	if (k == 'a') {
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
	xAngle += (mousePos.x - (SCREEN_WIDTH / 2)) * MOUSE_SENSITIVITY * deltaTime;
	yAngle += (mousePos.y - (SCREEN_HEIGHT / 2)) * MOUSE_SENSITIVITY * deltaTime;

	float groundHeight = GetGroundHeightAtPoint(camera->eye, navigationMesh);

	//https://community.khronos.org/t/about-glulookat-function-and-how-to-rotate-the-camera/67868/2
	glRotatef(yAngle, 1.0f, 0.0f, 0.0f);
	glRotatef(xAngle, 0.0f, 1.0f, 0.0f);

	//Smooth verical movement
	if (camera->eye.y < (PLAYER_HEIGHT + groundHeight)) {
		camera->eye.y += deltaTime * PLAYER_STEP_UP_SPEED;

		//If goes too far
		if (camera->eye.y > (PLAYER_HEIGHT + groundHeight)) {
			camera->eye.y = PLAYER_HEIGHT + groundHeight;
		}
	} 
	if (camera->eye.y > (PLAYER_HEIGHT + groundHeight)) {
		camera->eye.y -= deltaTime * PLAYER_STEP_DOWN_SPEED;
		
		//If goes too far
		if (camera->eye.y < (PLAYER_HEIGHT + groundHeight)) {
			camera->eye.y = PLAYER_HEIGHT + groundHeight;
		}
	}

	glTranslatef(-camera->eye.x, -camera->eye.y, -camera->eye.z);

	prevGroundHeight = groundHeight;

	SetCursorPos(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
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
		camera->centre.x += cos((xAngle - 90) * 3.141f / 180.0f) * PLAYER_WALK_SPEED * deltaTime;
		camera->centre.z += sin((xAngle - 90) * 3.141f / 180.0f) * PLAYER_WALK_SPEED * deltaTime;
		camera->eye.x += cos((xAngle - 90) * 3.141f / 180.0) * PLAYER_WALK_SPEED * deltaTime;
		camera->eye.z += sin((xAngle - 90) * 3.141f / 180.0) * PLAYER_WALK_SPEED * deltaTime;

		//Check if player goes out of bounds, if they do, reset position back (Wall collision)
		if (!CanPlayerMove()) {
			camera->centre.x -= cos((xAngle - 90) * 3.141f / 180.0f) * PLAYER_WALK_SPEED * deltaTime;
			camera->centre.z -= sin((xAngle - 90) * 3.141f / 180.0f) * PLAYER_WALK_SPEED * deltaTime;
			camera->eye.x -= cos((xAngle - 90) * 3.141f / 180.0) * PLAYER_WALK_SPEED * deltaTime;
			camera->eye.z -= sin((xAngle - 90) * 3.141f / 180.0) * PLAYER_WALK_SPEED * deltaTime;
		}
	}
	if (isMovingBackward) {
		//Convert current player rotation to radians, then vector. Subtract from position.
		camera->centre.x -= cos((xAngle - 90) * 3.141f / 180.0f) * PLAYER_WALK_SPEED * deltaTime;
		camera->centre.z -= sin((xAngle - 90) * 3.141f / 180.0f) * PLAYER_WALK_SPEED * deltaTime;
		camera->eye.x -= cos((xAngle - 90) * 3.141f / 180.0) * PLAYER_WALK_SPEED * deltaTime;
		camera->eye.z -= sin((xAngle - 90) * 3.141f / 180.0) * PLAYER_WALK_SPEED * deltaTime;

		//Check if player goes out of bounds, if they do, reset position back (Wall collision)
		if (!CanPlayerMove()) {
			camera->centre.x += cos((xAngle - 90) * 3.141f / 180.0f) * PLAYER_WALK_SPEED * deltaTime;
			camera->centre.z += sin((xAngle - 90) * 3.141f / 180.0f) * PLAYER_WALK_SPEED * deltaTime;
			camera->eye.x += cos((xAngle - 90) * 3.141f / 180.0) * PLAYER_WALK_SPEED * deltaTime;
			camera->eye.z += sin((xAngle - 90) * 3.141f / 180.0) * PLAYER_WALK_SPEED * deltaTime;
		}
	}
	if (isMovingRight) {
		//Convert current player rotation to radians, then vector. Add to position.
		camera->centre.x += cos(xAngle * 3.141f / 180.0f) * PLAYER_WALK_SPEED * deltaTime;
		camera->centre.z += sin(xAngle * 3.141f / 180.0f) * PLAYER_WALK_SPEED * deltaTime;
		camera->eye.x += cos(xAngle * 3.141f / 180.0) * PLAYER_WALK_SPEED * deltaTime;
		camera->eye.z += sin(xAngle * 3.141f / 180.0) * PLAYER_WALK_SPEED * deltaTime;

		//Check if player goes out of bounds, if they do, reset position back (Wall collision)
		if (!CanPlayerMove()) {
			camera->centre.x -= cos(xAngle * 3.141f / 180.0f) * PLAYER_WALK_SPEED * deltaTime;
			camera->centre.z -= sin(xAngle * 3.141f / 180.0f) * PLAYER_WALK_SPEED * deltaTime;
			camera->eye.x -= cos(xAngle * 3.141f / 180.0) * PLAYER_WALK_SPEED * deltaTime;
			camera->eye.z -= sin(xAngle * 3.141f / 180.0) * PLAYER_WALK_SPEED * deltaTime;
		}
	}
	if (isMovingLeft) {
		//Convert current player rotation to radians, then vector. Subtract from position.
		camera->centre.x -= cos(xAngle * 3.141f / 180.0f) * PLAYER_WALK_SPEED * deltaTime;
		camera->centre.z -= sin(xAngle * 3.141f / 180.0f) * PLAYER_WALK_SPEED * deltaTime;
		camera->eye.x -= cos(xAngle * 3.141f / 180.0) * PLAYER_WALK_SPEED * deltaTime;
		camera->eye.z -= sin(xAngle * 3.141f / 180.0) * PLAYER_WALK_SPEED * deltaTime;

		//Check if player goes out of bounds, if they do, reset position back (Wall collision)
		if (!CanPlayerMove()) {
			camera->centre.x += cos(xAngle * 3.141f / 180.0f) * PLAYER_WALK_SPEED * deltaTime;
			camera->centre.z += sin(xAngle * 3.141f / 180.0f) * PLAYER_WALK_SPEED * deltaTime;
			camera->eye.x += cos(xAngle * 3.141f / 180.0) * PLAYER_WALK_SPEED * deltaTime;
			camera->eye.z += sin(xAngle * 3.141f / 180.0) * PLAYER_WALK_SPEED * deltaTime;
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
	return tri.v1->y;
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