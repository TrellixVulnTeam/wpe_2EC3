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
#include "JSTestSerializationInheritFinal.h"

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

JSC::EncodedJSValue JSC_HOST_CALL jsTestSerializationInheritFinalPrototypeFunctionToJSON(JSC::ExecState*);

// Attributes

JSC::EncodedJSValue jsTestSerializationInheritFinalFinalLongAttributeFoo(JSC::ExecState*, JSC::EncodedJSValue, JSC::PropertyName);
bool setJSTestSerializationInheritFinalFinalLongAttributeFoo(JSC::ExecState*, JSC::EncodedJSValue, JSC::EncodedJSValue);
JSC::EncodedJSValue jsTestSerializationInheritFinalFinalLongAttributeBar(JSC::ExecState*, JSC::EncodedJSValue, JSC::PropertyName);
bool setJSTestSerializationInheritFinalFinalLongAttributeBar(JSC::ExecState*, JSC::EncodedJSValue, JSC::EncodedJSValue);
JSC::EncodedJSValue jsTestSerializationInheritFinalConstructor(JSC::ExecState*, JSC::EncodedJSValue, JSC::PropertyName);
bool setJSTestSerializationInheritFinalConstructor(JSC::ExecState*, JSC::EncodedJSValue, JSC::EncodedJSValue);

class JSTestSerializationInheritFinalPrototype : public JSC::JSNonFinalObject {
public:
    using Base = JSC::JSNonFinalObject;
    static JSTestSerializationInheritFinalPrototype* create(JSC::VM& vm, JSDOMGlobalObject* globalObject, JSC::Structure* structure)
    {
        JSTestSerializationInheritFinalPrototype* ptr = new (NotNull, JSC::allocateCell<JSTestSerializationInheritFinalPrototype>(vm.heap)) JSTestSerializationInheritFinalPrototype(vm, globalObject, structure);
        ptr->finishCreation(vm);
        return ptr;
    }

    DECLARE_INFO;
    static JSC::Structure* createStructure(JSC::VM& vm, JSC::JSGlobalObject* globalObject, JSC::JSValue prototype)
    {
        return JSC::Structure::create(vm, globalObject, prototype, JSC::TypeInfo(JSC::ObjectType, StructureFlags), info());
    }

private:
    JSTestSerializationInheritFinalPrototype(JSC::VM& vm, JSC::JSGlobalObject*, JSC::Structure* structure)
        : JSC::JSNonFinalObject(vm, structure)
    {
    }

    void finishCreation(JSC::VM&);
};

using JSTestSerializationInheritFinalConstructor = JSDOMConstructorNotConstructable<JSTestSerializationInheritFinal>;

template<> JSValue JSTestSerializationInheritFinalConstructor::prototypeForStructure(JSC::VM& vm, const JSDOMGlobalObject& globalObject)
{
    return JSTestSerializationInherit::getConstructor(vm, &globalObject);
}

template<> void JSTestSerializationInheritFinalConstructor::initializeProperties(VM& vm, JSDOMGlobalObject& globalObject)
{
    putDirect(vm, vm.propertyNames->prototype, JSTestSerializationInheritFinal::prototype(vm, globalObject), DontDelete | ReadOnly | DontEnum);
    putDirect(vm, vm.propertyNames->name, jsNontrivialString(&vm, String(ASCIILiteral("TestSerializationInheritFinal"))), ReadOnly | DontEnum);
    putDirect(vm, vm.propertyNames->length, jsNumber(0), ReadOnly | DontEnum);
}

template<> const ClassInfo JSTestSerializationInheritFinalConstructor::s_info = { "TestSerializationInheritFinal", &Base::s_info, nullptr, nullptr, CREATE_METHOD_TABLE(JSTestSerializationInheritFinalConstructor) };

/* Hash table for prototype */

static const HashTableValue JSTestSerializationInheritFinalPrototypeTableValues[] =
{
    { "constructor", DontEnum, NoIntrinsic, { (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsTestSerializationInheritFinalConstructor), (intptr_t) static_cast<PutPropertySlot::PutValueFunc>(setJSTestSerializationInheritFinalConstructor) } },
    { "finalLongAttributeFoo", CustomAccessor, NoIntrinsic, { (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsTestSerializationInheritFinalFinalLongAttributeFoo), (intptr_t) static_cast<PutPropertySlot::PutValueFunc>(setJSTestSerializationInheritFinalFinalLongAttributeFoo) } },
    { "finalLongAttributeBar", CustomAccessor, NoIntrinsic, { (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsTestSerializationInheritFinalFinalLongAttributeBar), (intptr_t) static_cast<PutPropertySlot::PutValueFunc>(setJSTestSerializationInheritFinalFinalLongAttributeBar) } },
    { "toJSON", JSC::Function, NoIntrinsic, { (intptr_t)static_cast<NativeFunction>(jsTestSerializationInheritFinalPrototypeFunctionToJSON), (intptr_t) (0) } },
};

