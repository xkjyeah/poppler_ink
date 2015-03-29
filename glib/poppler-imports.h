#ifdef __POPPLER_OBJECT_H__

DEFINE_POPPLER_OBJECT(XRef)
DEFINE_POPPLER_OBJECT(Object)
//DEFINE_POPPLER_OBJECT(Bool)
//DEFINE_POPPLER_OBJECT(Int)
//DEFINE_POPPLER_OBJECT(Real)
//DEFINE_POPPLER_OBJECT(String)
//DEFINE_POPPLER_OBJECT(Name)
//DEFINE_POPPLER_OBJECT(Null)
DEFINE_POPPLER_OBJECT(Array)
DEFINE_POPPLER_OBJECT(Dict)
//DEFINE_POPPLER_OBJECT(Stream)

//PopplerDict     *poppler_dict_new(gpointer xref) {
//    return (PopplerDict*) new Dict( (XRef*) xref );
//}
//PopplerDict     *poppler_dict_new_from_dict(gpointer xref) {
//    return (PopplerDict*) new Dict( (Dict*) xref );
//}
//void poppler_dict_free(PopplerDict *a) {
//    delete ((Dict*)a);
//}
//PopplerDict     *poppler_dict_copy(PopplerDict *d, gpointer xref) {
//    return ( (PopplerDict*)d )->copy( (XRef*) xref );
//}

// Dictionary objects
#define Klass Dict
#define klass dict
METHOD_MONAD( int, incRef, inc_ref)
METHOD_MONAD( int, decRef, dec_ref)
METHOD_MONAD( int, getLength, get_length)
METHOD_MONAD( PopplerXRef*, getXRef, get_x_ref)

METHOD_TWOPF( void, set, set, const char*, PopplerObject*, Object*)

METHOD_ONEP( void, remove, remove, const char*)
METHOD_ONEP( gboolean, is, is, const char*)
METHOD_THREEPFP( PopplerObject*, lookup, lookup, const char*, PopplerObject *, Object*, int)
METHOD_TWOPF( PopplerObject*, lookupNF, lookupNF, const char*, PopplerObject *, Object*)
METHOD_THREEPPP( gboolean, lookupInt, lookup_int, const char*, const char *, int *)

METHOD_ONEP( void, getKey, get_key, int)
METHOD_TWOPF( PopplerObject *, getVal, get_val, int, PopplerObject *, Object *)
METHOD_TWOPF( PopplerObject *, getValNF, get_val_nf, int, PopplerObject *, Object *)

DESTRUCTOR(free)
#undef Klass
#undef klass

// Array objects
#define Klass Array
#define klass array

CONSTRUCTOR_ONEF(new, XRef*, gpointer)
METHOD_MONAD(int, incRef, inc_ref)
METHOD_MONAD(int, decRef, dec_ref)
METHOD_MONAD(int, getLength, get_length)

METHOD_THREEPFP(PopplerObject*, get, get, int, PopplerObject*, Object*, int)
METHOD_TWOPF(PopplerObject*, getNF, getNF, int, PopplerObject*, Object*)

#undef Klass
#undef klass


//#define POPPLER_TYPE_DOCUMENT             (poppler_document_get_type ())
//#define POPPLER_DOCUMENT(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), POPPLER_TYPE_DOCUMENT, PopplerDocument))
//#define POPPLER_IS_DOCUMENT(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), POPPLER_TYPE_DOCUMENT))


// AnnotBorder objects
DEFINE_POPPLER_OBJECT(AnnotBorder)
DEFINE_POPPLER_OBJECT(AnnotBorderBS)

// AnnotBorder-related enums
#ifdef WANT_ENUMS
    #define Enum AnnotBorderType
    #define ENUM ANNOT_BORDER_TYPE
    BEGIN_ENUM
    E(TYPE_ARRAY),
    E(TYPE_BS)
    END_ENUM
    #undef Enum
    #undef ENUM

    #define Enum AnnotBorderStyle
    #define ENUM ANNOT_BORDER_STYLE
    BEGIN_ENUM
    E(SOLID),
    E(DASHED),
    E(BEVELD),
    E(INSET),
    E(UNDERLINED)
    END_ENUM
    #undef Enum
    #undef ENUM
#endif

#define Klass AnnotBorderBS
#define klass annot_border_bs
CONSTRUCTOR_MONAD(new)
#undef Klass
#undef klass 

#define Klass AnnotBorder
#define klass annot_border
METHOD_ONEP(void, setWidth, set_width, double)
METHOD_MONAD(double, getWidth, get_width)
METHOD_MONAD(double*, getDash, get_dash)
METHOD_MONAD(int, getDashLength, get_dash_length)
METHOD_MONAD(PopplerAnnotBorderType, getType, get_type)
METHOD_MONAD(PopplerAnnotBorderStyle, getStyle, get_style)
#undef Klass
#undef klass

#define Klass Object
#define klass object
CONSTRUCTOR_MONAD(new)
METHOD_ONEF(PopplerObject*, initDict, init_dict, PopplerDict*, Dict*)
METHOD_ONEP(PopplerObject*, initName, init_name, const char *)
METHOD_ONEF(PopplerObject*, initDict, init_dict_xref, PopplerXRef*, XRef*)
METHOD_ONEF(PopplerObject*, initArray, init_array_xref, PopplerXRef*, XRef*)
METHOD_TWOPF(void, dictSet, dict_set, const char *, PopplerObject*, Object*)
METHOD_MONAD(void, free, free)
#undef Klass
#undef klass

#endif
