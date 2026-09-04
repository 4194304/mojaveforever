// compile this with that command below, you will likely forget
// clang -dynamiclib -o ~/Documents/optfix/optfix.dylib ~/Documents/optfix/optfix.c -framework Foundation -install_name @executable_path/optfix.dylib

#include <objc/runtime.h>
#include <objc/message.h>
#include <stddef.h>
#include <dlfcn.h>
#include <stdio.h>
#include <mach/port.h>
#include <CoreFoundation/CoreFoundation.h>
#include <mach-o/loader.h>
#include <mach-o/dyld.h>
#include <string.h>
#include <sys/mman.h>

// load extra libraries
__attribute__((constructor))
static void load_libraries(void) {
    // add whatever you need here, here are some examples:
    // "/opt/local/lib/libcxx/libc++.1.dylib" would be to add macports libc++ (using absolute paths)
    // as for @executable_path, if you have something relative to the executable itself then use that
    const char *libraries[] = {
        "/opt/local/lib/libcxx/libc++.1.dylib"
    };
        
    size_t total_libs = sizeof(libraries) / sizeof(libraries[0]);
    
    for (size_t i = 0; i < total_libs; i++) {
        const char *target_library = libraries[i];
        void *handle = dlopen(target_library, RTLD_NOW | RTLD_GLOBAL);
        
        if (!handle) {
            fprintf(stderr, "ERROR: Failed to load %s: %s\n", 
                    target_library, dlerror());
        }
    }
}

// objective C runtime fixes

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

// metal metal metal metal

__attribute__((visibility("default")))
void * OBJC_CLASS_$_CAEDRMetadata;

__attribute__((constructor))
static void build_fallback_classes(void) {
    Class existingClass = objc_lookUpClass("CAEDRMetadata");
    if (existingClass != NULL) {
        OBJC_CLASS_$_CAEDRMetadata = (void *)existingClass;
        return;
    }
    
    Class baseNSObjectClass = objc_lookUpClass("NSObject");
    if (baseNSObjectClass != NULL) {
        Class dummyClass = objc_allocateClassPair(baseNSObjectClass, "CAEDRMetadata", 0);
        if (dummyClass != NULL) {

            objc_registerClassPair(dummyClass);
            OBJC_CLASS_$_CAEDRMetadata = (void *)dummyClass;
        }
    }
}

__attribute__((visibility("default")))
const char * MTLCommandBufferEncoderInfoErrorKey = "MTLCommandBufferEncoderInfoErrorKey";

__attribute__((visibility("default")))
const char * MTLCommonCounterSetTimestamp = "MTLCommonCounterSetTimestamp";

__attribute__((visibility("default")))
const char * MTLCommonCounterTimestamp = "MTLCommonCounterTimestamp";

__attribute__((visibility("default"))) void * OBJC_CLASS_$_MTLCommandBufferDescriptor;

__attribute__((visibility("default"))) void * OBJC_CLASS_$_MTLBlitPassDescriptor;

__attribute__((visibility("default"))) void * OBJC_CLASS_$_MTLCaptureDescriptor;

__attribute__((visibility("default"))) void * OBJC_CLASS_$_MTLCounterSampleBufferDescriptor;

