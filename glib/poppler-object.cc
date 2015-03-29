#include <glib.h>
#include "poppler-object.h"
#include <Annot.h>


#define DEFINE_POPPLER_OBJECT(...)
#define CONSTRUCTOR_MONAD(method) \
    CAT(Poppler,Klass) * CAT(CAT(CAT(poppler_, klass), _), method)() { \
        return (CAT(Poppler,Klass)*) new Klass(); \
    }
#define CONSTRUCTOR_ONEP(method, type1) \
    CAT(Poppler,Klass) * CAT(CAT(CAT(poppler_, klass), _), method)(type1 arg1) { \
        return (CAT(Poppler,Klass)*) new Klass(arg1); \
    }
#define CONSTRUCTOR_ONEF(method, Type1, type1) \
    CAT(Poppler,Klass) * CAT(CAT(CAT(poppler_, klass), _), method)(type1 arg1) { \
        return (CAT(Poppler,Klass)*) new Klass((Type1)arg1); \
    }
#define CONSTRUCTOR_XREF() CONSTRUCTOR_ONEF(new, XRef*, gpointer)
#define DESTRUCTOR(method) \
    void  CAT(CAT(CAT(poppler_, klass), _), method) (CAT(Poppler,Klass) *arg0) { \
        delete ( (Klass*) arg0 ); \
    }
#define METHOD_MONAD( return_type, Method, method) \
    return_type  CAT(CAT(CAT(poppler_, klass), _), method) (CAT(Poppler,Klass) *arg0) { \
        return (return_type) ( (Klass*) arg0 )->Method(); \
    }

#define METHOD_ONEF( return_type, Method, method, Type1, type1) \
    return_type  CAT(CAT(CAT(poppler_, klass), _), method) (CAT(Poppler,Klass) *arg0, Type1 arg1) { \
        return (return_type) ( (Klass*) arg0 )->Method((type1)arg1); \
    }

#define METHOD_ONEP( return_type, Method, method, type1) \
    return_type  CAT(CAT(CAT(poppler_, klass), _), method) (CAT(Poppler,Klass) *arg0, type1 arg1) { \
        return (return_type) ( (Klass*) arg0 )->Method(arg1); \
    }
#define METHOD_TWOPF( return_type, Method, method, type1, Type2, type2) \
    return_type  CAT(CAT(CAT(poppler_, klass), _), method) (CAT(Poppler,Klass) *arg0, type1 arg1, Type2 arg2) { \
        return (return_type) ( (Klass*) arg0 )->Method(arg1, (type2)arg2); \
    }
#define METHOD_THREEPFP( return_type, Method, method, type1, Type2, type2, type3) \
    return_type  CAT(CAT(CAT(poppler_, klass), _), method) (CAT(Poppler,Klass) *arg0, type1 arg1, Type2 arg2, type3 arg3) { \
        return (return_type) ( (Klass*) arg0 )->Method(arg1, (type2)arg2, arg3); \
    }
#define METHOD_THREEPPP( return_type, Method, method, type1, type2, type3) \
    return_type  CAT(CAT(CAT(poppler_, klass), _), method) (CAT(Poppler,Klass) *arg0, type1 arg1, type2 arg2, type3 arg3) { \
        return (return_type) ( (Klass*) arg0 )->Method(arg1, (type2)arg2, (type3)arg3); \
    }
#define BEGIN_ENUM
#define E(s)
#define END_ENUM

#include "poppler-imports.h"

#undef CONSTRUCTOR_MONAD
#undef CONSTRUCTOR_ONEP
#undef CONSTRUCTOR_ONEF
#undef DESTRUCTOR
#undef DEFINE_POPPLER_OBJECT
#undef METHOD_MONAD
#undef METHOD_ONEF
#undef METHOD_ONEP
#undef METHOD_TWOPF
#undef METHOD_TWOPP
#undef METHOD_THREEPFP
#undef METHOD_THREEPPP
#undef DEFINE_POPPLER_OBJECT
#undef BEGIN_ENUM
#undef E
#undef END_ENUM


