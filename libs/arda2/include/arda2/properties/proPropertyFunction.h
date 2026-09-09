/**
 *  function properties
 *
 *  @author Sean Riley
 *
 *  @date 1/13/2003
 *
 *  @file
**/

#ifndef __proPropertyFunction_h__
#define __proPropertyFunction_h__

#include "proProperty.h"

/**
 * function property - wraps a member function of a C++ class.
 * The interface to call function properties is Invoke(). This can be called with any
 * number and type of arguements - the calling code must ensure that these args match
 * the args expected by the wrapped member function.
 *
 * Caveats:
 *      == arguments to Invoke() must be passed as 32-bit values - either
 *         basic types by value, or pointers. Floating point values, although passed
 *         through registers on x86, are handled correctly.
 *
 *      == arguments passed to functions that expect a reference (string&) must
 *         be de-referenced by the calling function. 
 *
 *      == return value is an integer. even non-integer types will be copied into
 *         this integer, so they may be in the wrong binary format.
 *
**/
class proPropertyFunction : public proProperty
{
    PRO_DECLARE_OBJECT
public:
    proPropertyFunction() : proProperty(), m_func(NULL), m_returnFloat(false)  {}
    virtual ~proPropertyFunction() {}

    virtual const std::string &GetName() const = 0;
    virtual proClass*   GetDeclaringClass() const = 0;
    virtual void        Clone( const proObject* , proObject*  ) {};

    virtual void        Write( proObjectWriter* , const proObject*  ) const {}
    virtual void        Read( proObjectReader* , proObject*  ) {}

    virtual std::string      ToString( const proObject*  ) const;
    virtual void        FromString( proObject* , const std::string&  ) {}

    virtual proDataType GetDataType() const;

    uint GetNumArgs() { return (uint)m_args.size(); }
    void SetNumArgs(uint) {} 

    proDataType GetArgDataType(uint index) const;

    // interface for invoking the function on target
    virtual int Invoke(proObject *target, ...);

protected:

    virtual void SetAnnotation(const std::string &name, const std::string &value);

    void*               m_func;
    std::vector<proDataType> m_args;
    bool                m_returnFloat;
};

/**
 * native function property
 */
class proPropertyFunctionNative : public proPropertyFunction
{
    PRO_DECLARE_OBJECT
public:
    typedef proObject* MyType;

    virtual void Clone( const proObject* , proObject*  ){};

    virtual ~proPropertyFunctionNative() {};

protected:
};

/************* internal macro for function property macros **********/
#define REG_PROPERTY_FUN_INTERNAL(c, p, returnType)\
        m_func = Property_##c##_##p##Delegate;\
        proClass *cls = proClassRegistry::ClassForName<proClassNative*>(#c);\
        if (cls) { cls->AddProperty(this); }\
        m_isNative = true;\
        if ( _stricmp( #returnType ,"float") == 0)\
            m_returnFloat = true;\
    }\
    virtual const std::string &GetName() const { return s_name; }\
    virtual proClass* GetDeclaringClass() const { return proClassRegistry::ClassForName<proClass*>(#c); }\
    static const std::string s_name;\
 public:\
    virtual proClass *GetClass() const\
    {\
        if (m_impl)\
            return m_impl->GetClass();\
        return s_class;\
    }\
 protected:\
    static proClass *s_class;\
    friend class proClassNative;\
};\
Property_##c##_##p##Ref  regProperty_##c##_##p;\
const std::string Property_##c##_##p##Ref::s_name = #p;\
PRO_REGISTER_CLASS(Property_##c##_##p##Ref,proPropertyFunction)

/************* function property macro for 0 parameters **********/

#define REG_PROPERTY_FUNCTION0( c, p, func, returnType)\
returnType Property_##c##_##p##Delegate(proObject *obj)\
{\
	return ((c*)obj)->func();\
}\
class Property_##c##_##p##Ref : public proPropertyFunctionNative\
{\
public:\
    Property_##c##_##p##Ref() : proPropertyFunctionNative()\
    {\
    proClassRegistry::RegisterAnnotation(this,"NumArgs|0|ReturnType|" #returnType);\
        REG_PROPERTY_FUN_INTERNAL(c,p, returnType)

/************* function property macro for 1 parameters **********/

#define REG_PROPERTY_FUNCTION1( c, p, func, returnType, arg0Type )\
returnType Property_##c##_##p##Delegate(proObject *obj, arg0Type arg0)\
{\
	return ((c*)obj)->func(arg0);\
}\
class Property_##c##_##p##Ref : public proPropertyFunctionNative\
{\
public:\
    Property_##c##_##p##Ref()\
    {\
        proClassRegistry::RegisterAnnotation(this,"NumArgs|1|ReturnType|" #returnType "|Arg0|" #arg0Type);\
        REG_PROPERTY_FUN_INTERNAL(c,p, returnType)

/************* function property macro for 2 parameters **********/

#define REG_PROPERTY_FUNCTION2( c, p, func, returnType, arg0Type, arg1Type )\
returnType Property_##c##_##p##Delegate(proObject *obj, arg0Type arg0, arg1Type arg1)\
{\
	return ((c*)obj)->func(arg0, arg1);\
}\
class Property_##c##_##p##Ref : public proPropertyFunctionNative\
{\
public:\
    Property_##c##_##p##Ref()\
    {\
        proClassRegistry::RegisterAnnotation(this,"NumArgs|2|ReturnType|" #returnType "|Arg0|" #arg0Type "|Arg1|" #arg1Type);\
        REG_PROPERTY_FUN_INTERNAL(c,p, returnType)

/************* function property macro for 3 parameters **********/

#define REG_PROPERTY_FUNCTION3( c, p, func, returnType, arg0Type, arg1Type, arg2Type )\
returnType Property_##c##_##p##Delegate(proObject *obj, arg0Type arg0, arg1Type arg1, arg2Type arg2)\
{\
	return ((c*)obj)->func(arg0, arg1, arg2);\
}\
class Property_##c##_##p##Ref : public proPropertyFunctionNative\
{\
public:\
    Property_##c##_##p##Ref()\
    {\
        proClassRegistry::RegisterAnnotation(this,"NumArgs|3|ReturnType|" #returnType "|Arg0|" #arg0Type "|Arg1|" #arg1Type "|Arg2|" #arg2Type);\
        REG_PROPERTY_FUN_INTERNAL(c,p, returnType)

/************* function property macro for 4 parameters **********/

#define REG_PROPERTY_FUNCTION4( c, p, func, returnType, arg0Type, arg1Type, arg2Type, arg3Type )\
returnType Property_##c##_##p##Delegate(proObject *obj, arg0Type arg0, arg1Type arg1, arg2Type arg2, arg3Type arg3)\
{\
	return ((c*)obj)->func(arg0, arg1, arg2, arg3);\
}\
class Property_##c##_##p##Ref : public proPropertyFunctionNative\
{\
public:\
    Property_##c##_##p##Ref()\
    {\
        proClassRegistry::RegisterAnnotation(this,"NumArgs|4|ReturnType|" #returnType "|Arg0|" #arg0Type "|Arg1|" #arg1Type "|Arg2|" #arg2Type "|Arg3|" #arg3Type);\
        REG_PROPERTY_FUN_INTERNAL(c,p, returnType)
#endif
