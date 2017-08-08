/*
    This file is part of the WebKit open source project.
    This file has been generated by generate-bindings.pl. DO NOT MODIFY!

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include "config.h"
#include "JSTestSerializationInherit.h"

#include "JSDOMBinding.h"
#include "JSDOMBindingCaller.h"
#include "JSDOMConstructorNotConstructable.h"
#include "JSDOMConvert.h"
#include "JSDOMExceptionHandling.h"
#include "JSDOMWrapperCache.h"
#include <runtime/ObjectConstructor.h>
#include <wtf/GetPtr.h>

using namespace JSC;

namespace WebCore {

// Functions

JSC::EncodedJSValue JSC_HOST_CALL jsTestSerializationInheritPrototypeFunctionToJSON(JSC::ExecState*);

// Attributes

JSC::EncodedJSValue jsTestSerializationInheritInheritLongAttribute(JSC::ExecState*, JSC::EncodedJSValue, JSC::PropertyName);
bool setJSTestSerializationInheritInheritLongAttribute(JSC::ExecState*, JSC::EncodedJSValue, JSC::EncodedJSValue);
JSC::EncodedJSValue jsTestSerializationInheritConstructor(JSC::ExecState*, JSC::EncodedJSValue, JSC::PropertyName);
bool setJSTestSerializationInheritConstructor(JSC::ExecState*, JSC::EncodedJSValue, JSC::EncodedJSValue);

class JSTestSerializationInheritPrototype : public JSC::JSNonFinalObject {
public:
    using Base = JSC::JSNonFinalObject;
    static JSTestSerializationInheritPrototype* create(JSC::VM& vm, JSDOMGlobalObject* globalObject, JSC::Structure* structure)
    {
        JSTestSerializationInheritPrototype* ptr = new (NotNull, JSC::allocateCell<JSTestSerializationInheritPrototype>(vm.heap)) JSTestSerializationInheritPrototype(vm, globalObject, structure);
        ptr->finishCreation(vm);
        return ptr;
    }

    DECLARE_INFO;
    static JSC::Structure* createStructure(JSC::VM& vm, JSC::JSGlobalObject* globalObject, JSC::JSValue prototype)
    {
        return JSC::Structure::create(vm, globalObject, prototype, JSC::TypeInfo(JSC::ObjectType, StructureFlags), info());
    }

private:
    JSTestSerializationInheritPrototype(JSC::VM& vm, JSC::JSGlobalObject*, JSC::Structure* structure)
        : JSC::JSNonFinalObject(vm, structure)
    {
    }

    void finishCreation(JSC::VM&);
};

using JSTestSerializationInheritConstructor = JSDOMConstructorNotConstructable<JSTestSerializationInherit>;

template<> JSValue JSTestSerializationInheritConstructor::prototypeForStructure(JSC::VM& vm, const JSDOMGlobalObject& globalObject)
{
    return JSTestSerialization::getConstructor(vm, &globalObject);
}

template<> void JSTestSerializationInheritConstructor::initializeProperties(VM& vm, JSDOMGlobalObject& globalObject)
{
    putDirect(vm, vm.propertyNames->prototype, JSTestSerializationInherit::prototype(vm, globalObject), DontDelete | ReadOnly | DontEnum);
    putDirect(vm, vm.propertyNames->name, jsNontrivialString(&vm, String(ASCIILiteral("TestSerializationInherit"))), ReadOnly | DontEnum);
    putDirect(vm, vm.propertyNames->length, jsNumber(0), ReadOnly | DontEnum);
}

template<> const ClassInfo JSTestSerializationInheritConstructor::s_info = { "TestSerializationInherit", &Base::s_info, nullptr, nullptr, CREATE_METHOD_TABLE(JSTestSerializationInheritConstructor) };

/* Hash table for prototype */

static const HashTableValue JSTestSerializationInheritPrototypeTableValues[] =
{
    { "constructor", DontEnum, NoIntrinsic, { (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsTestSerializationInheritConstructor), (intptr_t) static_cast<PutPropertySlot::PutValueFunc>(setJSTestSerializationInheritConstructor) } },
    { "inheritLongAttribute", CustomAccessor, NoIntrinsic, { (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsTestSerializationInheritInheritLongAttribute), (intptr_t) static_cast<PutPropertySlot::PutValueFunc>(setJSTestSerializationInheritInheritLongAttribute) } },
    { "toJSON", JSC::Function, NoIntrinsic, { (intptr_t)static_cast<NativeFunction>(jsTestSerializationInheritPrototypeFunctionToJSON), (intptr_t) (0) } },
};

const ClassInfo JSTestSerializationInheritPrototype::s_info = { "TestSerializationInheritPrototype", &Base::s_info, nullptr, nullptr, CREATE_METHOD_TABLE(JSTestSerializationInheritPrototype) };

void JSTestSerializationInheritPrototype::finishCreation(VM& vm)
{
    Base::finishCreation(vm);
    reifyStaticProperties(vm, JSTestSerializationInheritPrototypeTableValues, *this);
}

const ClassInfo JSTestSerializationInherit::s_info = { "TestSerializationInherit", &Base::s_info, nullptr, nullptr, CREATE_METHOD_TABLE(JSTestSerializationInherit) };

JSTestSerializationInherit::JSTestSerializationInherit(Structure* structure, JSDOMGlobalObject& globalObject, Ref<TestSerializationInherit>&& impl)
    : JSTestSerialization(structure, globalObject, WTFMove(impl))
{
}

void JSTestSerializationInherit::finishCreation(VM& vm)
{
    Base::finishCreation(vm);
    ASSERT(inherits(vm, info()));

}

