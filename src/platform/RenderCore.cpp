#include "RenderCore.h"
#include "Program.h"
#include "Utils.h"
#include "fgl/Global.h"

void RenderCore::initialize()
{
	int max[3] = {0, 0, 0};
	glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, max);
	checkGLErrors("gg");
	
    /*program.create("uniform float roll;\
		 layout(binding = 0) uniform image2D destTex;\
		 layout (local_size_x = 16, local_size_y = 16) in;\
		 void main() {\
			 ivec2 storePos = ivec2(gl_GlobalInvocationID.xy);\
			 float value = (( int(roll*256) + storePos.x + storePos.y) % 256) / 255.0;\n\
			 //float localCoef = length(vec2(ivec2(gl_LocalInvocationID.xy)-8)*0.125);\n\
			 //float globalCoef = sin(float(gl_WorkGroupID.x+gl_WorkGroupID.y)*0.1 + roll)*0.5;\n\
			 //imageStore(destTex, storePos, vec4(1.0-globalCoef*localCoef, 0.0, 0.0, 1.0));\n\
imageStore(destTex, storePos, vec4(value, 0.0, 0.0, 1.0));\n\
		 }");*/


	Global::Instance().Initialize();
}