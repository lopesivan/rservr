#ifndef python_macro_h
#define python_macro_h

#ifdef __cplusplus
extern "C" {
#endif

#include <Python.h>

#include "load-all.h"


#define TYPE_PREFIX "rservr."


static inline PyObject __attribute__ ((warn_unused_result)) *auto_exception(PyObject *eException,
  const char *mMessage)
{
	PyErr_SetString(eException, mMessage);
	return NULL;
}

static inline int __attribute__ ((warn_unused_result)) delay_exception(PyObject *eException,
  const char *mMessage)
{
	PyErr_SetString(eException, mMessage);
	return 0;
}

static inline int __attribute__ ((warn_unused_result)) delay_exception2(PyObject *eException,
  const char *mMessage)
{
	PyErr_SetString(eException, mMessage);
	return -1;
}


static inline PyObject __attribute__ ((deprecated)) *not_implemented(void)
{ return auto_exception(PyExc_NotImplementedError, ""); }


#define SELF sSelf
#define ARGS aArgs


#define STRING_CASE(var, string) else if (strcmp(var, string) == 0)


#define TYPE_WRAPPER(type) \
python_##type

#define DEFINE_TYPE_WRAPPER(type, object) \
typedef struct { \
  PyObject_HEAD \
  object pointer; \
} TYPE_WRAPPER(type);

#define DEFINE_POINTER_TYPE_WRAPPER(type) \
DEFINE_TYPE_WRAPPER(type, const struct type*)

#define NEW_TYPE_WRAPPER(type, pointer) \
new_python_##type(pointer)

#define NEW_TYPE_WRAPPER_DECLARE(type, object) \
PyObject ATTR_INT *new_python_##type(object);

#define NEW_POINTER_TYPE_WRAPPER_DECLARE(type) \
NEW_TYPE_WRAPPER_DECLARE(type, const struct type*);

#define NEW_TYPE_WRAPPER_DEFINE(type, object) \
PyObject *new_python_##type(object pointer) { \
  if (!pointer) return auto_exception(PyExc_IndexError, #type); \
  python_##type *self = calloc(1, sizeof(python_##type)); \
  if (!self) return auto_exception(PyExc_RuntimeError, #type); \
  self->ob_type = &TYPE_BINDING(type); \
  self->ob_refcnt = 1; \
  self->pointer = pointer; \
  return (PyObject*) self; }

#define NEW_POINTER_TYPE_WRAPPER_DEFINE(type) \
NEW_TYPE_WRAPPER_DEFINE(type, const struct type*)


#define TYPE_WRAPPER_COMPARE(type) \
python_##type##_compare

#define TYPE_WRAPPER_COMPARE_DEFINE(type) \
static int TYPE_WRAPPER_COMPARE(type)(const TYPE_WRAPPER(type) *lLeft, const TYPE_WRAPPER(type) *rRight) { \
  if (!lLeft || !rRight || !lLeft->pointer || !rRight->pointer) return delay_exception2(PyExc_IndexError, ""); \
  if (lLeft->pointer == rRight->pointer) return 0; \
  return (lLeft->pointer < rRight->pointer)? -1 : 1; }


#define TYPE_BINDING(type) \
python_##type##_type

#define TYPE_BINDING_START(type, doc) \
static PyTypeObject TYPE_BINDING(type) = { \
  PyObject_HEAD_INIT(NULL) \
  tp_name: TYPE_PREFIX #type, \
  tp_basicsize: sizeof(TYPE_WRAPPER(type)), \
  tp_doc: doc

#define TYPE_BINDING_END(type) };


#define METHOD_BINDING_START(type, name, doc) \
const char python_##type##_##name##_doc[] = doc; \
static PyObject *python_##type##_##name##_function(python_##type *SELF, PyObject *ARGS, PyObject *KEYWORDS) {

#define METHOD_BINDING_END(type, name) }


#define METHOD_BINDING_LIST(type) \
python_##type##_methods

#define METHOD_BINDING_LIST_START(type) \
static PyMethodDef METHOD_BINDING_LIST(type)[] = {

#define METHOD_BINDING(type, name) \
{#name, (PyCFunction) python_##type##_##name##_function, METH_VARARGS, python_##type##_##name##_doc },

#define METHOD_BINDING_LIST_END(type) \
{NULL} };


#define GLOBAL_BINDING_START(name, doc) \
const char name##_doc[] = doc; \
static PyObject *name##_function(PyObject *SELF, PyObject *ARGS, PyObject *KEYWORDS) { \

#define GLOBAL_BINDING_END(name) } \
static PyMethodDef name##_binding = {#name, (PyCFunction) &name##_function, METH_KEYWORDS, name##_doc};


#define LOAD_GLOBAL_BINDING(name) \
if (!load_global_binding(MODULE, &name##_binding)) return 0;

#define LOAD_GLOBAL_TYPE(name) \
if (PyType_Ready(&python_##name##_type) < 0) return 0; \
Py_INCREF(&python_##name##_type); \
PyModule_AddObject(MODULE, #name, (PyObject*) &python_##name##_type);


#define NOT_IMPLEMENTED \
return not_implemented();

#define NO_ARGUMENTS \
if (!PyArg_ParseTuple(ARGS, "")) return NULL;

#define NO_RETURN \
return Py_BuildValue("");

#define STATIC_KEYWORDS(name) \
static char *name[]


#define LONG_CONSTANT(name) \
if (!load_long_constant(MODULE, #name, name)) return 0;

#ifdef __cplusplus
}
#endif

#endif /*python_macro_h*/