const ClassInfo JSTestSerializationInheritFinalPrototype::s_info = { "TestSerializationInheritFinalPrototype", &Base::s_info, nullptr, nullptr, CREATE_METHOD_TABLE(JSTestSerializationInheritFinalPrototype) };

void JSTestSerializationInheritFinalPrototype::finishCreation(VM& vm)
{
    Base::finishCreation(vm);
    reifyStaticProperties(vm, JSTestSerializationInheritFinalPrototypeTableValues, *this);
}

const ClassInfo JSTestSerializationInheritFinal::s_info = { "TestSerializationInheritFinal", &Base::s_info, nullptr, nullptr, CREATE_METHOD_TABLE(JSTestSerializationInheritFinal) };

JSTestSerializationInheritFinal::JSTestSerializationInheritFinal(Structure* structure, JSDOMGlobalObject& globalObject, Ref<TestSerializationInheritFinal>&& impl)
    : JSTestSerializationInherit(structure, globalObject, WTFMove(impl))
{
}

void JSTestSerializationInheritFinal::finishCreation(VM& vm)
{
    Base::finishCreation(vm);
    ASSERT(inherits(vm, info()));

}

JSObject* JSTestSerializationInheritFinal::createPrototype(VM& vm, JSDOMGlobalObject& globalObject)
{
    return JSTestSerializationInheritFinalPrototype::create(vm, &globalObject, JSTestSerializationInheritFinalPrototype::createStructure(vm, &globalObject, JSTestSerializationInherit::prototype(vm, globalObject)));
}

JSObject* JSTestSerializationInheritFinal::prototype(VM& vm, JSDOMGlobalObject& globalObject)
{
    return getDOMPrototype<JSTestSerializationInheritFinal>(vm, globalObject);
}

template<> inline JSTestSerializationInheritFinal* BindingCaller<JSTestSerializationInheritFinal>::castForAttribute(ExecState& state, EncodedJSValue thisValue)
{
    return jsDynamicDowncast<JSTestSerializationInheritFinal*>(state.vm(), JSValue::decode(thisValue));
}

template<> inline JSTestSerializationInheritFinal* BindingCaller<JSTestSerializationInheritFinal>::castForOperation(ExecState& state)
{
    return jsDynamicDowncast<JSTestSerializationInheritFinal*>(state.vm(), state.thisValue());
}

static inline JSValue jsTestSerializationInheritFinalFinalLongAttributeFooGetter(ExecState&, JSTestSerializationInheritFinal&, ThrowScope& throwScope);

EncodedJSValue jsTestSerializationInheritFinalFinalLongAttributeFoo(ExecState* state, EncodedJSValue thisValue, PropertyName)
{
    return BindingCaller<JSTestSerializationInheritFinal>::attribute<jsTestSerializationInheritFinalFinalLongAttributeFooGetter>(state, thisValue, "finalLongAttributeFoo");
}

static inline JSValue jsTestSerializationInheritFinalFinalLongAttributeFooGetter(ExecState& state, JSTestSerializationInheritFinal& thisObject, ThrowScope& throwScope)
{
    UNUSED_PARAM(throwScope);
    UNUSED_PARAM(state);
    auto& impl = thisObject.wrapped();
    JSValue result = toJS<IDLLong>(impl.finalLongAttributeFoo());
    return result;
}

static inline JSValue jsTestSerializationInheritFinalFinalLongAttributeBarGetter(ExecState&, JSTestSerializationInheritFinal&, ThrowScope& throwScope);

EncodedJSValue jsTestSerializationInheritFinalFinalLongAttributeBar(ExecState* state, EncodedJSValue thisValue, PropertyName)
{
    return BindingCaller<JSTestSerializationInheritFinal>::attribute<jsTestSerializationInheritFinalFinalLongAttributeBarGetter>(state, thisValue, "finalLongAttributeBar");
}

static inline JSValue jsTestSerializationInheritFinalFinalLongAttributeBarGetter(ExecState& state, JSTestSerializationInheritFinal& thisObject, ThrowScope& throwScope)
{
    UNUSED_PARAM(throwScope);
    UNUSED_PARAM(state);
    auto& impl = thisObject.wrapped();
    JSValue result = toJS<IDLLong>(impl.finalLongAttributeBar());
    return result;
}

EncodedJSValue jsTestSerializationInheritFinalConstructor(ExecState* state, EncodedJSValue thisValue, PropertyName)
{
    VM& vm = state->vm();
    auto throwScope = DECLARE_THROW_SCOPE(vm);
    JSTestSerializationInheritFinalPrototype* domObject = jsDynamicDowncast<JSTestSerializationInheritFinalPrototype*>(vm, JSValue::decode(thisValue));
    if (UNLIKELY(!domObject))
        return throwVMTypeError(state, throwScope);
    return JSValue::encode(JSTestSerializationInheritFinal::getConstructor(state->vm(), domObject->globalObject()));
}

