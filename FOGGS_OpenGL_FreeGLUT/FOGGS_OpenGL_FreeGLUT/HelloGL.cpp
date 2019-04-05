#include "HelloGL.h"

HelloGL::HelloGL(int argc, char* argv[]) {
	InitGL(argc, argv);
	InitObjects();
	glutMainLoop();
}

HelloGL::~HelloGL() {
	delete camera;

	for (int i = 0; i < objects.size(); i++) {
		delete objects[i];
	}
	objects.clear();

	delete navigationMesh;
	delete armourObject1;
	delete armourObject2;
	delete gunObject;
	
	delete lightPosition;
	delete lightData;
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
	glutSpecialFunc(GLUTCallbacks::SpecialDown);
	glutSpecialUpFunc(GLUTCallbacks::SpecialUp);

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
	glEnable(GL_TEXTURE_2D);

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

	Texture2D* levelTexture = new Texture2D();
	levelTexture->LoadFromData(BMPLoader::LoadBitMap("Assets/Textures/E1M1_Texture.bmp"), 2048, 2048);
	TexturedMesh* levelMesh = MeshLoader::LoadOBJ("Assets/Models/E1M1_Textured.obj", levelTexture);
	SceneObject* levelObject = new MeshObject(levelMesh, Vector3(0, 0, 0));
	objects.push_back(levelObject);

	navigationMesh = MeshLoader::LoadOBJ("Assets/Models/E1M1_Navigation.obj", levelTexture);

	Texture2D* skyboxTexture = new Texture2D();
	skyboxTexture->LoadFromData(BMPLoader::LoadBitMap("Assets/Textures/skybox.bmp"), 512, 512);
	TexturedMesh* skyboxMesh = MeshLoader::LoadOBJ("Assets/Models/Skybox.obj", skyboxTexture);
	SceneObject* skyboxObject = new MeshObject(skyboxMesh, Vector3(0, 0, 0));
	objects.push_back(skyboxObject);

	Texture2D* armourTexture = new Texture2D();
	armourTexture->LoadFromData(BMPLoader::LoadBitMap("Assets/Textures/ArmourTexture.bmp"), 256, 256);
	TexturedMesh* armourMesh = MeshLoader::LoadOBJ("Assets/Models/ArmourPickup.obj", armourTexture);
	armourObject1 = new SphereColliderObject(armourMesh, Vector3(127.8f, 14.3f, 40.7f), 5);
	armourObject2 = new SphereColliderObject(armourMesh, Vector3(-78.3f, -6.5f, 36.8f), 5);

	Texture2D* gunTexture = new Texture2D();
	gunTexture->LoadFromData(BMPLoader::LoadBitMap("Assets/Textures/shotgun.bmp"), 512, 512);
	TexturedMesh* gunMesh = MeshLoader::LoadOBJ("Assets/Models/Shotgun.obj", gunTexture);
	gunObject = new MeshObject(gunMesh, Vector3(0, 0, 0));


	//Scene Graph Heirarchy
	BinaryNode* head;
	BinaryNode* armourSG1, *armourSG2, *levelSG, *gunSG, *skyboxSG;
	armourSG1 = SceneGraph::MakeNode(armourObject1, "Armour 1");
	armourSG2 = SceneGraph::MakeNode(armourObject2, "Armour 2");
	armourSG2->sibling = armourSG1;
	armourSG1->sibling = armourSG2;
	levelSG = SceneGraph::MakeNode(levelObject, "E1M1 Level");
	levelSG->child = armourSG1;
	gunSG = SceneGraph::MakeNode(gunObject, "Shotgun");
	levelSG->sibling = gunSG;
	gunSG->sibling = levelSG;
	skyboxSG = SceneGraph::MakeNode(skyboxObject, "Skybox");
	skyboxSG->child = levelSG;
	head = skyboxSG;

	SceneGraph::Traverse(head);
}

