#pragma once

class HelloGL;

namespace GLUTCallbacks {
	void Init(HelloGL* gl);
	void Display();
	void Timer(int preferredRefresh);
	void KeyboardDown(unsigned char key, int x, int y);
	void KeyboardUp(unsigned char key, int x, int y);
}