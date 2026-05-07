#include <stdafx.h>
#include <Debug.h>
#include <Scene.h>
jmp_buf jump_buffer_1;

void __print__(void* src) {
	MessageBox(NULL, static_cast<LPCWSTR>(src), L"´íÎó", MB_OK | MB_ICONQUESTION);
};
void __ERROR__(int i) {
	switch (i) {
	case 0:
		Scene::reset();
		break;
	case 1:
		longjmp(jump_buffer_1,1);
		break;
	}
}