__attribute__((constructor))
static void metalfallbacks(void) {

    Class existingCounterDescClass = objc_lookUpClass("MTLCounterSampleBufferDescriptor");
    if (existingCounterDescClass != NULL) {
        OBJC_CLASS_$_MTLCounterSampleBufferDescriptor = (void *)existingCounterDescClass;
    } else {
    Class baseNSObjectClass = objc_lookUpClass("NSObject");
    if (baseNSObjectClass != NULL) {
        Class dummyClass = objc_allocateClassPair(baseNSObjectClass, "MTLCounterSampleBufferDescriptor", 0);
        if (dummyClass != NULL) {
            objc_registerClassPair(dummyClass);
            OBJC_CLASS_$_MTLCounterSampleBufferDescriptor = (void *)dummyClass;
            }
        }
    }
    
    Class existingCmdBufClass = objc_lookUpClass("MTLCommandBufferDescriptor");
    if (existingCmdBufClass != NULL) {
        OBJC_CLASS_$_MTLCommandBufferDescriptor = (void *)existingCmdBufClass;
    } else {
    Class baseNSObjectClass = objc_lookUpClass("NSObject");
    if (baseNSObjectClass != NULL) {
        Class dummyClass = objc_allocateClassPair(baseNSObjectClass, "MTLCommandBufferDescriptor", 0);
        if (dummyClass != NULL) {
            objc_registerClassPair(dummyClass);
            OBJC_CLASS_$_MTLCommandBufferDescriptor = (void *)dummyClass;
            }
        }
    }
    
    Class existingCaptureClass = objc_lookUpClass("MTLCaptureDescriptor");
    if (existingCaptureClass != NULL) {
       OBJC_CLASS_$_MTLCaptureDescriptor = (void *)existingCaptureClass;
    } else {
       Class baseNSObjectClass = objc_lookUpClass("NSObject");
       if (baseNSObjectClass != NULL) {
          Class dummyClass = objc_allocateClassPair(baseNSObjectClass, "MTLCaptureDescriptor", 0);
          if (dummyClass != NULL) {
                objc_registerClassPair(dummyClass);
               OBJC_CLASS_$_MTLCaptureDescriptor = (void *)dummyClass;
            }
        }
    }
    
    Class existingBlitClass = objc_lookUpClass("MTLBlitPassDescriptor");
    if (existingBlitClass != NULL) {
        OBJC_CLASS_$_MTLBlitPassDescriptor = (void *)existingBlitClass;
    } else {
      Class baseNSObjectClass = objc_lookUpClass("NSObject");
      if (baseNSObjectClass != NULL) {
            Class dummyClass = objc_allocateClassPair(baseNSObjectClass, "MTLBlitPassDescriptor", 0);
            if (dummyClass != NULL) {
                objc_registerClassPair(dummyClass);
                OBJC_CLASS_$_MTLBlitPassDescriptor = (void *)dummyClass;
            }
        }
    }
}

__attribute__((visibility("default")))
const CFBooleanRef __kCFBooleanTrue;

__attribute__((constructor))
static void fix_triple_underscore_boolean(void) {
    CFBooleanRef *real_system_bool = (CFBooleanRef *)dlsym(RTLD_DEFAULT, "kCFBooleanTrue");
    if (real_system_bool) {
        *(CFBooleanRef *)&__kCFBooleanTrue = *real_system_bool;
    }
}

__attribute__((visibility("default")))
const char * kCGColorSpaceITUR_2100_HLG = "kCGColorSpaceITUR_2100_HLG";

__attribute__((visibility("default")))
const char * kCGColorSpaceITUR_2100_PQ = "kCGColorSpaceITUR_2100_PQ";

// Qt6 attempt 1

extern const mach_port_t kIOMasterPortDefault;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-variable-declarations"
const mach_port_t kIOMainPortDefault = 0;
#pragma clang diagnostic pop

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-variable-declarations"
const void * UTTypeBundle = (const void *)CFSTR("com.apple.bundle");
#pragma clang diagnostic pop

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-variable-declarations"

const void * UTTypeTIFF = (const void *)CFSTR("public.tiff");
const void * UTTypePNG  = (const void *)CFSTR("public.png");
const void * UTTypeJPEG = (const void *)CFSTR("public.jpeg");

#pragma clang diagnostic pop

// macports libc++ is not perfect I guess

__attribute__((visibility("default")))
void* dummy_stringbuf_hook(void* hidden_ret_ptr, void* this_ptr) __asm__("__ZNKSt3__115basic_stringbufIcNS_11char_traitsIcEENS_9allocatorIcEEE3strEv");

void* dummy_stringbuf_hook(void* hidden_ret_ptr, void* this_ptr) {
    unsigned char* string_memory = (unsigned char*)hidden_ret_ptr;
    for (int i = 0; i < 24; i++) {
        string_memory[i] = 0;
    }
    return hidden_ret_ptr;
}

__attribute__((visibility("default")))
const void* dummy_stringstream_vtt[8] __asm__("__ZTTNSt3__118basic_stringstreamIcNS_11char_traitsIcEENS_9allocatorIcEEEE") __attribute__((aligned(8))) = {
    0, 0, 0, 0, 0, 0, 0, 0
};

