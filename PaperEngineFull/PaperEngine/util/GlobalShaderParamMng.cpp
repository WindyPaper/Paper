#include "util/PrecompileHead.h"
#include "util/GlobalShaderParamMng.h"
#include "GameObject/IGameObjComponent.h"
#include "GameObject/Renderable.h"
#include "Scene/Node.h"
#include "util/Camera.h"

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

	math::Vector3f dir = math::Vector3f(0.66, -0.66, 0.33);
	dir.normalize();
	_initGlobalLightDir(dir);
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

void GlobalShaderParamMng::_initGlobalLightDir(const math::Vector3f &dir)
{
	math::Vector3f pos = math::Vector3f(-1382, 3331, -596);
	math::Vector3f up = math::Vector3f(0.59, 0.74, 0.30);
	

	up.normalize();
	Frustum generateProjMat;

	math::OrthographicProjParam param;
	param.width = (int)(param.width/2);
	param.height = (int)(param.height/2);
	math::Matrix44 orthoMat;
	orthoMat.initOrthographicProj(param);

	math::Matrix44 viewMat;
	viewMat.m = XMMatrixLookToRH(pos, dir, up);

	mShaderParams[MatrixLightView] = ShaderParam("light_view_mat", viewMat);
	mShaderParams[MatrixLightProj] = ShaderParam("light_proj_mat", generateProjMat.getProjMatrix());
}
