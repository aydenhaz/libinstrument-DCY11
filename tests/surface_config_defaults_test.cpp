#include <NS_MultiScreenBase/AutoConfigJsonData.h>
#include <cassert>
#include <cstring>
#include <new>
static int constructed,destroyed;
CGString::CGString(){++constructed;}
CGString::~CGString(){++destroyed;}
int main(){
 using NS_MultiScreenBase::SurfaceConfig;
 alignas(SurfaceConfig) unsigned char bytes[sizeof(SurfaceConfig)];std::memset(bytes,0xa5,sizeof(bytes));
 auto* config=new(bytes) SurfaceConfig;
 assert(constructed==1&&destroyed==0);
 for(int i=0;i<7;++i)assert(config->scalar[i]==0);
 assert(config->scalar[7]==0xffffffffu&&config->scalar[8]==8);
 for(int i=0;i<3;++i)assert(config->tail[i]==0);
 assert(config->tail[3]==1&&config->final_flag==0);
 config->~SurfaceConfig();assert(destroyed==1);
}