static void dummy_vtable_method(void) {
}

__attribute__((visibility("default")))
const void* dummy_stringbuf_vtable[] __asm__("__ZTVNSt3__115basic_stringbufIcNS_11char_traitsIcEENS_9allocatorIcEEEE") __attribute__((aligned(8))) = {
    (void*)0, (void*)0, 
    &dummy_vtable_method, &dummy_vtable_method, &dummy_vtable_method, &dummy_vtable_method,
    &dummy_vtable_method, &dummy_vtable_method, &dummy_vtable_method, &dummy_vtable_method,
    &dummy_vtable_method, &dummy_vtable_method, &dummy_vtable_method, &dummy_vtable_method,
    &dummy_vtable_method, &dummy_vtable_method, &dummy_vtable_method, &dummy_vtable_method
};

__attribute__((visibility("default")))
const void* dummy_stringstream_vtable[] __asm__("__ZTVNSt3__118basic_stringstreamIcNS_11char_traitsIcEENS_9allocatorIcEEEE") __attribute__((aligned(8))) = {
    (void*)0, (void*)0, 
    &dummy_vtable_method, &dummy_vtable_method, &dummy_vtable_method, &dummy_vtable_method,
    &dummy_vtable_method, &dummy_vtable_method, &dummy_vtable_method, &dummy_vtable_method,
    &dummy_vtable_method, &dummy_vtable_method, &dummy_vtable_method, &dummy_vtable_method,
    &dummy_vtable_method, &dummy_vtable_method, &dummy_vtable_method, &dummy_vtable_method
};

// Electron 32

__attribute__((visibility("default")))
const void *AVCaptureDeviceTypeExternalUnknown = (const void *)CFSTR("AVCaptureDeviceTypeExternalUnknown");

__attribute__((visibility("default")))
const void *AVCaptureDeviceTypeBuiltInWideAngleCamera = (const void *)CFSTR("AVCaptureDeviceTypeBuiltInWideAngleCamera");

extern void *__CFConstantStringClassReference;

typedef struct {
    void *isa;
    uintptr_t info;
    const char *data;
    long len;
} shim_cfstring_t;

__attribute__((visibility("default")))
const shim_cfstring_t kCMFormatDescriptionTransferFunction_sRGB = {
    .isa  = &__CFConstantStringClassReference,
    .info = 0x7C2,
    .data = "sRGB",
    .len  = 4
};

__attribute__((visibility("default"))) 
const struct { void *isa; uintptr_t info; const char *data; long len; } ASAuthorizationErrorDomain = { 
    &__CFConstantStringClassReference, 0x7C2, 
    "com.apple.AuthenticationServices.AuthorizationError", 51 
};

typedef struct {
    void *isa;
    void *superclass;
    void *cache;
    void *vtable;
    uintptr_t data;
} shim_objc_class_t;

static const uintptr_t dummy_objc_class_ro = 0x2; 

#define DEFINE_OBJC_CLASS_STUB(name) \
__attribute__((visibility("default"))) \
shim_objc_class_t OBJC_METACLASS_$_##name = { \
    .isa = &OBJC_METACLASS_$_##name, \
    .superclass = NULL, \
    .data = (uintptr_t)&dummy_objc_class_ro \
}; \
__attribute__((visibility("default"))) \
const shim_objc_class_t OBJC_CLASS_$_##name = { \
    .isa = &OBJC_METACLASS_$_##name, \
    .superclass = NULL, \
    .data = (uintptr_t)&dummy_objc_class_ro \
}

DEFINE_OBJC_CLASS_STUB(AVCaptureDeviceDiscoverySession);
DEFINE_OBJC_CLASS_STUB(AVCapturePhotoOutput);
DEFINE_OBJC_CLASS_STUB(AVCapturePhotoSettings);
DEFINE_OBJC_CLASS_STUB(NSColorSampler);
DEFINE_OBJC_CLASS_STUB(ASAuthorizationController);

static double shim_maximumPotentialExtendedDynamicRangeColorComponentValue(void *self, void *_cmd) {
    return 1.0;
}