void HelloGL::Display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Draw Level
	objects[0]->Draw();

	//Draw armour pieces
	glPushMatrix();
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		if (armourObject1->GetIsActive())
			armourObject1->Draw();
		if (armourObject2->GetIsActive())
			armourObject2->Draw();
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHTING);
	glPopMatrix();

	//Draw gun
	glPushMatrix();
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);

		float y_offset = 0;
		//			<-Check if player is moving------------------------------------------>		  <-Frequency--------------------------------------------------------------------------------------------> <-Mag>
		y_offset = ((isMovingForward || isMovingBackward || isMovingLeft || isMovingRight) ? sin((glutGet(GLUT_ELAPSED_TIME) / 3000.0f) * PLAYER_WALK_SPEED * (isSprinting ? PLAYER_SPRINT_MULTIPLIER : 1)) / 3 : 0);
		glTranslatef(camera->eye.x, camera->eye.y - 4 + y_offset, camera->eye.z);
		glRotatef(-(xAngle+90), 0.0f, 1.0f, 0.0f);
		gunObject->Draw();
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHTING);
	glPopMatrix();

	//Draw skybox
	glPushMatrix();
		glTranslatef(camera->eye.x, camera->eye.y, camera->eye.z);
		objects[1]->Draw();
	glPopMatrix();

	//DebugNavigationMesh();

	//Draw UI
	std::stringstream ss1;
	ss1 << "Armour: " << armourCount;
	DrawString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)(ss1.str().c_str()), 200, 200);
	std::stringstream ss2;
	ss2 << "FPS: " << round(1/deltaTime);
	DrawString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)(ss2.str().c_str()), 200, 175);
	std::stringstream ss3;
	ss3 << "PlayerPos: " << "(" << round(camera->eye.x * 100) / 100 << ", " << round((camera->eye.y - PLAYER_HEIGHT) * 100) / 100 << ", " << round(camera->eye.z * 100) / 100 << ")";
	DrawString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)(ss3.str().c_str()), 200, 150);

	glFlush();
	glutSwapBuffers();
}

