//========================================================================
//
// Array.h
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
//
// To see a description of the changes please see the Changelog file that
// came with your tarball or type make ChangeLog if you are building from git
//
//========================================================================

#ifndef ARRAY_H
#define ARRAY_H

#ifdef USE_GCC_PRAGMAS
#pragma interface
#endif

#include "poppler-config.h"
#include "Object.h"
#include "goo/GooMutex.h"
#include <vector>

class XRef;

//------------------------------------------------------------------------
// Array
//------------------------------------------------------------------------

class Array  {
public:

  // Constructor.
  Array(XRef *xrefA);

  // Destructor.
  ~Array();

  // Reference counting.
	__attribute__((deprecated))
  int incRef();
	__attribute__((deprecated))
  int decRef();

  // Get number of elements.
  int getLength() { return elems.size(); }

  // Copy array with new xref
	__attribute__((deprecated))
  const Object &copy(XRef *xrefA, const Object &obj);

  // Add an element.
  void add(const Object &elem);

  // Remove an element by position
  void remove(int i);

  // Accessors.
  Object &get(int i, int resursion = 0);
  Object &getNF(int i);
  GBool getString(int i, std::string &string);

private:

  XRef *xref;			// the xref table for this PDF file
	std::vector<Object> elems;
  //int size;			// size of <elems> array
  //int length;			// number of elements in array
  //int ref;			// reference count
#if MULTITHREADED
  GooMutex mutex;
#endif
};

#endif
