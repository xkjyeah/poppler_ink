//========================================================================
//
// Object.h
//
// Copyright 1996-2003 Glyph & Cog, LLC
//
//========================================================================

//========================================================================
//
// Modified under the Poppler project - http://poppler.freedesktop.org
//
// All changes made under the Poppler project to this file are licensed
// under GPL version 2 or later
//
// Copyright (C) 2007 Julien Rebetez <julienr@svn.gnome.org>
// Copyright (C) 2008 Kees Cook <kees@outflux.net>
// Copyright (C) 2008, 2010 Albert Astals Cid <aacid@kde.org>
// Copyright (C) 2009 Jakub Wilk <ubanus@users.sf.net>
// Copyright (C) 2012 Fabio D'Urso <fabiodurso@hotmail.it>
// Copyright (C) 2013 Thomas Freitag <Thomas.Freitag@alfa.de>
// Copyright (C) 2013 Adrian Johnson <ajohnson@redneon.com>
// Copyright (C) 2013 Adrian Perez de Castro <aperez@igalia.com>
//
// To see a description of the changes please see the Changelog file that
// came with your tarball or type make ChangeLog if you are building from git
//
//========================================================================

#ifndef OBJECT_H
#define OBJECT_H

#ifdef USE_GCC_PRAGMAS
#pragma interface
#endif

#include <set>
#include <cstdio>
#include <string>
#include <memory>
#include "Error.h"

#define OBJECT_TYPE_CHECK(wanted_type) \
    if (unlikely(type != wanted_type)) { \
        error(errInternal, 0, "Call to Object where the object was type {0:d}, " \
                 "not the expected type {1:d}", type, wanted_type); \
        abort(); \
    }

#define OBJECT_3TYPES_CHECK(wanted_type1, wanted_type2, wanted_type3) \
    if (unlikely(type != wanted_type1) && unlikely(type != wanted_type2) && unlikely(type != wanted_type3)) { \
        error(errInternal, 0, "Call to Object where the object was type {0:d}, " \
	      "not the expected type {1:d}, {2:d} or {3:d}", type, wanted_type1, wanted_type2, wanted_type3); \
        abort(); \
    }

class XRef;
class Array;
class Dict;
class Stream;

//------------------------------------------------------------------------
// Ref
//------------------------------------------------------------------------

struct Ref {
  int num;			// object number
  int gen;			// generation number
};


//------------------------------------------------------------------------
// Contents...
//------------------------------------------------------------------------

struct Contents {
  virtual ~Contents() {} 
};
struct BoolContents : Contents {
  bool booln;
  BoolContents(bool b) : booln(b) {}
};
struct IntContents : Contents {
  int intg;
  IntContents(int b) : intg(b) {}
};
struct Int64Contents : Contents {
  long long int64g;
  Int64Contents(long long b) : int64g(b) {}
};
struct DoubleContents : Contents {
  double real;
  DoubleContents(long long b) : real(b) {}
};
struct StringContents : Contents {
  std::string string;
  StringContents(std::string b) : string(b) {}
};
struct NameContents : Contents {
  std::string string;
  NameContents(std::string b) : string(b) {}
};
struct ArrayContents : Contents {
  std::shared_ptr<Array> array;
  ArrayContents(XRef *xref);
};
struct DictContents : Contents {
  std::shared_ptr<Dict> dict;
  DictContents(XRef *xref);
  DictContents(std::shared_ptr<Dict> dict);
};
struct StreamContents : Contents {
  std::shared_ptr<Stream> str;
  StreamContents(std::shared_ptr<Stream> str) : str(str) {}
};
struct RefContents : Contents {
  Ref ref;
  RefContents(int numA, int genA) : ref{numA, genA} {}
};
struct CommandContents : Contents {
  std::string str;
  CommandContents(std::string str) : str(str) {}
};
//------------------------------------------------------------------------
// object types
//------------------------------------------------------------------------

enum ObjType {
  // simple objects
  objBool,			// boolean
  objInt,			// integer
  objReal,			// real
  objString,			// string
  objName,			// name
  objNull,			// null

  // complex objects
  objArray,			// array
  objDict,			// dictionary
  objStream,			// stream
  objRef,			// indirect reference

  // special objects
  objCmd,			// command name
  objError,			// error return from Lexer
  objEOF,			// end of file return from Lexer
  objNone,			// uninitialized object

  // poppler-only objects
  objInt64			// integer with at least 64-bits
};

#define numObjTypes 15		// total number of object types

//------------------------------------------------------------------------
// Object
//------------------------------------------------------------------------

//#define initObj(t) zeroUnion(); ++numAlloc[type = t]
//#else
//#define initObj(t) zeroUnion(); type = t
//#endif

class Object {
public:
  // clear the anonymous union as best we can -- clear at least a pointer
  void zeroUnion() { contents.reset(); }

  // Default constructor.
  Object():
    contents() { }

  // Copy constructor -- use default

