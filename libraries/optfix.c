#include <objc/runtime.h>
#include <objc/message.h>
#include <stddef.h>

__attribute__((visibility("default")))
Class objc_opt_class(id obj) {
    if (obj == NULL) return NULL;

    Class cls = object_getClass(obj);
    if (cls == NULL) return NULL;

    if (!objc_lookUpClass(class_getName(cls))) {
        return NULL;
    }

    if (class_isMetaClass(cls)) {
        return (Class)obj;
    }
    
    return cls;
}

__attribute__((visibility("default")))
BOOL objc_opt_isKindOfClass(id obj, Class cls) {
    if (obj == NULL || cls == NULL) return NO;
    
    Class objCls = object_getClass(obj);
    if (objCls == NULL || !objc_lookUpClass(class_getName(objCls))) {
        return NO;
    }
    
    SEL selector = sel_registerName("isKindOfClass:");
    typedef BOOL (*ObjCMsgSendFn)(id, SEL, Class);
    ObjCMsgSendFn sendMethod = (ObjCMsgSendFn)objc_msgSend;
    
    return sendMethod(obj, selector, cls);
}

__attribute__((visibility("default")))
id objc_opt_new(Class cls) {
    if (cls == NULL) return nil;
    
    SEL allocSel = sel_registerName("alloc");
    SEL initSel = sel_registerName("init");
    
    typedef id (*ObjCMsgSendAlloc)(Class, SEL);
    typedef id (*ObjCMsgSendInit)(id, SEL);
    
    ObjCMsgSendAlloc sendAlloc = (ObjCMsgSendAlloc)objc_msgSend;
    ObjCMsgSendInit sendInit = (ObjCMsgSendInit)objc_msgSend;
    
    id allocatedObj = sendAlloc(cls, allocSel);
    if (allocatedObj == nil) return nil;
    
    return sendInit(allocatedObj, initSel);
}

__attribute__((visibility("default")))
id objc_opt_self(id obj) {
    return obj;
}

__attribute__((visibility("default")))
BOOL objc_opt_respondToSelector(id obj, SEL sel) {
    if (obj == NULL || sel == NULL) return NO;
    return class_respondsToSelector(object_getClass(obj), sel);
}

__attribute__((visibility("default")))
id objc_opt_self(id obj) {
    return obj;
}

__attribute__((visibility("default")))
BOOL objc_opt_respondToSelector(id obj, SEL sel) {
    if (obj == NULL || sel == NULL) return NO;
    return class_respondsToSelector(object_getClass(obj), sel);
}
