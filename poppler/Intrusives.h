#pragma once

class XRef;
class Array;
class Dict;
class Stream;
class GooString;
#include <boost/intrusive_ptr.hpp>
#include <memory>

void intrusive_ptr_add_ref(Dict *d);
void intrusive_ptr_release(Dict *d);
void intrusive_ptr_add_ref(Stream *d);
void intrusive_ptr_release(Stream *d);
void intrusive_ptr_add_ref(Array *d);
void intrusive_ptr_release(Array *d);
void intrusive_ptr_add_ref(GooString *d);
void intrusive_ptr_release(GooString *d);

template <typename T>
struct gmalloc_deleter {
	void operator() (T *t) {
		gfree(t);
	}
};

typedef boost::intrusive_ptr<Dict> DictPtr;
typedef boost::intrusive_ptr<Stream> StreamPtr;
typedef boost::intrusive_ptr<Array> ArrayPtr;
typedef std::unique_ptr<GooString> StringPtr;
typedef std::unique_ptr<char, gmalloc_deleter<char> > CharPtr;
