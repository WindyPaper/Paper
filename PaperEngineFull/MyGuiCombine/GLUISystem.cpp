#include "Precompiled.h"
#include "GLUISystem.h"
#include <stdio.h>

GLUISystem::~GLUISystem()
{
	destroy();
}

bool GLUISystem::init(size_t winHandle)
{
	bool ret = create(winHandle);

	return ret;
}

void GLUISystem::destroy()
{
	BaseManager::destroy();
}

void GLUISystem::draw()
{
	renderUI();
}

void GLUISystem::setupResources()
{
	base::BaseManager::setupResources();
	addResourceLocation(getRootMedia() + "./Res/Media/CusUI");
	addResourceLocation(getRootMedia() + "./Res/Media/MyGUI_Media");
}

void GLUISystem::createScene()
{
	//MyGUI::LayoutManager::getInstance().loadLayout("Wallpaper.layout");
	//const MyGUI::VectorWidgetPtr& root = MyGUI::LayoutManager::getInstance().loadLayout("HelpPanel.layout");
	//root.at(0)->findWidget("Text")->castType<MyGUI::TextBox>()->setCaption("Write commands in console to change some widget parameters. For example \"colour 1 0 0 1\" changes text colour to red.");

	/*MyGUI::EditBox* pEdit = MyGUI::Gui::getInstance().createWidget<MyGUI::EditBox>("EditBoxStretch", MyGUI::IntCoord(10, 80, 100, 100), MyGUI::Align::Default, "Overlapped");
	pEdit->setCaption("some edit");
	pEdit->setTextAlign(MyGUI::Align::Center);
	pEdit->setEditMultiLine(true);*/
	mRoot = MyGUI::LayoutManager::getInstance().loadLayout("DebugLayout.layout");
}

void GLUISystem::setProfileMsg(const IProfile *pProfile)
{
	char profileText[256];
	memset(profileText, 0, 256);
	sprintf_s(profileText, 256, "FPS:%d\nVerticeNum:%d\nDrawCallNum:%d\n", pProfile->getFPS(), pProfile->getVerticeNum(), pProfile->getDrawCallNum());
	mRoot.at(0)->findWidget("debugLab")->castType<MyGUI::TextBox>()->setCaption(profileText);
}
