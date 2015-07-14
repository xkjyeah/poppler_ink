#include "Object.h"
#include "Intrusives.h"


void intrusive_ptr_add_ref(Dict *d) {
	d->incRef();
}
void intrusive_ptr_release(Dict *d) {
	if (!d->decRef()) {
		delete d;
	}
}

void intrusive_ptr_add_ref(Stream *d) {
	d->incRef();
}
void intrusive_ptr_release(Stream *d) {
	if (!d->decRef()) {
		delete d;
	}
}

void intrusive_ptr_add_ref(Array *d) {
	d->incRef();
}
void intrusive_ptr_release(Array *d) {
	if (!d->decRef()) {
		delete d;
	}
}



