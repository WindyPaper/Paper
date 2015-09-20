#include "util/PrecompileHead.h"
#include "util/GlobalShaderParamMng.h"
#include "GameObject/IGameObjComponent.h"
#include "GameObject/Renderable.h"
#include "Scene/Node.h"

template<> GlobalShaderParamMng *Singleton<GlobalShaderParamMng>::msInstance = 0;

GlobalShaderParamMng *GlobalShaderParamMng::getInstancePtr()
{
	return msInstance;
}

GlobalShaderParamMng &GlobalShaderParamMng::getInstance()
{
	assert(msInstance);
	return (*msInstance);
}

GlobalShaderParamMng::GlobalShaderParamMng() :
mpCurrRenderable(0)
{
	mShaderParams.resize(GlobalShaderParamCount);

	initGlobalParam();
}

GlobalShaderParamMng::~GlobalShaderParamMng()
{

}

void GlobalShaderParamMng::initGlobalParam()
{
	mShaderParams[MatrixView] = ShaderParam("g_View", math::Matrix44());
	mShaderParams[MatrixProj] = ShaderParam("g_Proj", math::Matrix44());
	mShaderParams[MatrixModel] = ShaderParam("g_Model", math::Matrix44());
	mShaderParams[MatrixMVP] = ShaderParam("g_MVP", math::Matrix44());
}

const math::Matrix44 & GlobalShaderParamMng::getMatrixParam(const MatrixParamIndex index) const
{
	assert(index < GlobalShaderParamCount);

	if (index == MatrixModel)
	{
		if (mpCurrRenderable)
		{
			IGameObjRenderComp *pRenderComp = mpCurrRenderable->getParentComp();
			Node *pNode = pRenderComp->getOwnNode();
			math::Matrix44 *matrix = reinterpret_cast<math::Matrix44*>(mShaderParams[index].pValue);
			matrix->initFullTransform(pNode->getWorldPosition(), pNode->getWorldScale(), pNode->getWorldOrientation());
			return *matrix;
		}
	}

	const math::Matrix44 *matrix = reinterpret_cast<math::Matrix44*>(mShaderParams[index].pValue);
	return *matrix;
}

/*
math::Matrix44 & GlobalShaderParamMng::getMatrixParam(const MatrixParamIndex index)
{
	assert(index < GlobalShaderParamCount);
	return *mShaderParams[index].value.pMatrix44;
}*/

const GlobalShaderParamMng::GlobalShaderParamVec & GlobalShaderParamMng::getGlobalShaderParamList() const
{
	return mShaderParams;
}

void GlobalShaderParamMng::setMatrixParam(const MatrixParamIndex index, const math::Matrix44 &matrix)
{
	assert(index >= MatrixBegin && index < MatrixEnd);
	//*mShaderParams[index].value.pMatrix44 = matrix;
	mShaderParams[index].setValue(matrix);
}


void GlobalShaderParamMng::setCurrRenderable(Renderable *pRenderable)
{
	mpCurrRenderable = pRenderable;
}
