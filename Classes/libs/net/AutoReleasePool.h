//
//  AutoReleasePool.h
//  Coc
//
//  Created by Hector on 13-10-27.
//
//

// 调用IOS的自动化释放池
class AutoReleasePool
{
private:
	void * pool;
public:
	AutoReleasePool();
	~AutoReleasePool();
};