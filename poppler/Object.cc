//========================================================================
//
// Object.cc
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
// Copyright (C) 2008, 2010, 2012 Albert Astals Cid <aacid@kde.org>
// Copyright (C) 2013 Adrian Johnson <ajohnson@redneon.com>
//
// To see a description of the changes please see the Changelog file that
// came with your tarball or type make ChangeLog if you are building from git
//
//========================================================================

#include <config.h>

#ifdef USE_GCC_PRAGMAS
#pragma implementation
#endif

#include <stddef.h>
#include "Object.h"
#include "Array.h"
#include "Dict.h"
#include "Error.h"
#include "Stream.h"
#include "XRef.h"

//------------------------------------------------------------------------
// Object
//------------------------------------------------------------------------

static const char *objTypeNames[numObjTypes] = {
  "boolean",
  "integer",
  "real",
  "string",
  "name",
  "null",
  "array",
  "dictionary",
  "stream",
  "ref",
  "cmd",
  "error",
  "eof",
  "none",
  "integer64"
};

#ifdef DEBUG_MEM
int Object::numAlloc[numObjTypes] =
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
#endif

Object *Object::initArray(XRef *xref) {
  initObj(objArray);
  array = new Array(xref);
  return this;
}

Object *Object::initDict(XRef *xref) {
  initObj(objDict);
  dict = new Dict(xref);
  return this;
}

Object *Object::initDict(Dict *dictA) {
  initObj(objDict);
  dict = dictA;
  return this;
}

Object *Object::initStream(Stream *streamA) {
  initObj(objStream);
  stream = streamA;
  return this;
}

/** equivalent to "shallow copy" **/
Object::Object(const Object &other)
: type(objNone) {
	*this = other;
}
Object::Object(Object &&other)
: type(objNone) {
	*this = other;
}
Object &Object::operator=(Object &&other) {
	if (this == &other)
		return *this;

	free();

	this->type = other.type;
	other.type = objNone;

  switch (type) {
  case objString:
		new (&string) StringPtr(std::move(other.string));
    break;
  case objName:
		new (&name) CharPtr(std::move(other.name));
    break;
  case objArray:
		new (&array) ArrayPtr(std::move(other.array));
    break;
  case objDict:
		new (&dict) DictPtr(std::move(other.dict));
    break;
  case objStream:
		new (&stream) StreamPtr(std::move(other.stream));
    break;
  case objCmd:
		new (&cmd) CharPtr(std::move(other.cmd));
    break;
  default:
		if (sizeof(int64g) >= sizeof(ref)) {
			int64g = other.int64g;
		}
		else {
			ref = other.ref;
		}
    break;
  }
  return *this;
}

/** deep copy **/
Object &Object::operator=(const Object &other) {
	free();

	this->type = other.type;

  switch (type) {
  case objString:
		new (&string) StringPtr(other.string->copy());
    break;
  case objName:
		new (&name) CharPtr(copyString(other.name.get()));
    break;
  case objArray:
		new (&array) ArrayPtr(other.array);
    break;
  case objDict:
		new (&dict) DictPtr(other.dict);
    break;
  case objStream:
		new (&stream) StreamPtr(other.stream);
    break;
  case objCmd:
		new (&cmd) CharPtr(copyString(other.cmd.get()));
    break;
  default:
		if (sizeof(int64g) >= sizeof(ref)) {
			int64g = other.int64g;
		}
		else {
			ref = other.ref;
		}
    break;
  }
  return *this;
}

Object::~Object() {
	free();
}

Object *Object::copy(Object *obj) {
  *obj = *this;
  return obj;
}

Object *Object::fetch(XRef *xref, Object *obj, int recursion) {
  return (type == objRef && xref) ?
         xref->fetch(ref.num, ref.gen, obj, recursion) : copy(obj);
}

void Object::free() {
  switch (type) {
  case objString:
    string.~unique_ptr();
    break;
  case objName:
    name.~unique_ptr();
    break;
  case objArray:
    array.~intrusive_ptr();
    break;
  case objDict:
    dict.~intrusive_ptr();
    break;
  case objStream:
    stream.~intrusive_ptr();
    break;
  case objCmd:
		cmd.~unique_ptr();
    break;
  default:
    break;
  }
#ifdef DEBUG_MEM
  --numAlloc[type];
#endif
  type = objNone;
}

const char *Object::getTypeName() {
  return objTypeNames[type];
}

void Object::print(FILE *f) {
  Object obj;
  int i;

  switch (type) {
  case objBool:
    fprintf(f, "%s", booln ? "true" : "false");
    break;
  case objInt:
    fprintf(f, "%d", intg);
    break;
  case objReal:
    fprintf(f, "%g", real);
    break;
  case objString:
    fprintf(f, "(");
    fwrite(string->getCString(), 1, string->getLength(), f);
    fprintf(f, ")");
    break;
  case objName:
    fprintf(f, "/%s", name.get());
    break;
  case objNull:
    fprintf(f, "null");
    break;
  case objArray:
    fprintf(f, "[");
    for (i = 0; i < arrayGetLength(); ++i) {
      if (i > 0)
	fprintf(f, " ");
      arrayGetNF(i, &obj);
      obj.print(f);
      obj.free();
    }
    fprintf(f, "]");
    break;
  case objDict:
    fprintf(f, "<<");
    for (i = 0; i < dictGetLength(); ++i) {
      fprintf(f, " /%s ", dictGetKey(i));
      dictGetValNF(i, &obj);
      obj.print(f);
      obj.free();
    }
    fprintf(f, " >>");
    break;
  case objStream:
    fprintf(f, "<stream>");
    break;
  case objRef:
    fprintf(f, "%d %d R", ref.num, ref.gen);
    break;
  case objCmd:
    fprintf(f, "%s", cmd.get());
    break;
  case objError:
    fprintf(f, "<error>");
    break;
  case objEOF:
    fprintf(f, "<EOF>");
    break;
  case objNone:
    fprintf(f, "<none>");
    break;
  case objInt64:
    fprintf(f, "%lld", int64g);
    break;
  }
}

void Object::memCheck(FILE *f) {
#ifdef DEBUG_MEM
  int i;
  int t;

  t = 0;
  for (i = 0; i < numObjTypes; ++i)
    t += numAlloc[i];
  if (t > 0) {
    fprintf(f, "Allocated objects:\n");
    for (i = 0; i < numObjTypes; ++i) {
      if (numAlloc[i] > 0)
	fprintf(f, "  %-20s: %6d\n", objTypeNames[i], numAlloc[i]);
    }
  }
#else
  (void)f;
#endif
}