static void *shim_localizedName(void *self, void *_cmd) {
    extern void *__CFConstantStringClassReference;
    static const struct { void *isa; uintptr_t info; const char *data; long len; } mock_name_string = {
        &__CFConstantStringClassReference, 0x7C2, "Display", 7
    };
    return (void *)&mock_name_string;
}

__attribute__((constructor))
static void inject_missing_appkit_selectors(void) {
    void *nsscreen_class = objc_getClass("NSScreen");
    
    if (nsscreen_class) {
        void *hdr_sel = sel_registerName("maximumPotentialExtendedDynamicRangeColorComponentValue");
        class_addMethod((Class)nsscreen_class, (SEL)hdr_sel, (IMP)shim_maximumPotentialExtendedDynamicRangeColorComponentValue, "d@:");
        
        void *name_sel = sel_registerName("localizedName");
        class_addMethod((Class)nsscreen_class, (SEL)name_sel, (IMP)shim_localizedName, "@@:");
    }
}

// Chromium 128

#define kLAErrorBiometryNotAvailable -6

__attribute__((visibility("default")))
const void *kSecUseDataProtectionKeychain = (const void *)CFSTR("kSecUseDataProtectionKeychain");

__attribute__((visibility("default")))
const void *ASWebAuthenticationSessionErrorDomain = (const void *)CFSTR("ASWebAuthenticationSessionErrorDomain");

__attribute__((visibility("default")))
const void *_kLSOpenOptionBackgroundLaunchKey = (const void *)CFSTR("_kLSOpenOptionBackgroundLaunchKey");

__attribute__((visibility("default")))
void *AsWebAuthManagerSymbol __asm__("_OBJC_CLASS_$_ASWebAuthenticationSessionWebBrowserSessionManager") = NULL;

__attribute__((visibility("default")))
void *SfUniversalLinkSymbol __asm__("_OBJC_CLASS_$_SFUniversalLink") = NULL;

__attribute__((visibility("default")))
void *NsWorkspaceConfigSymbol __asm__("_OBJC_CLASS_$_NSWorkspaceOpenConfiguration") = NULL;

static double fallback_maximumPotentialEDR(id self, SEL _cmd) {
    return 1.0; 
}

static id fallback_localizedName(id self, SEL _cmd) {
    return (id)CFSTR("Display");
}

__attribute__((constructor))
static void patch_mojave_ns_screen(void) {
    void *appkit_handle = dlopen("/System/Library/Frameworks/AppKit.framework/AppKit", RTLD_LAZY | RTLD_GLOBAL);
    if (!appkit_handle) return;

    Class NSScreen_class = objc_getClass("NSScreen");
    if (NSScreen_class) {
        SEL edr_sel = sel_getUid("maximumPotentialExtendedDynamicRangeColorComponentValue");
        if (!class_respondsToSelector(NSScreen_class, edr_sel)) {
            class_addMethod(NSScreen_class, edr_sel, (IMP)fallback_maximumPotentialEDR, "d@:");
        }

        SEL name_sel = sel_getUid("localizedName");
        if (!class_respondsToSelector(NSScreen_class, name_sel)) {
            class_addMethod(NSScreen_class, name_sel, (IMP)fallback_localizedName, "@@:");
        }
    }
}

static id fallback_sharedManager(id self, SEL _cmd) {
    Class NSObject_class = objc_getClass("NSObject");
    id alloced = ((id (*)(Class, SEL))objc_msgSend)(NSObject_class, sel_getUid("alloc"));
    return ((id (*)(id, SEL))objc_msgSend)(alloced, sel_getUid("init"));
}

static id fallback_methodSignature(id self, SEL _cmd, SEL aSelector) {
    Class NSMethodSignature_class = objc_getClass("NSMethodSignature");
    return ((id (*)(Class, SEL, const char *))objc_msgSend)(NSMethodSignature_class, sel_getUid("signatureWithObjCTypes:"), "v@:@");
}

static void fallback_forwardInvocation(id self, SEL _cmd, id anInvocation) {
}

