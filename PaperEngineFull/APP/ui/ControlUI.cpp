#include "ControlUI.h"
#include <stddef.h>
#include "util/Engine_Define.h"
#include "util/Paper_Math.h"

#include <assert.h>

//template<> ControlUI *Singleton<ControlUI>::msInstance = 0;
//
//ControlUI & ControlUI::getInstance()
//{
//	assert(msInstance);
//	return (*msInstance);
//}
//
//ControlUI * ControlUI::getInstancePtr()
//{
//	return msInstance;
//}

ControlUI::ControlUI() :
mpBar(0)
{
	mIsUseTw = false;

	if (mIsUseTw)
	{
		assert(initAntTweakBar());
	}	
}

ControlUI::~ControlUI()
{
	//SAFE_DELETE(mpBar);

	if (mIsUseTw)
	{
		TwTerminate();
	}	
}

bool ControlUI::initAntTweakBar()
{
	if (!mIsUseTw)
	{
		return false;
	}

	bool ret = false;	

	if (TwInit(TW_OPENGL_CORE, 0) == 1)
	{
		TwWindowSize(WIN_WIDTH, WIN_HEIGHT);

		assert(mpBar == 0);
		mpBar = TwNewBar("OGLDEV");

		//TwStructMember Vector3fMembers[] = {
		//	{ "x", TW_TYPE_FLOAT, offsetof(math::Vector3f, x), "" },
		//	{ "y", TW_TYPE_FLOAT, offsetof(math::Vector3f, y), "" },
		//	{ "z", TW_TYPE_FLOAT, offsetof(math::Vector3f, z), "" }
		//};

		//TwDefineStruct("Vector3f", Vector3fMembers, 3, sizeof(math::Vector3f), NULL, NULL);

		////TwStructMember AttenuationMembers[] = {
		////	{ "Const", TW_TYPE_FLOAT, offsetof(LightAttenuation, Constant), "" },
		////	{ "Linear", TW_TYPE_FLOAT, offsetof(LightAttenuation, Linear), "" },
		////	{ "Exp", TW_TYPE_FLOAT, offsetof(LightAttenuation, Exp), "" }
		////};

		////TwDefineStruct("Attenuation", AttenuationMembers, 3, sizeof(LightAttenuation), NULL, NULL);

		//TwBar *bar = 0;

		//bar = TwNewBar("OGLDEV");
		//typedef enum { BUDDHA, BUNNY, DRAGON } MESH_TYPE;
		//TwEnumVal Meshes[] = { { BUDDHA, "Buddha" }, { BUNNY, "Bunny" }, { DRAGON, "Dragon" } };
		//TwType MeshTwType = TwDefineEnum("MeshType", Meshes, 3);
		//int meshIndex = BUDDHA;
		//TwAddVarRW(bar, "Mesh", MeshTwType, &meshIndex, NULL);

		//TwAddSeparator(bar, "", NULL);

		//TwAddSeparator(bar, "", NULL);

		//int g_Rotation = 0;
		//TwAddVarRW(bar, "ObjRotation", TW_TYPE_QUAT4F, &g_Rotation, " axisz=-z ");


		////TwAddButton(bar, "AutoRotate", AutoRotateCB, NULL, " label='Auto rotate' ");

		//int speed = 10;
		//TwAddVarRW(bar, "Rot Speed", TW_TYPE_FLOAT, &speed			,
		//	" min=0 max=5 step=0.1 keyIncr=s keyDecr=d help='Rotation speed (turns/second)' ");

		//TwAddSeparator(bar, "", NULL);

		//float refresh = 0.1f;
		//TwSetParam(bar, NULL, "refresh", TW_PARAM_FLOAT, 1, &refresh);

		////TwDefine(" GLOBAL help='This example shows how to integrate AntTweakBar with OGLDEV.' "); // Message added to the help bar.

		//int gGLMajorVersion = 0;
		//TwAddVarRO(bar, "GL Major Version", TW_TYPE_INT32, &gGLMajorVersion, " label='Major version of GL' ");

		initTwType();

		ret = true;
	}

	return ret;
}

void ControlUI::initTwType()
{
	TwStructMember vec3fMembers[] = {
		{ "x", TW_TYPE_FLOAT, offsetof(math::Vector3f, x), "" },
		{ "y", TW_TYPE_FLOAT, offsetof(math::Vector3f, y), "" },
		{ "z", TW_TYPE_FLOAT, offsetof(math::Vector3f, z), "" }
	};
	mVector3fType = TwDefineStruct("Vector3f", vec3fMembers, 3, sizeof(math::Vector3f), 0, 0);

	//-----

	TwStructMember vec4fMembers[] = {
		{ "x", TW_TYPE_FLOAT, offsetof(XMVECTOR, x), "" },
		{ "y", TW_TYPE_FLOAT, offsetof(XMVECTOR, y), "" },
		{ "z", TW_TYPE_FLOAT, offsetof(XMVECTOR, z), "" },
		{ "w", TW_TYPE_FLOAT, offsetof(XMVECTOR, w), "" }
	};

	mVector4fType = TwDefineStruct("Vector4f", vec4fMembers, 4, sizeof(XMVECTOR), NULL, NULL);
}