bool setJSTestSerializationInheritFinalConstructor(ExecState* state, EncodedJSValue thisValue, EncodedJSValue encodedValue)
{
    VM& vm = state->vm();
    auto throwScope = DECLARE_THROW_SCOPE(vm);
    JSValue value = JSValue::decode(encodedValue);
    JSTestSerializationInheritFinalPrototype* domObject = jsDynamicDowncast<JSTestSerializationInheritFinalPrototype*>(vm, JSValue::decode(thisValue));
    if (UNLIKELY(!domObject)) {
        throwVMTypeError(state, throwScope);
        return false;
    }
    // Shadowing a built-in constructor
    return domObject->putDirect(state->vm(), state->propertyNames().constructor, value);
}

static inline bool setJSTestSerializationInheritFinalFinalLongAttributeFooFunction(ExecState&, JSTestSerializationInheritFinal&, JSValue, ThrowScope&);

bool setJSTestSerializationInheritFinalFinalLongAttributeFoo(ExecState* state, EncodedJSValue thisValue, EncodedJSValue encodedValue)
{
    return BindingCaller<JSTestSerializationInheritFinal>::setAttribute<setJSTestSerializationInheritFinalFinalLongAttributeFooFunction>(state, thisValue, encodedValue, "finalLongAttributeFoo");
}

static inline bool setJSTestSerializationInheritFinalFinalLongAttributeFooFunction(ExecState& state, JSTestSerializationInheritFinal& thisObject, JSValue value, ThrowScope& throwScope)
{
    UNUSED_PARAM(state);
    UNUSED_PARAM(throwScope);
    auto& impl = thisObject.wrapped();
    auto nativeValue = convert<IDLLong>(state, value);
    RETURN_IF_EXCEPTION(throwScope, false);
    impl.setFinalLongAttributeFoo(WTFMove(nativeValue));
    return true;
}


static inline bool setJSTestSerializationInheritFinalFinalLongAttributeBarFunction(ExecState&, JSTestSerializationInheritFinal&, JSValue, ThrowScope&);

bool setJSTestSerializationInheritFinalFinalLongAttributeBar(ExecState* state, EncodedJSValue thisValue, EncodedJSValue encodedValue)
{
    return BindingCaller<JSTestSerializationInheritFinal>::setAttribute<setJSTestSerializationInheritFinalFinalLongAttributeBarFunction>(state, thisValue, encodedValue, "finalLongAttributeBar");
}

static inline bool setJSTestSerializationInheritFinalFinalLongAttributeBarFunction(ExecState& state, JSTestSerializationInheritFinal& thisObject, JSValue value, ThrowScope& throwScope)
{
    UNUSED_PARAM(state);
    UNUSED_PARAM(throwScope);
    auto& impl = thisObject.wrapped();
    auto nativeValue = convert<IDLLong>(state, value);
    RETURN_IF_EXCEPTION(throwScope, false);
    impl.setFinalLongAttributeBar(WTFMove(nativeValue));
    return true;
}


JSValue JSTestSerializationInheritFinal::getConstructor(VM& vm, const JSGlobalObject* globalObject)
{
    return getDOMConstructor<JSTestSerializationInheritFinalConstructor>(vm, *jsCast<const JSDOMGlobalObject*>(globalObject));
}

JSC::JSObject* JSTestSerializationInheritFinal::serialize(ExecState* state, JSTestSerializationInheritFinal* thisObject, ThrowScope& throwScope)
{
    auto& vm = state->vm();
    auto* result = JSTestSerializationInherit::serialize(state, thisObject, throwScope);

    auto finalLongAttributeBarValue = jsTestSerializationInheritFinalFinalLongAttributeBarGetter(*state, *thisObject, throwScope);
    throwScope.assertNoException();
    result->putDirect(vm, Identifier::fromString(&vm, "finalLongAttributeBar"), finalLongAttributeBarValue);

    return result;
}

static inline EncodedJSValue jsTestSerializationInheritFinalPrototypeFunctionToJSONCaller(ExecState* state, JSTestSerializationInheritFinal* thisObject, JSC::ThrowScope& throwScope)
{
    return JSValue::encode(JSTestSerializationInheritFinal::serialize(state, thisObject, throwScope));
}

EncodedJSValue JSC_HOST_CALL jsTestSerializationInheritFinalPrototypeFunctionToJSON(ExecState* state)
{
    return BindingCaller<JSTestSerializationInheritFinal>::callOperation<jsTestSerializationInheritFinalPrototypeFunctionToJSONCaller>(state, "toJSON");
}


}