__attribute__((constructor))
__attribute__((constructor))
__attribute__((constructor))
static void init_mojave_dynamic_classes(void) {
    Class NSObject_class = objc_getClass("NSObject");

    Class auth_class = objc_allocateClassPair(NSObject_class, "ASWebAuthenticationSessionWebBrowserSessionManager", 0);
    if (auth_class) {
        class_addMethod(object_getClass((id)auth_class), sel_getUid("sharedManager"), (IMP)fallback_sharedManager, "@@:");
        objc_registerClassPair(auth_class);
        AsWebAuthManagerSymbol = (void *)auth_class;
    }

    Class link_class = objc_allocateClassPair(NSObject_class, "SFUniversalLink", 0);
    if (link_class) {
        class_addMethod(link_class, sel_getUid("methodSignatureForSelector:"), (IMP)fallback_methodSignature, "@@::");
        class_addMethod(link_class, sel_getUid("forwardInvocation:"), (IMP)fallback_forwardInvocation, "v@:@");
        class_addMethod(object_getClass((id)link_class), sel_getUid("methodSignatureForSelector:"), (IMP)fallback_methodSignature, "@@::");
        class_addMethod(object_getClass((id)link_class), sel_getUid("forwardInvocation:"), (IMP)fallback_forwardInvocation, "v@:@");

        objc_registerClassPair(link_class);
        SfUniversalLinkSymbol = (void *)link_class;
    }

    Class config_class = objc_allocateClassPair(NSObject_class, "NSWorkspaceOpenConfiguration", 0);
    if (config_class) {
        class_addMethod(config_class, sel_getUid("methodSignatureForSelector:"), (IMP)fallback_methodSignature, "@@::");
        class_addMethod(config_class, sel_getUid("forwardInvocation:"), (IMP)fallback_forwardInvocation, "v@:@");
        class_addMethod(object_getClass((id)config_class), sel_getUid("methodSignatureForSelector:"), (IMP)fallback_methodSignature, "@@::");
        class_addMethod(object_getClass((id)config_class), sel_getUid("forwardInvocation:"), (IMP)fallback_forwardInvocation, "v@:@");

        objc_registerClassPair(config_class);
        NsWorkspaceConfigSymbol = (void *)config_class;
    }
}


static BOOL fallback_canEvaluatePolicy(id self, SEL _cmd, long policy, id *error) {
    if (policy > 2) {
        if (error) {
            Class NSError_class = objc_getClass("NSError");
            SEL err_sel = sel_getUid("errorWithDomain:code:userInfo:");
            *error = ((id (*)(Class, SEL, id, long, id))objc_msgSend)(
                NSError_class, err_sel, (id)CFSTR("com.apple.LocalAuthentication"), kLAErrorBiometryNotAvailable, NULL
            );
        }
        return NO;
    }

    SEL orig_sel = sel_getUid("mojave_orig_canEvaluatePolicy:error:");
    typedef BOOL (*orig_impl_t)(id, SEL, long, id *);
    return ((orig_impl_t)class_getMethodImplementation(objc_getClass("LAContext"), orig_sel))(self, _cmd, policy, error);
}

__attribute__((constructor))
static void patch_mojave_local_authentication(void) {
    void *la_handle = dlopen("/System/Library/Frameworks/LocalAuthentication.framework/LocalAuthentication", RTLD_LAZY | RTLD_GLOBAL);
    if (!la_handle) return;

    Class LAContext_class = objc_getClass("LAContext");
    if (LAContext_class) {
        Method orig_canEval = class_getInstanceMethod(LAContext_class, sel_getUid("canEvaluatePolicy:error:"));
        if (orig_canEval) {
            class_addMethod(LAContext_class, sel_getUid("mojave_orig_canEvaluatePolicy:error:"), method_getImplementation(orig_canEval), method_getTypeEncoding(orig_canEval));
            
            method_setImplementation(orig_canEval, (IMP)fallback_canEvaluatePolicy);
        }
    }
}

__attribute__((visibility("default")))
void _CSCheckFixDisable(void) {
    void (*real_CSCheckFixDisable)(void) = NULL;

    void* handle = dlopen("/System/Library/Frameworks/CoreServices.framework/CoreServices", RTLD_LAZY);
    
    if (handle) {
        real_CSCheckFixDisable = (void (*)(void))dlsym(handle, "_CSCheckFixDisable");
        if (real_CSCheckFixDisable) {
            real_CSCheckFixDisable();
            dlclose(handle);
            return;
        }
        dlclose(handle);
    }
} 