bool ControlUI::keyPressed(const OIS::KeyEvent &arg)
{
	int atbKey = OISKeyToATBKey(arg.key);

	return (TwKeyPressed(atbKey, TW_KMOD_NONE) == 1);
}

bool ControlUI::mouseMoved(const OIS::MouseEvent &arg)
{
	int x = arg.state.X.abs;
	int y = arg.state.Y.abs;

	return (TwMouseMotion(x, y));
}

bool ControlUI::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	TwMouseButtonID btn = (id == OIS::MB_Left) ? TW_MOUSE_LEFT : TW_MOUSE_RIGHT;
	TwMouseAction ma = TW_MOUSE_PRESSED;

	return (TwMouseButton(ma, btn) == 1);
}

bool ControlUI::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	TwMouseButtonID btn = (id == OIS::MB_Left) ? TW_MOUSE_LEFT : TW_MOUSE_RIGHT;
	TwMouseAction ma = TW_MOUSE_RELEASED;

	return (TwMouseButton(ma, btn) == 1);
}

TwBar * ControlUI::getMainBar()
{
	return mpBar;
}

void ControlUI::draw()
{
	if (!mIsUseTw)
	{
		return;
	}

	TwDraw();
}

void ControlUI::addVector3(const char *name, math::Vector3f *pVec3)
{
	TwAddVarRW(mpBar, name, mVector3fType, pVec3, "");
}

void ControlUI::addVector4(const char *name, XMVECTOR *pVec4)
{
	TwAddVarRW(mpBar, name, mVector3fType, pVec4, "");
}

