#include <objc/runtime.h>
#include <objc/message.h>
#include <stddef.h>

__attribute__((visibility("default")))
Class objc_opt_class(id obj) {
    if (obj == NULL) return NULL;
    if (class_isMetaClass(object_getClass(obj))) {
        return (Class)obj;
    }
    return object_getClass(obj);
}

__attribute__((visibility("default")))
BOOL objc_opt_isKindOfClass(id obj, Class cls) {
    if (obj == NULL || cls == NULL) return 0;
    
    SEL selector = sel_registerName("isKindOfClass:");
    typedef BOOL (*ObjCMsgSendFn)(id, SEL, Class);
    ObjCMsgSendFn sendMethod = (ObjCMsgSendFn)objc_msgSend;
    
    return sendMethod(obj, selector, cls);
}