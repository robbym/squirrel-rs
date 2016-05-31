#include <stdarg.h>
#include <stdio.h>

typedef void (*callback_t)(void* v, size_t len, const char* buf);

static callback_t _print_callback;
static callback_t _err_callback;

void shim_set_print_callback(callback_t cb) {
	_print_callback = cb;
}

void shim_set_err_callback(callback_t cb) {
	_err_callback = cb;
}

void shim_print_fn(void* v, const char* s, ...) {
	va_list args1;
	va_start(args1, s);
	va_list args2;
	va_copy(args2, args1);

	char* buf = NULL;
	size_t len = vsnprintf(buf, 0, s, args1);
	va_end(args1);
	
	buf = (char*) malloc(len + 1); // Add one for trailing null
	vsnprintf(buf, len, s, args2);
	va_end(args2);
	
	_print_callback(v, len, buf);

	free(buf);
}

void shim_err_fn(void* v, const char* s, ...) {
	va_list args1;
	va_start(args1, s);
	va_list args2;
	va_copy(args2, args1);

	char* buf = NULL;
	size_t len = vsnprintf(buf, 0, s, args1);
	va_end(args1);
	
	buf = (char*) malloc(len + 1); // Add one for trailing null
	vsnprintf(buf, len, s, args2);
	va_end(args2);
	
	_err_callback(v, len, buf);

	free(buf);
}