JSObject* JSTestSerializationInherit::createPrototype(VM& vm, JSDOMGlobalObject& globalObject)
{
    return JSTestSerializationInheritPrototype::create(vm, &globalObject, JSTestSerializationInheritPrototype::createStructure(vm, &globalObject, JSTestSerialization::prototype(vm, globalObject)));
}

JSObject* JSTestSerializationInherit::prototype(VM& vm, JSDOMGlobalObject& globalObject)
{
    return getDOMPrototype<JSTestSerializationInherit>(vm, globalObject);
}

template<> inline JSTestSerializationInherit* BindingCaller<JSTestSerializationInherit>::castForAttribute(ExecState& state, EncodedJSValue thisValue)
{
    return jsDynamicDowncast<JSTestSerializationInherit*>(state.vm(), JSValue::decode(thisValue));
}

template<> inline JSTestSerializationInherit* BindingCaller<JSTestSerializationInherit>::castForOperation(ExecState& state)
{
    return jsDynamicDowncast<JSTestSerializationInherit*>(state.vm(), state.thisValue());
}

static inline JSValue jsTestSerializationInheritInheritLongAttributeGetter(ExecState&, JSTestSerializationInherit&, ThrowScope& throwScope);

EncodedJSValue jsTestSerializationInheritInheritLongAttribute(ExecState* state, EncodedJSValue thisValue, PropertyName)
{
    return BindingCaller<JSTestSerializationInherit>::attribute<jsTestSerializationInheritInheritLongAttributeGetter>(state, thisValue, "inheritLongAttribute");
}

static inline JSValue jsTestSerializationInheritInheritLongAttributeGetter(ExecState& state, JSTestSerializationInherit& thisObject, ThrowScope& throwScope)
{
    UNUSED_PARAM(throwScope);
    UNUSED_PARAM(state);
    auto& impl = thisObject.wrapped();
    JSValue result = toJS<IDLLong>(impl.inheritLongAttribute());
    return result;
}

EncodedJSValue jsTestSerializationInheritConstructor(ExecState* state, EncodedJSValue thisValue, PropertyName)
{
    VM& vm = state->vm();
    auto throwScope = DECLARE_THROW_SCOPE(vm);
    JSTestSerializationInheritPrototype* domObject = jsDynamicDowncast<JSTestSerializationInheritPrototype*>(vm, JSValue::decode(thisValue));
    if (UNLIKELY(!domObject))
        return throwVMTypeError(state, throwScope);
    return JSValue::encode(JSTestSerializationInherit::getConstructor(state->vm(), domObject->globalObject()));
}

bool setJSTestSerializationInheritConstructor(ExecState* state, EncodedJSValue thisValue, EncodedJSValue encodedValue)
{
    VM& vm = state->vm();
    auto throwScope = DECLARE_THROW_SCOPE(vm);
    JSValue value = JSValue::decode(encodedValue);
    JSTestSerializationInheritPrototype* domObject = jsDynamicDowncast<JSTestSerializationInheritPrototype*>(vm, JSValue::decode(thisValue));
    if (UNLIKELY(!domObject)) {
        throwVMTypeError(state, throwScope);
        return false;
    }
    // Shadowing a built-in constructor
    return domObject->putDirect(state->vm(), state->propertyNames().constructor, value);
}

static inline bool setJSTestSerializationInheritInheritLongAttributeFunction(ExecState&, JSTestSerializationInherit&, JSValue, ThrowScope&);

bool setJSTestSerializationInheritInheritLongAttribute(ExecState* state, EncodedJSValue thisValue, EncodedJSValue encodedValue)
{
    return BindingCaller<JSTestSerializationInherit>::setAttribute<setJSTestSerializationInheritInheritLongAttributeFunction>(state, thisValue, encodedValue, "inheritLongAttribute");
}

static inline bool setJSTestSerializationInheritInheritLongAttributeFunction(ExecState& state, JSTestSerializationInherit& thisObject, JSValue value, ThrowScope& throwScope)
{
    UNUSED_PARAM(state);
    UNUSED_PARAM(throwScope);
    auto& impl = thisObject.wrapped();
    auto nativeValue = convert<IDLLong>(state, value);
    RETURN_IF_EXCEPTION(throwScope, false);
    impl.setInheritLongAttribute(WTFMove(nativeValue));
    return true;
}


JSValue JSTestSerializationInherit::getConstructor(VM& vm, const JSGlobalObject* globalObject)
{
    return getDOMConstructor<JSTestSerializationInheritConstructor>(vm, *jsCast<const JSDOMGlobalObject*>(globalObject));
}

JSC::JSObject* JSTestSerializationInherit::serialize(ExecState* state, JSTestSerializationInherit* thisObject, ThrowScope& throwScope)
{
    auto& vm = state->vm();
    auto* result = JSTestSerialization::serialize(state, thisObject, throwScope);

    auto inheritLongAttributeValue = jsTestSerializationInheritInheritLongAttributeGetter(*state, *thisObject, throwScope);
    throwScope.assertNoException();
    result->putDirect(vm, Identifier::fromString(&vm, "inheritLongAttribute"), inheritLongAttributeValue);

    return result;
}

static inline EncodedJSValue jsTestSerializationInheritPrototypeFunctionToJSONCaller(ExecState* state, JSTestSerializationInherit* thisObject, JSC::ThrowScope& throwScope)
{
    return JSValue::encode(JSTestSerializationInherit::serialize(state, thisObject, throwScope));
}

EncodedJSValue JSC_HOST_CALL jsTestSerializationInheritPrototypeFunctionToJSON(ExecState* state)
{
    return BindingCaller<JSTestSerializationInherit>::callOperation<jsTestSerializationInheritPrototypeFunctionToJSONCaller>(state, "toJSON");
}


}
