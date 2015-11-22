#ifndef _BOUND_INS_MESH_H_
#define _BOUND_INS_MESH_H_

#include <vector>
#include "util/Paper_Math.h"
#include "GameObject/GameObject.h"

class IMesh;

class BoundInsMesh : public GameObject
{
public:
	BoundInsMesh();
	BoundInsMesh(const std::string &name);
	~BoundInsMesh();

	void addTransformMatrix(const math::Matrix44 matrix);

	void initCubeMesh();
	void initCustomMesh(const std::string &meshName, const unsigned char *pVertexData, int vertexNum, const unsigned char *pIndexData, int indexNum);

	IMesh *getMesh() { return mpMesh; }

private:
	std::vector<math::Matrix44*> mMatrixVec;

	float *mpCubeBoxVData;
	unsigned int *mpCubeBoxIData;
	IMesh *mpMesh;
};

#endif