void HelloGL::Update() {
	deltaTime = (glutGet(GLUT_ELAPSED_TIME) / 1000.0f) - prevElapsedTime;
	prevElapsedTime = (glutGet(GLUT_ELAPSED_TIME) / 1000.0f);

	if(isSprinting)
		std::cout << isSprinting << std::endl;

	if (!isMovingForward && !isMovingBackward && !isMovingLeft && !isMovingRight) {
		isSprinting = false;
	}

	glLoadIdentity();	//Reset matrix

	Movement();
	CameraLook();

	glutPostRedisplay();

	for (int i = 0; i < 2; i++) {
		objects[i]->Update(deltaTime);
	}

	gunObject->Update(deltaTime);

	if(armourObject1->GetIsActive())
		armourObject1->Update(deltaTime);
	if (armourObject2->GetIsActive())
		armourObject2->Update(deltaTime);
	
	if (armourObject1->IsPointInCollider(camera->eye) && armourObject1->GetIsActive()) {
		//Collect armour
		armourCount += 100;
		armourObject1->SetIsActive(false);
	} else if (armourObject2->IsPointInCollider(camera->eye) && armourObject2->GetIsActive()) {
		//Collect armour
		armourCount += 200;
		armourObject2->SetIsActive(false);
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

	//If is capital / player is pressing shift
	if (k != key) {
		isSprinting = true;
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

void HelloGL::SpecialDown(int key, int x, int y) {
	if (key == GLUT_ACTIVE_SHIFT) {
		isSprinting = true;
	}
}

void HelloGL::SpecialUp(int key, int x, int y) {
	if (key == GLUT_ACTIVE_SHIFT) {
		isSprinting = false;
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

void HelloGL::DrawString(void* font, const unsigned char * str, int x, int y) {
	//Draw UI		https://stackoverflow.com/questions/20866508/using-glut-to-simply-print-text
	glDisable(GL_TEXTURE_2D);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);
		glMatrixMode(GL_MODELVIEW);

		glPushMatrix();
			glLoadIdentity();
			glRasterPos2f(x, y);
			glutBitmapString(font, str);
		glPopMatrix();

		glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_TEXTURE_2D);
}

void HelloGL::Movement() {
	if (isMovingForward) {
		//Convert current player rotation to radians, then vector. Add to position.
		camera->centre.x += cos((xAngle - 90) * 3.141f / 180.0f) * PLAYER_WALK_SPEED * (isSprinting ? PLAYER_SPRINT_MULTIPLIER : 1) * deltaTime;
		camera->centre.z += sin((xAngle - 90) * 3.141f / 180.0f) * PLAYER_WALK_SPEED * (isSprinting ? PLAYER_SPRINT_MULTIPLIER : 1) * deltaTime;
		camera->eye.x += cos((xAngle - 90) * 3.141f / 180.0) * PLAYER_WALK_SPEED * (isSprinting ? PLAYER_SPRINT_MULTIPLIER : 1) * deltaTime;
		camera->eye.z += sin((xAngle - 90) * 3.141f / 180.0) * PLAYER_WALK_SPEED * (isSprinting ? PLAYER_SPRINT_MULTIPLIER : 1) * deltaTime;

		//Check if player goes out of bounds, if they do, reset position back (Wall collision)
		if (!CanPlayerMove()) {
			camera->centre.x -= cos((xAngle - 90) * 3.141f / 180.0f) * PLAYER_WALK_SPEED * (isSprinting ? PLAYER_SPRINT_MULTIPLIER : 1) * deltaTime;
			camera->centre.z -= sin((xAngle - 90) * 3.141f / 180.0f) * PLAYER_WALK_SPEED * (isSprinting ? PLAYER_SPRINT_MULTIPLIER : 1) * deltaTime;
			camera->eye.x -= cos((xAngle - 90) * 3.141f / 180.0) * PLAYER_WALK_SPEED * (isSprinting ? PLAYER_SPRINT_MULTIPLIER : 1) * deltaTime;
			camera->eye.z -= sin((xAngle - 90) * 3.141f / 180.0) * PLAYER_WALK_SPEED * (isSprinting ? PLAYER_SPRINT_MULTIPLIER : 1) * deltaTime;
		}
	}
	if (isMovingBackward) {
		//Convert current player rotation to radians, then vector. Subtract from position.
		camera->centre.x -= cos((xAngle - 90) * 3.141f / 180.0f) * PLAYER_WALK_SPEED * (isSprinting ? PLAYER_SPRINT_MULTIPLIER : 1) * deltaTime;
		camera->centre.z -= sin((xAngle - 90) * 3.141f / 180.0f) * PLAYER_WALK_SPEED * (isSprinting ? PLAYER_SPRINT_MULTIPLIER : 1) * deltaTime;
		camera->eye.x -= cos((xAngle - 90) * 3.141f / 180.0) * PLAYER_WALK_SPEED * (isSprinting ? PLAYER_SPRINT_MULTIPLIER : 1) * deltaTime;
		camera->eye.z -= sin((xAngle - 90) * 3.141f / 180.0) * PLAYER_WALK_SPEED * (isSprinting ? PLAYER_SPRINT_MULTIPLIER : 1) * deltaTime;

		//Check if player goes out of bounds, if they do, reset position back (Wall collision)
		if (!CanPlayerMove()) {
			camera->centre.x += cos((xAngle - 90) * 3.141f / 180.0f) * PLAYER_WALK_SPEED * (isSprinting ? PLAYER_SPRINT_MULTIPLIER : 1) * deltaTime;
			camera->centre.z += sin((xAngle - 90) * 3.141f / 180.0f) * PLAYER_WALK_SPEED * (isSprinting ? PLAYER_SPRINT_MULTIPLIER : 1) * deltaTime;
			camera->eye.x += cos((xAngle - 90) * 3.141f / 180.0) * PLAYER_WALK_SPEED * (isSprinting ? PLAYER_SPRINT_MULTIPLIER : 1) * deltaTime;
			camera->eye.z += sin((xAngle - 90) * 3.141f / 180.0) * PLAYER_WALK_SPEED * (isSprinting ? PLAYER_SPRINT_MULTIPLIER : 1) * deltaTime;
		}
	}
	if (isMovingRight) {
		//Convert current player rotation to radians, then vector. Add to position.
		camera->centre.x += cos(xAngle * 3.141f / 180.0f) * PLAYER_WALK_SPEED * (isSprinting ? PLAYER_SPRINT_MULTIPLIER : 1) * deltaTime;
		camera->centre.z += sin(xAngle * 3.141f / 180.0f) * PLAYER_WALK_SPEED * (isSprinting ? PLAYER_SPRINT_MULTIPLIER : 1) * deltaTime;
		camera->eye.x += cos(xAngle * 3.141f / 180.0) * PLAYER_WALK_SPEED * (isSprinting ? PLAYER_SPRINT_MULTIPLIER : 1) * deltaTime;
		camera->eye.z += sin(xAngle * 3.141f / 180.0) * PLAYER_WALK_SPEED * (isSprinting ? PLAYER_SPRINT_MULTIPLIER : 1) * deltaTime;

		//Check if player goes out of bounds, if they do, reset position back (Wall collision)
		if (!CanPlayerMove()) {
			camera->centre.x -= cos(xAngle * 3.141f / 180.0f) * PLAYER_WALK_SPEED * (isSprinting ? PLAYER_SPRINT_MULTIPLIER : 1) * deltaTime;
			camera->centre.z -= sin(xAngle * 3.141f / 180.0f) * PLAYER_WALK_SPEED * (isSprinting ? PLAYER_SPRINT_MULTIPLIER : 1) * deltaTime;
			camera->eye.x -= cos(xAngle * 3.141f / 180.0) * PLAYER_WALK_SPEED * (isSprinting ? PLAYER_SPRINT_MULTIPLIER : 1) * deltaTime;
			camera->eye.z -= sin(xAngle * 3.141f / 180.0) * PLAYER_WALK_SPEED * (isSprinting ? PLAYER_SPRINT_MULTIPLIER : 1) * deltaTime;
		}
	}
	if (isMovingLeft) {
		//Convert current player rotation to radians, then vector. Subtract from position.
		camera->centre.x -= cos(xAngle * 3.141f / 180.0f) * PLAYER_WALK_SPEED * (isSprinting ? PLAYER_SPRINT_MULTIPLIER : 1) * deltaTime;
		camera->centre.z -= sin(xAngle * 3.141f / 180.0f) * PLAYER_WALK_SPEED * (isSprinting ? PLAYER_SPRINT_MULTIPLIER : 1) * deltaTime;
		camera->eye.x -= cos(xAngle * 3.141f / 180.0) * PLAYER_WALK_SPEED * (isSprinting ? PLAYER_SPRINT_MULTIPLIER : 1) * deltaTime;
		camera->eye.z -= sin(xAngle * 3.141f / 180.0) * PLAYER_WALK_SPEED * (isSprinting ? PLAYER_SPRINT_MULTIPLIER : 1) * deltaTime;

		//Check if player goes out of bounds, if they do, reset position back (Wall collision)
		if (!CanPlayerMove()) {
			camera->centre.x += cos(xAngle * 3.141f / 180.0f) * PLAYER_WALK_SPEED * (isSprinting ? PLAYER_SPRINT_MULTIPLIER : 1) * deltaTime;
			camera->centre.z += sin(xAngle * 3.141f / 180.0f) * PLAYER_WALK_SPEED * (isSprinting ? PLAYER_SPRINT_MULTIPLIER : 1) * deltaTime;
			camera->eye.x += cos(xAngle * 3.141f / 180.0) * PLAYER_WALK_SPEED * (isSprinting ? PLAYER_SPRINT_MULTIPLIER : 1) * deltaTime;
			camera->eye.z += sin(xAngle * 3.141f / 180.0) * PLAYER_WALK_SPEED * (isSprinting ? PLAYER_SPRINT_MULTIPLIER : 1) * deltaTime;
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