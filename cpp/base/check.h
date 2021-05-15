#ifndef BASE_CHECK_H
#define BASE_CHECK_H

#define CHECK(x) if(x)printf("Check failed at %s:%d\n",__FILE__,__LINE__);
#define DCHECK(x) if(x)printf("Check failed at %s:%d\n",__FILE__,__LINE__);

#endif