int ControlUI::OISKeyToATBKey(OIS::KeyCode oisKey)
{
	switch (oisKey)
	{
	case OIS::KC_UNASSIGNED:
		break;
	case OIS::KC_ESCAPE:
		return TW_KEY_ESCAPE;
		break;
	case OIS::KC_1:
		break;
	case OIS::KC_2:
		break;
	case OIS::KC_3:
		break;
	case OIS::KC_4:
		break;
	case OIS::KC_5:
		break;
	case OIS::KC_6:
		break;
	case OIS::KC_7:
		break;
	case OIS::KC_8:
		break;
	case OIS::KC_9:
		break;
	case OIS::KC_0:
		break;
	case OIS::KC_MINUS:
		break;
	case OIS::KC_EQUALS:
		break;
	case OIS::KC_BACK:
		break;
	case OIS::KC_TAB:
		break;
	case OIS::KC_Q:
		break;
	case OIS::KC_W:
		break;
	case OIS::KC_E:
		break;
	case OIS::KC_R:
		break;
	case OIS::KC_T:
		break;
	case OIS::KC_Y:
		break;
	case OIS::KC_U:
		break;
	case OIS::KC_I:
		break;
	case OIS::KC_O:
		break;
	case OIS::KC_P:
		break;
	case OIS::KC_LBRACKET:
		break;
	case OIS::KC_RBRACKET:
		break;
	case OIS::KC_RETURN:
		break;
	case OIS::KC_LCONTROL:
		break;
	case OIS::KC_A:
		break;
	case OIS::KC_S:
		break;
	case OIS::KC_D:
		break;
	case OIS::KC_F:
		break;
	case OIS::KC_G:
		break;
	case OIS::KC_H:
		break;
	case OIS::KC_J:
		break;
	case OIS::KC_K:
		break;
	case OIS::KC_L:
		break;
	case OIS::KC_SEMICOLON:
		break;
	case OIS::KC_APOSTROPHE:
		break;
	case OIS::KC_GRAVE:
		break;
	case OIS::KC_LSHIFT:
		break;
	case OIS::KC_BACKSLASH:
		break;
	case OIS::KC_Z:
		break;
	case OIS::KC_X:
		break;
	case OIS::KC_C:
		break;
	case OIS::KC_V:
		break;
	case OIS::KC_B:
		break;
	case OIS::KC_N:
		break;
	case OIS::KC_M:
		break;
	case OIS::KC_COMMA:
		break;
	case OIS::KC_PERIOD:
		break;
	case OIS::KC_SLASH:
		break;
	case OIS::KC_RSHIFT:
		break;
	case OIS::KC_MULTIPLY:
		break;
	case OIS::KC_LMENU:
		break;
	case OIS::KC_SPACE:
		break;
	case OIS::KC_CAPITAL:
		break;
	case OIS::KC_F1:
		break;
	case OIS::KC_F2:
		break;
	case OIS::KC_F3:
		break;
	case OIS::KC_F4:
		break;
	case OIS::KC_F5:
		break;
	case OIS::KC_F6:
		break;
	case OIS::KC_F7:
		break;
	case OIS::KC_F8:
		break;
	case OIS::KC_F9:
		break;
	case OIS::KC_F10:
		break;
	case OIS::KC_NUMLOCK:
		break;
	case OIS::KC_SCROLL:
		break;
	case OIS::KC_NUMPAD7:
		break;
	case OIS::KC_NUMPAD8:
		break;
	case OIS::KC_NUMPAD9:
		break;
	case OIS::KC_SUBTRACT:
		break;
	case OIS::KC_NUMPAD4:
		break;
	case OIS::KC_NUMPAD5:
		break;
	case OIS::KC_NUMPAD6:
		break;
	case OIS::KC_ADD:
		break;
	case OIS::KC_NUMPAD1:
		break;
	case OIS::KC_NUMPAD2:
		break;
	case OIS::KC_NUMPAD3:
		break;
	case OIS::KC_NUMPAD0:
		break;
	case OIS::KC_DECIMAL:
		break;
	case OIS::KC_OEM_102:
		break;
	case OIS::KC_F11:
		break;
	case OIS::KC_F12:
		break;
	case OIS::KC_F13:
		break;
	case OIS::KC_F14:
		break;
	case OIS::KC_F15:
		break;
	case OIS::KC_KANA:
		break;
	case OIS::KC_ABNT_C1:
		break;
	case OIS::KC_CONVERT:
		break;
	case OIS::KC_NOCONVERT:
		break;
	case OIS::KC_YEN:
		break;
	case OIS::KC_ABNT_C2:
		break;
	case OIS::KC_NUMPADEQUALS:
		break;
	case OIS::KC_PREVTRACK:
		break;
	case OIS::KC_AT:
		break;
	case OIS::KC_COLON:
		break;
	case OIS::KC_UNDERLINE:
		break;
	case OIS::KC_KANJI:
		break;
	case OIS::KC_STOP:
		break;
	case OIS::KC_AX:
		break;
	case OIS::KC_UNLABELED:
		break;
	case OIS::KC_NEXTTRACK:
		break;
	case OIS::KC_NUMPADENTER:
		break;
	case OIS::KC_RCONTROL:
		break;
	case OIS::KC_MUTE:
		break;
	case OIS::KC_CALCULATOR:
		break;
	case OIS::KC_PLAYPAUSE:
		break;
	case OIS::KC_MEDIASTOP:
		break;
	case OIS::KC_VOLUMEDOWN:
		break;
	case OIS::KC_VOLUMEUP:
		break;
	case OIS::KC_WEBHOME:
		break;
	case OIS::KC_NUMPADCOMMA:
		break;
	case OIS::KC_DIVIDE:
		break;
	case OIS::KC_SYSRQ:
		break;
	case OIS::KC_RMENU:
		break;
	case OIS::KC_PAUSE:
		break;
	case OIS::KC_HOME:
		break;
	case OIS::KC_UP:
		return TW_KEY_UP;
		break;
	case OIS::KC_PGUP:
		break;
	case OIS::KC_LEFT:
		return TW_KEY_LEFT;
		break;
	case OIS::KC_RIGHT:
		return TW_KEY_RIGHT;
		break;
	case OIS::KC_END:
		break;
	case OIS::KC_DOWN:
		return TW_KEY_DOWN;
		break;
	case OIS::KC_PGDOWN:
		break;
	case OIS::KC_INSERT:
		break;
	case OIS::KC_DELETE:
		break;
	case OIS::KC_LWIN:
		break;
	case OIS::KC_RWIN:
		break;
	case OIS::KC_APPS:
		break;
	case OIS::KC_POWER:
		break;
	case OIS::KC_SLEEP:
		break;
	case OIS::KC_WAKE:
		break;
	case OIS::KC_WEBSEARCH:
		break;
	case OIS::KC_WEBFAVORITES:
		break;
	case OIS::KC_WEBREFRESH:
		break;
	case OIS::KC_WEBSTOP:
		break;
	case OIS::KC_WEBFORWARD:
		break;
	case OIS::KC_WEBBACK:
		break;
	case OIS::KC_MYCOMPUTER:
		break;
	case OIS::KC_MAIL:
		break;
	case OIS::KC_MEDIASELECT:
		break;
	default:
		break;
	}

	return TW_KEY_RETURN;//return default key
}