  // Initialize an object.
  Object *initBool(bool boolnA)
    { contents.reset(new BoolContents()); booln = boolnA; return this; }
  Object *initInt(int intgA)
    { initObj(objInt); intg = intgA; return this; }
  Object *initReal(double realA)
    { initObj(objReal); real = realA; return this; }
  Object *initString(GooString *stringA)
    { initObj(objString); string = stringA; return this; }
  Object *initName(const char *nameA)
    { initObj(objName); name = copyString(nameA); return this; }
  Object *initNull()
    { initObj(objNull); return this; }
  Object *initArray(XRef *xref);
  Object *initDict(XRef *xref);
  Object *initDict(Dict *dictA);
  Object *initStream(Stream *streamA);
  Object *initRef(int numA, int genA)
    { initObj(objRef); ref.num = numA; ref.gen = genA; return this; }
  Object *initCmd(char *cmdA)
    { initObj(objCmd); cmd = copyString(cmdA); return this; }
  Object *initError()
    { initObj(objError); return this; }
  Object *initEOF()
    { initObj(objEOF); return this; }
  Object *initInt64(long long int64gA)
    { initObj(objInt64); int64g = int64gA; return this; }

  // Copy an object.
//  Object *copy(Object *obj);
//  Object *shallowCopy(Object *obj) {
//    *obj = *this;
//    return obj;
//  }

  // If object is a Ref, fetch and return the referenced object.
  // Otherwise, return a copy of the object.
  Object *fetch(XRef *xref, Object *obj, int recursion = 0);

  // Type checking.
  ObjType getType() { return type; }
  bool isBool() { return type == objBool; }
  bool isInt() { return type == objInt; }
  bool isReal() { return type == objReal; }
  bool isNum() { return type == objInt || type == objReal || type == objInt64; }
  bool isString() { return type == objString; }
  bool isName() { return type == objName; }
  bool isNull() { return type == objNull; }
  bool isArray() { return type == objArray; }
  bool isDict() { return type == objDict; }
  bool isStream() { return type == objStream; }
  bool isRef() { return type == objRef; }
  bool isCmd() { return type == objCmd; }
  bool isError() { return type == objError; }
  bool isEOF() { return type == objEOF; }
  bool isNone() { return type == objNone; }
  bool isInt64() { return type == objInt64; }

  // Special type checking.
  bool isName(const char *nameA)
    { return type == objName && !strcmp(name, nameA); }
  bool isDict(const char *dictType);
  bool isStream(char *dictType);
  bool isCmd(const char *cmdA)
    { return type == objCmd && !strcmp(cmd, cmdA); }

  // Accessors.
  bool getBool() { OBJECT_TYPE_CHECK(objBool); return booln; }
  int getInt() { OBJECT_TYPE_CHECK(objInt); return intg; }
  double getReal() { OBJECT_TYPE_CHECK(objReal); return real; }

  // Note: integers larger than 2^53 can not be exactly represented by a double.
  // Where the exact value of integers up to 2^63 is required, use isInt64()/getInt64().
  double getNum() { OBJECT_3TYPES_CHECK(objInt, objInt64, objReal);
    return type == objInt ? (double)intg : type == objInt64 ? (double)int64g : real; }
  GooString *getString() { OBJECT_TYPE_CHECK(objString); return string; }
  // After takeString() the only method that should be called for the object is free()
  // because the object it's not expected to have a NULL string.
  GooString *takeString() {
    OBJECT_TYPE_CHECK(objString); GooString *s = string; string = NULL; return s; }
  char *getName() { OBJECT_TYPE_CHECK(objName); return name; }
  Array *getArray() { OBJECT_TYPE_CHECK(objArray); return array; }
  Dict *getDict() { OBJECT_TYPE_CHECK(objDict); return dict; }
  Stream *getStream() { OBJECT_TYPE_CHECK(objStream); return stream; }
  Ref getRef() { OBJECT_TYPE_CHECK(objRef); return ref; }
  int getRefNum() { OBJECT_TYPE_CHECK(objRef); return ref.num; }
  int getRefGen() { OBJECT_TYPE_CHECK(objRef); return ref.gen; }
  char *getCmd() { OBJECT_TYPE_CHECK(objCmd); return cmd; }
  long long getInt64() { OBJECT_TYPE_CHECK(objInt64); return int64g; }

  // Array accessors.
  int arrayGetLength();
  void arrayAdd(Object *elem);
  void arrayRemove(int i);
  Object *arrayGet(int i, Object *obj, int recursion);
  Object *arrayGetNF(int i, Object *obj);

  // Dict accessors.
  int dictGetLength();
  void dictAdd(char *key, Object *val);
  void dictSet(const char *key, Object *val);
  bool dictIs(const char *dictType);
  Object *dictLookup(const char *key, Object *obj, int recursion = 0);
  Object *dictLookupNF(const char *key, Object *obj);
  char *dictGetKey(int i);
  Object *dictGetVal(int i, Object *obj);
  Object *dictGetValNF(int i, Object *obj);

