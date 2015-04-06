//========================================================================
//
// FileSpec.h
//
// All changes made under the Poppler project to this file are licensed
// under GPL version 2 or later
//
// Copyright (C) 2008 Carlos Garcia Campos <carlosgc@gnome.org>
//
// To see a description of the changes please see the Changelog file that
// came with your tarball or type make ChangeLog if you are building from git
//
//========================================================================

#ifndef FILE_SPEC_H
#define FILE_SPEC_H

#ifdef USE_GCC_PRAGMAS
#pragma interface
#endif

#include "Object.h"

class EmbFile {
public:
  EmbFile(Object *efStream);
  ~EmbFile();

  int size() { return m_size; }
  std::string modDate() { return m_modDate; }
  std::string createDate() { return m_createDate; }
  std::string checksum() { return m_checksum; }
  std::string mimeType() { return m_mimetype; }
  std::shared_ptr<Stream> stream() { return isOk() ? m_objStr.getStream() : 0; }
  GBool isOk() { return m_objStr.isStream(); }
  GBool save(const char *path);

private:
  GBool save2(FILE *f);

  int m_size;
  std::string m_createDate;
  std::string m_modDate;
  std::string m_checksum;
  std::string m_mimetype;
  Object m_objStr;
};

class FileSpec {
public:
  FileSpec(Object *fileSpec);
  ~FileSpec();

  GBool isOk() { return ok; }

  std::string getFileName() const { return fileName; }
  std::string getFileNameForPlatform();
  std::string getDescription() const { return desc; }
  EmbFile *getEmbeddedFile();

private:
  GBool ok;

  Object fileSpec;

  std::string fileName;         // F, UF, DOS, Mac, Unix
  std::string platformFileName;
  Object fileStream;           // Ref to F entry in UF
  EmbFile *embFile;
  std::string desc;             // Desc
};

GBool getFileSpecName (Object *fileSpec, Object *fileName);
GBool getFileSpecNameForPlatform (Object *fileSpec, Object *fileName);

#endif /* FILE_SPEC_H */
