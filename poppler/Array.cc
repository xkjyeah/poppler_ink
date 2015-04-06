//========================================================================
//
// Array.cc
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
// Copyright (C) 2005 Kristian Høgsberg <krh@redhat.com>
// Copyright (C) 2012 Fabio D'Urso <fabiodurso@hotmail.it>
// Copyright (C) 2013 Thomas Freitag <Thomas.Freitag@alfa.de>
// Copyright (C) 2013 Albert Astals Cid <aacid@kde.org>
//
// To see a description of the changes please see the Changelog file that
// came with your tarball or type make ChangeLog if you are building from git
//
//========================================================================

#include <config.h>

#ifdef USE_GCC_PRAGMAS
#pragma implementation
#endif

#include <stdlib.h>
#include <stddef.h>
#include "goo/gmem.h"
#include "Object.h"
#include "Array.h"

#if MULTITHREADED
#  define arrayLocker()   MutexLocker locker(&mutex)
#else
#  define arrayLocker()
#endif
//------------------------------------------------------------------------
// Array
//------------------------------------------------------------------------

Array::Array(XRef *xrefA) {
  xref = xrefA;
#if MULTITHREADED
  gInitMutex(&mutex);
#endif
}

Array::~Array() {
#if MULTITHREADED
  gDestroyMutex(&mutex);
#endif
}

Object *Array::copy(XRef *xrefA, Object *obj) {
  arrayLocker();
  obj->initArray(xrefA);
  for (int i = 0; i < length; ++i) {
    Object obj1;
    obj->arrayAdd(elems[i]);
  }
  return obj;
}

int Array::incRef() {
	return 1;
}

int Array::decRef() {
	return 1;
}

void Array::add(const Object &elem) {
  arrayLocker();
  if (length == size) {
    if (length == 0) {
      size = 8;
    } else {
      size *= 2;
    }
    elems = (Object *)greallocn(elems, size, sizeof(Object));
  }
  elems[length] = *elem;
  ++length;
}

void Array::remove(int i) {
  arrayLocker();
  if (i < 0 || i >= elems.size()) {
#ifdef DEBUG_MEM
    abort();
#else
    return;
#endif
  }
	elems.erase(i);
}

Object &Array::get(int i, int recursion) {
  if (i < 0 || i >= elems.size()) {
#ifdef DEBUG_MEM
    abort();
#else
    return obj->initNull();
#endif
  }
  return elems[i].fetch(xref, recursion);
}

Object &Array::getNF(int i) {
  if (i < 0 || i >= elems.size()) {
#ifdef DEBUG_MEM
    abort();
#else
    return obj->initNull();
#endif
  }
  return elems[i];
}

GBool Array::getString(int i, std::string &string)
{
  Object obj = getNF(i);

  if (obj.isString()) {
		string = obj.getString();
    return gTrue;
  } else {
    return gFalse;
  }
}