  // Stream accessors.
  bool streamIs(char *dictType);
  void streamReset();
  void streamClose();
  int streamGetChar();
  int streamGetChars(int nChars, Guchar *buffer);
  int streamLookChar();
  char *streamGetLine(char *buf, int size);
  Goffset streamGetPos();
  void streamSetPos(Goffset pos, int dir = 0);
  Dict *streamGetDict();

  // Output.
  const char *getTypeName();
  void print(FILE *f = stdout);

  // Memory testing.
  static void memCheck(FILE *f);

private:

  //ObjType type;			// object type
  std::shared_ptr<Contents> contents;
//  union {			// value for each type:
//    bool booln;		//   boolean
//    int intg;			//   integer
//    long long int64g;           //   64-bit integer
//    double real;		//   real
//    std::string string;		//   string
//    char *name;			//   name
//    Array *array;		//   array
//    Dict *dict;			//   dictionary
//    Stream *stream;		//   stream
//    Ref ref;			//   indirect reference
//    char *cmd;			//   command
//  };
};

//------------------------------------------------------------------------
// Array accessors.
//------------------------------------------------------------------------

#include "Array.h"

inline int Object::arrayGetLength()
  { OBJECT_TYPE_CHECK(objArray); return array->getLength(); }

inline void Object::arrayAdd(Object *elem)
  { OBJECT_TYPE_CHECK(objArray); array->add(elem); }

inline void Object::arrayRemove(int i)
  { OBJECT_TYPE_CHECK(objArray); array->remove(i); }

inline Object *Object::arrayGet(int i, Object *obj, int recursion = 0)
  { OBJECT_TYPE_CHECK(objArray); return array->get(i, obj, recursion); }

inline Object *Object::arrayGetNF(int i, Object *obj)
  { OBJECT_TYPE_CHECK(objArray); return array->getNF(i, obj); }

//------------------------------------------------------------------------
// Dict accessors.
//------------------------------------------------------------------------

#include "Dict.h"

inline int Object::dictGetLength()
  { OBJECT_TYPE_CHECK(objDict); return dict->getLength(); }

inline void Object::dictAdd(char *key, Object *val)
  { OBJECT_TYPE_CHECK(objDict); dict->add(key, val); }

inline void Object::dictSet(const char *key, Object *val)
 	{ OBJECT_TYPE_CHECK(objDict); dict->set(key, val); }

inline bool Object::dictIs(const char *dictType)
  { OBJECT_TYPE_CHECK(objDict); return dict->is(dictType); }

inline bool Object::isDict(const char *dictType)
  { return type == objDict && dictIs(dictType); }

inline Object *Object::dictLookup(const char *key, Object *obj, int recursion)
  { OBJECT_TYPE_CHECK(objDict); return dict->lookup(key, obj, recursion); }

inline Object *Object::dictLookupNF(const char *key, Object *obj)
  { OBJECT_TYPE_CHECK(objDict); return dict->lookupNF(key, obj); }

inline char *Object::dictGetKey(int i)
  { OBJECT_TYPE_CHECK(objDict); return dict->getKey(i); }

inline Object *Object::dictGetVal(int i, Object *obj)
  { OBJECT_TYPE_CHECK(objDict); return dict->getVal(i, obj); }

inline Object *Object::dictGetValNF(int i, Object *obj)
  { OBJECT_TYPE_CHECK(objDict); return dict->getValNF(i, obj); }

//------------------------------------------------------------------------
// Stream accessors.
//------------------------------------------------------------------------

#include "Stream.h"

inline bool Object::streamIs(char *dictType)
  { OBJECT_TYPE_CHECK(objStream); return stream->getDict()->is(dictType); }

inline bool Object::isStream(char *dictType)
  { return type == objStream && streamIs(dictType); }

inline void Object::streamReset()
  { OBJECT_TYPE_CHECK(objStream); stream->reset(); }

inline void Object::streamClose()
  { OBJECT_TYPE_CHECK(objStream); stream->close(); }

inline int Object::streamGetChar()
  { OBJECT_TYPE_CHECK(objStream); return stream->getChar(); }

inline int Object::streamGetChars(int nChars, Guchar *buffer)
  { OBJECT_TYPE_CHECK(objStream); return stream->doGetChars(nChars, buffer); }

inline int Object::streamLookChar()
  { OBJECT_TYPE_CHECK(objStream); return stream->lookChar(); }

inline char *Object::streamGetLine(char *buf, int size)
  { OBJECT_TYPE_CHECK(objStream); return stream->getLine(buf, size); }

inline Goffset Object::streamGetPos()
  { OBJECT_TYPE_CHECK(objStream); return stream->getPos(); }

inline void Object::streamSetPos(Goffset pos, int dir)
  { OBJECT_TYPE_CHECK(objStream); stream->setPos(pos, dir); }

inline Dict *Object::streamGetDict()
  { OBJECT_TYPE_CHECK(objStream); return stream->getDict(); }

#endif
