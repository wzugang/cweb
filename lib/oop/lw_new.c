#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <error.h>
#include "lw_new.h"
#include "../cgic/cgic.h"

klass_info *klass_of(void *self) {
	assert(self);
	klass_info *kls = *((klass_info **) self);
	return kls;
}

size_t klass_size_of(void *self) {
	assert(self);
	klass_info *kls = *((klass_info **) self);
	return kls->size;
}

void *lw_new(void *klass) {
	klass_info *kls = KLASS(klass);
	if (kls->init) {
		((init_fun) (kls->init))();
	}
	void *obj = malloc(kls->size);
	*((klass_info **) obj) = kls;
	return ((void_fun) (kls->ctor))(obj);
}

void lw_destory(void *self) {
	klass_info *kls = *((klass_info **) self);
	if (kls) {
		free(((void_fun) (kls->dtor))(self));
		self = NULL;
#ifdef DEBUG
		fprintf(cgiOut , "%s" , kls->class_name);
#endif
	}
}
