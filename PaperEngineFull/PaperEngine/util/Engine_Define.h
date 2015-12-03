#ifndef _ENGINE_DEFINE_H_
#define _ENGINE_DEFINE_H_

#define SAFE_DELETE(p) if(p) { delete p; p = 0; }
#define SAFE_DELETE_ARRAY(p) if(p) { delete[] p; p = 0; }

typedef unsigned int uint;
typedef unsigned int uint32;

#define EINT64 __int64

#define  MAX_TEX_UNIT 8

#define DEFAULT_IMAGE_PATH "res/defaultTex.png"

#define MAX_LOG_CHAR 1024

#define BIT(x) (1<<(x))

//此宏展开后，类似于printf("%d%d", 1, 2);
#define MSG_LOG(fmt, ...) \
	printf("%s(%d)-<%s>: "##fmt, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)

//xna math
//#define _XM_NO_INTRINSICS_

#define MAX_INSTANCE_RENDER_NUM 128

#define MAX_INSTANCE_NUM 2048

#define MAX_RES_PATH 512

#endif