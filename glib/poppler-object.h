#ifndef __POPPLER_OBJECT_H__
#define __POPPLER_OBJECT_H__

#include <glib.h>

G_BEGIN_DECLS

#define CAT(a, b) PRIMITIVE_CAT(a,b)
#define PRIMITIVE_CAT(a, b) a ## b
#define DEFINE_POPPLER_OBJECT(Name) \
    typedef struct _Poppler ## Name Poppler ## Name ;

#define CONSTRUCTOR_MONAD(method) \
    CAT(Poppler,Klass)*  CAT(CAT(CAT(poppler_, klass), _), method) ();

#define CONSTRUCTOR_ONEP(method, type1) \
    CAT(Poppler,Klass)*  CAT(CAT(CAT(poppler_, klass), _), method) (type1 arg1);

#define CONSTRUCTOR_ONEF(method, Type1, type1) \
    CAT(Poppler,Klass)*  CAT(CAT(CAT(poppler_, klass), _), method) (type1 arg1);

#define DESTRUCTOR(method) \
    void  CAT(CAT(CAT(poppler_, klass), _), method) (CAT(Poppler,Klass) *arg0);

#define METHOD_MONAD( return_type, Method, method) \
    return_type  CAT(CAT(CAT(poppler_, klass), _), method) (CAT(Poppler,Klass) *arg0);

#define METHOD_ONEF( return_type, Method, method, Type1, type1) \
    return_type  CAT(CAT(CAT(poppler_, klass), _), method) (CAT(Poppler,Klass) *arg0, Type1 arg1);

#define METHOD_ONEP( return_type, Method, method, type1) \
    return_type  CAT(CAT(CAT(poppler_, klass), _), method) (CAT(Poppler,Klass) *arg0, type1 arg1);

#define METHOD_TWOPF( return_type, Method, method, type1, Type2, type2) \
    return_type  CAT(CAT(CAT(poppler_, klass), _), method) (CAT(Poppler,Klass) *arg0, type1 arg1, Type2 arg2);

#define METHOD_THREEPFP( return_type, Method, method, type1, Type2, type2, type3) \
    return_type  CAT(CAT(CAT(poppler_, klass), _), method) (CAT(Poppler,Klass) *arg0, type1 arg1, Type2 arg2, type3 arg3);

#define METHOD_THREEPPP( return_type, Method, method, type1, type2, type3) \
    return_type  CAT(CAT(CAT(poppler_, klass), _), method) (CAT(Poppler,Klass) *arg0, type1 arg1, type2 arg2, type3 arg3);

#define BEGIN_ENUM   typedef enum {
#define END_ENUM     } CAT(Poppler, Enum);
#define E(s)            CAT(CAT(CAT(POPPLER_, ENUM), _), s)
#define WANT_ENUMS

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
#undef E
#undef BEGIN_ENUM
#undef END_ENUM
#undef WANT_ENUMS


G_END_DECLS

#endif

