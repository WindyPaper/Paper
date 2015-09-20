#ifndef _GLOBAL_SHADER_PARAM_MNG_H_
#define _GLOBAL_SHADER_PARAM_MNG_H_

#include <string>
#include <vector>
#include <assert.h>
#include "util/Paper_Math.h"
#include "util/Float4.h"
#include "util/Singleton.h"
#include "ResourceSystem/ShaderParam.h"

class Renderable;

class GlobalShaderParamMng : public Singleton<GlobalShaderParamMng>
{
public:
	static GlobalShaderParamMng &getInstance();
	static GlobalShaderParamMng *getInstancePtr();

	typedef std::vector<ShaderParam> GlobalShaderParamVec;

	enum MatrixParamIndex
	{
		MatrixBegin = 0,

		MatrixMVP = MatrixBegin,
		MatrixModel,
		MatrixView,
		MatrixModelView,
		MatrixInverseModel,
		MatrixInverseView,
		MatrixProj,
		MatrixInverseProj,

		MatrixProjector,
		MatrixInverseTransposeModel,

		MatrixLightView,
		MatrixLightProj0,
		MatrixLightProj1,
		MatrixLightProj2,

		MatrixEnd,
	};

	enum VectorParamIndex
	{
		VectorBegin = MatrixEnd,

		VecAmbientColor = VectorBegin,
		VecCameraPos,
		VecCameraParam,
		VecScreenSize,
		VecShadowMapSize,
		VecSoftShadowParam,
		VecShadowColor,
		VecDebugCameraColor,
		VecFogColor,
		VecFogParam,
		VecTime,

		VecLightDir0,
		VecLightDiffuse0,
		VecLightPos0,
		VecLightAtten0,

		VecLightDir1,
		VecLightDiffuse1,
		VecLightPos1,
		VecLightAtten1,

		VecLightDir2,
		VecLightDiffuse2,
		VecLightPos2,
		VecLightAtten2,

		VecLightDir3,
		VecLightDiffuse3,
		VecLightPos3,
		VecLightAtten3,

		VecLightCameraNear,
		VecLightCameraFar,
		VecLightCameraFrustumIntervalEnd,

		DefaultLight,
		ParticleMeshColor,
		ParticleTime,
		ParticleForce,
		ParticleGravity,
		ParticleMovement,
		ParticleColor,
		ParticleTexAnim,
		ParticleUpVec,
		ParticleRightVec,

		VectorEnd,
	};

	enum TexParamIndex
	{
		TexBegin = VectorEnd,

		TexMainBuffer = TexBegin,
		TexDepthMap,
		TexLightLitMap,

		TexShadowMap0,
		TexShadowMap1,
		TexShadowMap2,
		TexShadowMap3,
		TexShadowMap4,
		TexShadowMap5,
		TexShadowMap6,
		TexShadowMap7,

		TexSwapBuffer,
		TexProjectorMap,

		TexCustom0,
		TexCustom1,
		TexCustom2,
		TexCustom3,

		//TexDfNormalMap,
		//TexDfDepthMap,
		//TexDfParamMap,
		TexEnd,
	};

	enum ShaderParamCount
	{
		GlobalShaderParamCount = TexEnd,
	};

	GlobalShaderParamMng();
	~GlobalShaderParamMng();

	void initGlobalParam();

	const math::Matrix44 &getMatrixParam(const MatrixParamIndex index) const;
	//math::Matrix44 &getMatrixParam(const MatrixParamIndex index);
	const GlobalShaderParamVec &getGlobalShaderParamList() const;
	void setMatrixParam(const MatrixParamIndex index, const math::Matrix44 &matrix);

	void setCurrRenderable(Renderable *pRenderable);
	
private:
	GlobalShaderParamVec mShaderParams;
	Renderable *mpCurrRenderable;
};

#endif