
#ifndef MACRO_H_
#define MACRO_H_

namespace uc {

#define SAFE_DELETE(ptr) if( (ptr) != NULL ) { delete (ptr); (ptr) = NULL; }

#define SAFE_DELETE_CONTAINER(ContainerType, ptr)					\
    if (ptr != NULL) {												\
        ContainerType::iterator iter = (ptr)->begin();				\
        while (iter != (ptr)->end()) {								\
            if (*iter != NULL) {									\
                delete (*iter);										\
            }														\
            iter = (ptr)->erase(iter);								\
        }															\
            														\
        delete (ptr);												\
        (ptr) = NULL;												\
    }

#define SAFE_CLEAR_CONTAINER(ContainerType, container)				\
	{																\
		ContainerType::iterator iter = (container).begin();			\
		while (iter != (container).end()) {							\
			if (*iter != NULL) {									\
				delete (*iter);										\
			}														\
			iter = (container).erase(iter);							\
		}															\
	}


}

#endif