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
#include "JSTestGlobalObject.h"

#include "JSDOMBinding.h"
#include "JSDOMBindingCaller.h"
#include "JSDOMConstructorNotConstructable.h"
#include "JSDOMConvert.h"
#include "JSDOMExceptionHandling.h"
#include "JSDOMWrapperCache.h"
#include "RuntimeEnabledFeatures.h"
#include "WebCoreJSClientData.h"
#include <runtime/Error.h>
#include <runtime/FunctionPrototype.h>
#include <wtf/GetPtr.h>

#if ENABLE(TEST_FEATURE)
#include "TestGlobalObjectBuiltins.h"
#endif

using namespace JSC;

namespace WebCore {

// Functions

JSC::EncodedJSValue JSC_HOST_CALL jsTestGlobalObjectInstanceFunctionRegularOperation(JSC::ExecState*);
#if ENABLE(TEST_FEATURE)
JSC::EncodedJSValue JSC_HOST_CALL jsTestGlobalObjectInstanceFunctionEnabledAtRuntimeOperation(JSC::ExecState*);
#endif
JSC::EncodedJSValue JSC_HOST_CALL jsTestGlobalObjectInstanceFunctionEnabledInSpecificWorld(JSC::ExecState*);
#if ENABLE(TEST_FEATURE)
JSC::EncodedJSValue JSC_HOST_CALL jsTestGlobalObjectInstanceFunctionTestPrivateFunction(JSC::ExecState*);
#endif

// Attributes

JSC::EncodedJSValue jsTestGlobalObjectRegularAttribute(JSC::ExecState*, JSC::EncodedJSValue, JSC::PropertyName);
bool setJSTestGlobalObjectRegularAttribute(JSC::ExecState*, JSC::EncodedJSValue, JSC::EncodedJSValue);
JSC::EncodedJSValue jsTestGlobalObjectPublicAndPrivateAttribute(JSC::ExecState*, JSC::EncodedJSValue, JSC::PropertyName);
bool setJSTestGlobalObjectPublicAndPrivateAttribute(JSC::ExecState*, JSC::EncodedJSValue, JSC::EncodedJSValue);
#if ENABLE(TEST_FEATURE)
JSC::EncodedJSValue jsTestGlobalObjectPublicAndPrivateConditionalAttribute(JSC::ExecState*, JSC::EncodedJSValue, JSC::PropertyName);
bool setJSTestGlobalObjectPublicAndPrivateConditionalAttribute(JSC::ExecState*, JSC::EncodedJSValue, JSC::EncodedJSValue);
#endif
#if ENABLE(TEST_FEATURE)
JSC::EncodedJSValue jsTestGlobalObjectEnabledAtRuntimeAttribute(JSC::ExecState*, JSC::EncodedJSValue, JSC::PropertyName);
bool setJSTestGlobalObjectEnabledAtRuntimeAttribute(JSC::ExecState*, JSC::EncodedJSValue, JSC::EncodedJSValue);
#endif
JSC::EncodedJSValue jsTestGlobalObjectConstructor(JSC::ExecState*, JSC::EncodedJSValue, JSC::PropertyName);
bool setJSTestGlobalObjectConstructor(JSC::ExecState*, JSC::EncodedJSValue, JSC::EncodedJSValue);

using JSTestGlobalObjectConstructor = JSDOMConstructorNotConstructable<JSTestGlobalObject>;

/* Hash table */

static const struct CompactHashIndex JSTestGlobalObjectTableIndex[16] = {
    { -1, -1 },
    { -1, -1 },
    { 2, -1 },
    { 0, -1 },
    { -1, -1 },
    { -1, -1 },
    { -1, -1 },
    { -1, -1 },
    { -1, -1 },
    { 3, -1 },
    { -1, -1 },
    { -1, -1 },
    { -1, -1 },
    { -1, -1 },
    { 1, -1 },
    { -1, -1 },
};


static const HashTableValue JSTestGlobalObjectTableValues[] =
{
    { "regularAttribute", CustomAccessor, NoIntrinsic, { (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsTestGlobalObjectRegularAttribute), (intptr_t) static_cast<PutPropertySlot::PutValueFunc>(setJSTestGlobalObjectRegularAttribute) } },
    { "publicAndPrivateAttribute", CustomAccessor, NoIntrinsic, { (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsTestGlobalObjectPublicAndPrivateAttribute), (intptr_t) static_cast<PutPropertySlot::PutValueFunc>(setJSTestGlobalObjectPublicAndPrivateAttribute) } },
#if ENABLE(TEST_FEATURE)
    { "publicAndPrivateConditionalAttribute", CustomAccessor, NoIntrinsic, { (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsTestGlobalObjectPublicAndPrivateConditionalAttribute), (intptr_t) static_cast<PutPropertySlot::PutValueFunc>(setJSTestGlobalObjectPublicAndPrivateConditionalAttribute) } },
#else
    { 0, 0, NoIntrinsic, { 0, 0 } },
#endif
    { "regularOperation", JSC::Function, NoIntrinsic, { (intptr_t)static_cast<NativeFunction>(jsTestGlobalObjectInstanceFunctionRegularOperation), (intptr_t) (1) } },
};

static const HashTable JSTestGlobalObjectTable = { 4, 15, true, JSTestGlobalObjectTableValues, JSTestGlobalObjectTableIndex };
template<> JSValue JSTestGlobalObjectConstructor::prototypeForStructure(JSC::VM& vm, const JSDOMGlobalObject& globalObject)
{
    UNUSED_PARAM(vm);
    return globalObject.functionPrototype();
}

template<> void JSTestGlobalObjectConstructor::initializeProperties(VM& vm, JSDOMGlobalObject& globalObject)
{
    putDirect(vm, vm.propertyNames->prototype, globalObject.getPrototypeDirect(), DontDelete | ReadOnly | DontEnum);
    putDirect(vm, vm.propertyNames->name, jsNontrivialString(&vm, String(ASCIILiteral("TestGlobalObject"))), ReadOnly | DontEnum);
    putDirect(vm, vm.propertyNames->length, jsNumber(0), ReadOnly | DontEnum);
}

template<> const ClassInfo JSTestGlobalObjectConstructor::s_info = { "TestGlobalObject", &Base::s_info, nullptr, nullptr, CREATE_METHOD_TABLE(JSTestGlobalObjectConstructor) };

/* Hash table for prototype */

static const struct CompactHashIndex JSTestGlobalObjectPrototypeTableIndex[2] = {
    { -1, -1 },
    { 0, -1 },
};


static const HashTableValue JSTestGlobalObjectPrototypeTableValues[] =
{
    { "constructor", DontEnum, NoIntrinsic, { (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsTestGlobalObjectConstructor), (intptr_t) static_cast<PutPropertySlot::PutValueFunc>(setJSTestGlobalObjectConstructor) } },
};

static const HashTable JSTestGlobalObjectPrototypeTable = { 1, 1, true, JSTestGlobalObjectPrototypeTableValues, JSTestGlobalObjectPrototypeTableIndex };
const ClassInfo JSTestGlobalObjectPrototype::s_info = { "TestGlobalObjectPrototype", &Base::s_info, &JSTestGlobalObjectPrototypeTable, nullptr, CREATE_METHOD_TABLE(JSTestGlobalObjectPrototype) };

const ClassInfo JSTestGlobalObject::s_info = { "TestGlobalObject", &Base::s_info, &JSTestGlobalObjectTable, nullptr, CREATE_METHOD_TABLE(JSTestGlobalObject) };

JSTestGlobalObject::JSTestGlobalObject(Structure* structure, JSDOMGlobalObject& globalObject, Ref<TestGlobalObject>&& impl)
    : JSDOMWrapper<TestGlobalObject>(structure, globalObject, WTFMove(impl))
{
}

void JSTestGlobalObject::finishCreation(VM& vm)
{
    Base::finishCreation(vm);
    ASSERT(inherits(vm, info()));

#if ENABLE(TEST_FEATURE)
    if (RuntimeEnabledFeatures::sharedFeatures().testFeatureEnabled()) {
        auto* customGetterSetter = CustomGetterSetter::create(vm, jsTestGlobalObjectEnabledAtRuntimeAttribute, setJSTestGlobalObjectEnabledAtRuntimeAttribute);
        putDirectCustomAccessor(vm, vm.propertyNames->enabledAtRuntimeAttribute, customGetterSetter, attributesForStructure(CustomAccessor));
    }
#endif
    putDirectCustomAccessor(vm, static_cast<JSVMClientData*>(vm.clientData)->builtinNames().publicAndPrivateAttributePrivateName(), CustomGetterSetter::create(vm, jsTestGlobalObjectPublicAndPrivateAttribute, nullptr), attributesForStructure(DontDelete | ReadOnly));
#if ENABLE(TEST_FEATURE)
    putDirectCustomAccessor(vm, static_cast<JSVMClientData*>(vm.clientData)->builtinNames().publicAndPrivateConditionalAttributePrivateName(), CustomGetterSetter::create(vm, jsTestGlobalObjectPublicAndPrivateConditionalAttribute, nullptr), attributesForStructure(DontDelete | ReadOnly));
#endif
#if ENABLE(TEST_FEATURE)
    if (RuntimeEnabledFeatures::sharedFeatures().testFeatureEnabled())
        putDirectNativeFunction(vm, this, vm.propertyNames->enabledAtRuntimeOperation, 1, jsTestGlobalObjectInstanceFunctionEnabledAtRuntimeOperation, NoIntrinsic, attributesForStructure(JSC::Function));
#endif
    if (worldForDOMObject(this).specificWorld())
        putDirectNativeFunction(vm, this, vm.propertyNames->enabledInSpecificWorld, 1, jsTestGlobalObjectInstanceFunctionEnabledInSpecificWorld, NoIntrinsic, attributesForStructure(JSC::Function));
#if ENABLE(TEST_FEATURE)
    if (RuntimeEnabledFeatures::sharedFeatures().testFeatureEnabled())
        putDirectNativeFunction(vm, this, static_cast<JSVMClientData*>(vm.clientData)->builtinNames().testPrivateFunctionPrivateName(), 0, jsTestGlobalObjectInstanceFunctionTestPrivateFunction, NoIntrinsic, attributesForStructure(JSC::Function));
#endif
#if ENABLE(TEST_FEATURE)
    if (RuntimeEnabledFeatures::sharedFeatures().testFeatureEnabled())
        putDirectBuiltinFunction(vm, this, vm.propertyNames->testJSBuiltinFunction, testGlobalObjectTestJSBuiltinFunctionCodeGenerator(vm), attributesForStructure(JSC::Builtin));
#endif
}

void JSTestGlobalObject::destroy(JSC::JSCell* cell)
{
    JSTestGlobalObject* thisObject = static_cast<JSTestGlobalObject*>(cell);
    thisObject->JSTestGlobalObject::~JSTestGlobalObject();
}

template<> inline JSTestGlobalObject* BindingCaller<JSTestGlobalObject>::castForAttribute(ExecState& state, EncodedJSValue thisValue)
{
    return jsDynamicDowncast<JSTestGlobalObject*>(state.vm(), JSValue::decode(thisValue));
}

template<> inline JSTestGlobalObject* BindingCaller<JSTestGlobalObject>::castForOperation(ExecState& state)
{
    return jsDynamicDowncast<JSTestGlobalObject*>(state.vm(), state.thisValue());
}

static inline JSValue jsTestGlobalObjectRegularAttributeGetter(ExecState&, JSTestGlobalObject&, ThrowScope& throwScope);

EncodedJSValue jsTestGlobalObjectRegularAttribute(ExecState* state, EncodedJSValue thisValue, PropertyName)
{
    return BindingCaller<JSTestGlobalObject>::attribute<jsTestGlobalObjectRegularAttributeGetter>(state, thisValue, "regularAttribute");
}

static inline JSValue jsTestGlobalObjectRegularAttributeGetter(ExecState& state, JSTestGlobalObject& thisObject, ThrowScope& throwScope)
{
    UNUSED_PARAM(throwScope);
    UNUSED_PARAM(state);
    auto& impl = thisObject.wrapped();
    JSValue result = toJS<IDLDOMString>(state, impl.regularAttribute());
    return result;
}

static inline JSValue jsTestGlobalObjectPublicAndPrivateAttributeGetter(ExecState&, JSTestGlobalObject&, ThrowScope& throwScope);

EncodedJSValue jsTestGlobalObjectPublicAndPrivateAttribute(ExecState* state, EncodedJSValue thisValue, PropertyName)
{
    return BindingCaller<JSTestGlobalObject>::attribute<jsTestGlobalObjectPublicAndPrivateAttributeGetter>(state, thisValue, "publicAndPrivateAttribute");
}

static inline JSValue jsTestGlobalObjectPublicAndPrivateAttributeGetter(ExecState& state, JSTestGlobalObject& thisObject, ThrowScope& throwScope)
{
    UNUSED_PARAM(throwScope);
    UNUSED_PARAM(state);
    auto& impl = thisObject.wrapped();
    JSValue result = toJS<IDLDOMString>(state, impl.publicAndPrivateAttribute());
    return result;
}

#if ENABLE(TEST_FEATURE)
static inline JSValue jsTestGlobalObjectPublicAndPrivateConditionalAttributeGetter(ExecState&, JSTestGlobalObject&, ThrowScope& throwScope);

EncodedJSValue jsTestGlobalObjectPublicAndPrivateConditionalAttribute(ExecState* state, EncodedJSValue thisValue, PropertyName)
{
    return BindingCaller<JSTestGlobalObject>::attribute<jsTestGlobalObjectPublicAndPrivateConditionalAttributeGetter>(state, thisValue, "publicAndPrivateConditionalAttribute");
}

static inline JSValue jsTestGlobalObjectPublicAndPrivateConditionalAttributeGetter(ExecState& state, JSTestGlobalObject& thisObject, ThrowScope& throwScope)
{
    UNUSED_PARAM(throwScope);
    UNUSED_PARAM(state);
    auto& impl = thisObject.wrapped();
    JSValue result = toJS<IDLDOMString>(state, impl.publicAndPrivateConditionalAttribute());
    return result;
}

#endif

#if ENABLE(TEST_FEATURE)
static inline JSValue jsTestGlobalObjectEnabledAtRuntimeAttributeGetter(ExecState&, JSTestGlobalObject&, ThrowScope& throwScope);

EncodedJSValue jsTestGlobalObjectEnabledAtRuntimeAttribute(ExecState* state, EncodedJSValue thisValue, PropertyName)
{
    return BindingCaller<JSTestGlobalObject>::attribute<jsTestGlobalObjectEnabledAtRuntimeAttributeGetter>(state, thisValue, "enabledAtRuntimeAttribute");
}

static inline JSValue jsTestGlobalObjectEnabledAtRuntimeAttributeGetter(ExecState& state, JSTestGlobalObject& thisObject, ThrowScope& throwScope)
{
    UNUSED_PARAM(throwScope);
    UNUSED_PARAM(state);
    auto& impl = thisObject.wrapped();
    JSValue result = toJS<IDLDOMString>(state, impl.enabledAtRuntimeAttribute());
    return result;
}

#endif

EncodedJSValue jsTestGlobalObjectConstructor(ExecState* state, EncodedJSValue thisValue, PropertyName)
{
    VM& vm = state->vm();
    auto throwScope = DECLARE_THROW_SCOPE(vm);
    JSTestGlobalObjectPrototype* domObject = jsDynamicDowncast<JSTestGlobalObjectPrototype*>(vm, JSValue::decode(thisValue));
    if (UNLIKELY(!domObject))
        return throwVMTypeError(state, throwScope);
    return JSValue::encode(JSTestGlobalObject::getConstructor(state->vm(), domObject->globalObject()));
}

bool setJSTestGlobalObjectConstructor(ExecState* state, EncodedJSValue thisValue, EncodedJSValue encodedValue)
{
    VM& vm = state->vm();
    auto throwScope = DECLARE_THROW_SCOPE(vm);
    JSValue value = JSValue::decode(encodedValue);
    JSTestGlobalObjectPrototype* domObject = jsDynamicDowncast<JSTestGlobalObjectPrototype*>(vm, JSValue::decode(thisValue));
    if (UNLIKELY(!domObject)) {
        throwVMTypeError(state, throwScope);
        return false;
    }
    // Shadowing a built-in constructor
    return domObject->putDirect(state->vm(), state->propertyNames().constructor, value);
}

static inline bool setJSTestGlobalObjectRegularAttributeFunction(ExecState&, JSTestGlobalObject&, JSValue, ThrowScope&);

bool setJSTestGlobalObjectRegularAttribute(ExecState* state, EncodedJSValue thisValue, EncodedJSValue encodedValue)
{
    return BindingCaller<JSTestGlobalObject>::setAttribute<setJSTestGlobalObjectRegularAttributeFunction>(state, thisValue, encodedValue, "regularAttribute");
}

static inline bool setJSTestGlobalObjectRegularAttributeFunction(ExecState& state, JSTestGlobalObject& thisObject, JSValue value, ThrowScope& throwScope)
{
    UNUSED_PARAM(state);
    UNUSED_PARAM(throwScope);
    auto& impl = thisObject.wrapped();
    auto nativeValue = convert<IDLDOMString>(state, value);
    RETURN_IF_EXCEPTION(throwScope, false);
    impl.setRegularAttribute(WTFMove(nativeValue));
    return true;
}


static inline bool setJSTestGlobalObjectPublicAndPrivateAttributeFunction(ExecState&, JSTestGlobalObject&, JSValue, ThrowScope&);

bool setJSTestGlobalObjectPublicAndPrivateAttribute(ExecState* state, EncodedJSValue thisValue, EncodedJSValue encodedValue)
{
    return BindingCaller<JSTestGlobalObject>::setAttribute<setJSTestGlobalObjectPublicAndPrivateAttributeFunction>(state, thisValue, encodedValue, "publicAndPrivateAttribute");
}

static inline bool setJSTestGlobalObjectPublicAndPrivateAttributeFunction(ExecState& state, JSTestGlobalObject& thisObject, JSValue value, ThrowScope& throwScope)
{
    UNUSED_PARAM(state);
    UNUSED_PARAM(throwScope);
    auto& impl = thisObject.wrapped();
    auto nativeValue = convert<IDLDOMString>(state, value);
    RETURN_IF_EXCEPTION(throwScope, false);
    impl.setPublicAndPrivateAttribute(WTFMove(nativeValue));
    return true;
}


#if ENABLE(TEST_FEATURE)
static inline bool setJSTestGlobalObjectPublicAndPrivateConditionalAttributeFunction(ExecState&, JSTestGlobalObject&, JSValue, ThrowScope&);

bool setJSTestGlobalObjectPublicAndPrivateConditionalAttribute(ExecState* state, EncodedJSValue thisValue, EncodedJSValue encodedValue)
{
    return BindingCaller<JSTestGlobalObject>::setAttribute<setJSTestGlobalObjectPublicAndPrivateConditionalAttributeFunction>(state, thisValue, encodedValue, "publicAndPrivateConditionalAttribute");
}

static inline bool setJSTestGlobalObjectPublicAndPrivateConditionalAttributeFunction(ExecState& state, JSTestGlobalObject& thisObject, JSValue value, ThrowScope& throwScope)
{
    UNUSED_PARAM(state);
    UNUSED_PARAM(throwScope);
    auto& impl = thisObject.wrapped();
    auto nativeValue = convert<IDLDOMString>(state, value);
    RETURN_IF_EXCEPTION(throwScope, false);
    impl.setPublicAndPrivateConditionalAttribute(WTFMove(nativeValue));
    return true;
}

#endif

#if ENABLE(TEST_FEATURE)
static inline bool setJSTestGlobalObjectEnabledAtRuntimeAttributeFunction(ExecState&, JSTestGlobalObject&, JSValue, ThrowScope&);

bool setJSTestGlobalObjectEnabledAtRuntimeAttribute(ExecState* state, EncodedJSValue thisValue, EncodedJSValue encodedValue)
{
    return BindingCaller<JSTestGlobalObject>::setAttribute<setJSTestGlobalObjectEnabledAtRuntimeAttributeFunction>(state, thisValue, encodedValue, "enabledAtRuntimeAttribute");
}

static inline bool setJSTestGlobalObjectEnabledAtRuntimeAttributeFunction(ExecState& state, JSTestGlobalObject& thisObject, JSValue value, ThrowScope& throwScope)
{
    UNUSED_PARAM(state);
    UNUSED_PARAM(throwScope);
    auto& impl = thisObject.wrapped();
    auto nativeValue = convert<IDLDOMString>(state, value);
    RETURN_IF_EXCEPTION(throwScope, false);
    impl.setEnabledAtRuntimeAttribute(WTFMove(nativeValue));
    return true;
}

#endif

JSValue JSTestGlobalObject::getConstructor(VM& vm, const JSGlobalObject* globalObject)
{
    return getDOMConstructor<JSTestGlobalObjectConstructor>(vm, *jsCast<const JSDOMGlobalObject*>(globalObject));
}

static inline JSC::EncodedJSValue jsTestGlobalObjectInstanceFunctionRegularOperationCaller(JSC::ExecState*, JSTestGlobalObject*, JSC::ThrowScope&);

EncodedJSValue JSC_HOST_CALL jsTestGlobalObjectInstanceFunctionRegularOperation(ExecState* state)
{
    return BindingCaller<JSTestGlobalObject>::callOperation<jsTestGlobalObjectInstanceFunctionRegularOperationCaller>(state, "regularOperation");
}

static inline JSC::EncodedJSValue jsTestGlobalObjectInstanceFunctionRegularOperationCaller(JSC::ExecState* state, JSTestGlobalObject* castedThis, JSC::ThrowScope& throwScope)
{
    UNUSED_PARAM(state);
    UNUSED_PARAM(throwScope);
    auto& impl = castedThis->wrapped();
    if (UNLIKELY(state->argumentCount() < 1))
        return throwVMError(state, throwScope, createNotEnoughArgumentsError(state));
    auto testParam = convert<IDLDOMString>(*state, state->uncheckedArgument(0));
    RETURN_IF_EXCEPTION(throwScope, encodedJSValue());
    impl.regularOperation(WTFMove(testParam));
    return JSValue::encode(jsUndefined());
}

#if ENABLE(TEST_FEATURE)
static inline JSC::EncodedJSValue jsTestGlobalObjectInstanceFunctionEnabledAtRuntimeOperation1Caller(JSC::ExecState*, JSTestGlobalObject*, JSC::ThrowScope&);

static inline EncodedJSValue jsTestGlobalObjectInstanceFunctionEnabledAtRuntimeOperation1(ExecState* state)
{
    return BindingCaller<JSTestGlobalObject>::callOperation<jsTestGlobalObjectInstanceFunctionEnabledAtRuntimeOperation1Caller>(state, "enabledAtRuntimeOperation");
}

static inline JSC::EncodedJSValue jsTestGlobalObjectInstanceFunctionEnabledAtRuntimeOperation1Caller(JSC::ExecState* state, JSTestGlobalObject* castedThis, JSC::ThrowScope& throwScope)
{
    UNUSED_PARAM(state);
    UNUSED_PARAM(throwScope);
    auto& impl = castedThis->wrapped();
    if (UNLIKELY(state->argumentCount() < 1))
        return throwVMError(state, throwScope, createNotEnoughArgumentsError(state));
    auto testParam = convert<IDLDOMString>(*state, state->uncheckedArgument(0));
    RETURN_IF_EXCEPTION(throwScope, encodedJSValue());
    impl.enabledAtRuntimeOperation(WTFMove(testParam));
    return JSValue::encode(jsUndefined());
}

#endif

#if ENABLE(TEST_FEATURE)
static inline JSC::EncodedJSValue jsTestGlobalObjectInstanceFunctionEnabledAtRuntimeOperation2Caller(JSC::ExecState*, JSTestGlobalObject*, JSC::ThrowScope&);

static inline EncodedJSValue jsTestGlobalObjectInstanceFunctionEnabledAtRuntimeOperation2(ExecState* state)
{
    return BindingCaller<JSTestGlobalObject>::callOperation<jsTestGlobalObjectInstanceFunctionEnabledAtRuntimeOperation2Caller>(state, "enabledAtRuntimeOperation");
}

static inline JSC::EncodedJSValue jsTestGlobalObjectInstanceFunctionEnabledAtRuntimeOperation2Caller(JSC::ExecState* state, JSTestGlobalObject* castedThis, JSC::ThrowScope& throwScope)
{
    UNUSED_PARAM(state);
    UNUSED_PARAM(throwScope);
    auto& impl = castedThis->wrapped();
    if (UNLIKELY(state->argumentCount() < 1))
        return throwVMError(state, throwScope, createNotEnoughArgumentsError(state));
    auto testParam = convert<IDLLong>(*state, state->uncheckedArgument(0));
    RETURN_IF_EXCEPTION(throwScope, encodedJSValue());
    impl.enabledAtRuntimeOperation(WTFMove(testParam));
    return JSValue::encode(jsUndefined());
}

#endif

#if ENABLE(TEST_FEATURE)
EncodedJSValue JSC_HOST_CALL jsTestGlobalObjectInstanceFunctionEnabledAtRuntimeOperation(ExecState* state)
{
    VM& vm = state->vm();
    auto throwScope = DECLARE_THROW_SCOPE(vm);
    UNUSED_PARAM(throwScope);
    size_t argsCount = std::min<size_t>(1, state->argumentCount());
    if (argsCount == 1) {
        JSValue distinguishingArg = state->uncheckedArgument(0);
#if ENABLE(TEST_FEATURE)
        if (distinguishingArg.isNumber())
            return jsTestGlobalObjectInstanceFunctionEnabledAtRuntimeOperation2(state);
#endif
#if ENABLE(TEST_FEATURE)
        return jsTestGlobalObjectInstanceFunctionEnabledAtRuntimeOperation1(state);
#endif
    }
    return argsCount < 1 ? throwVMError(state, throwScope, createNotEnoughArgumentsError(state)) : throwVMTypeError(state, throwScope);
}
#endif

static inline JSC::EncodedJSValue jsTestGlobalObjectInstanceFunctionEnabledInSpecificWorldCaller(JSC::ExecState*, JSTestGlobalObject*, JSC::ThrowScope&);

EncodedJSValue JSC_HOST_CALL jsTestGlobalObjectInstanceFunctionEnabledInSpecificWorld(ExecState* state)
{
    return BindingCaller<JSTestGlobalObject>::callOperation<jsTestGlobalObjectInstanceFunctionEnabledInSpecificWorldCaller>(state, "enabledInSpecificWorld");
}

static inline JSC::EncodedJSValue jsTestGlobalObjectInstanceFunctionEnabledInSpecificWorldCaller(JSC::ExecState* state, JSTestGlobalObject* castedThis, JSC::ThrowScope& throwScope)
{
    UNUSED_PARAM(state);
    UNUSED_PARAM(throwScope);
    auto& impl = castedThis->wrapped();
    if (UNLIKELY(state->argumentCount() < 1))
        return throwVMError(state, throwScope, createNotEnoughArgumentsError(state));
    auto testParam = convert<IDLLong>(*state, state->uncheckedArgument(0));
    RETURN_IF_EXCEPTION(throwScope, encodedJSValue());
    impl.enabledInSpecificWorld(WTFMove(testParam));
    return JSValue::encode(jsUndefined());
}

#if ENABLE(TEST_FEATURE)
static inline JSC::EncodedJSValue jsTestGlobalObjectInstanceFunctionTestPrivateFunctionCaller(JSC::ExecState*, JSTestGlobalObject*, JSC::ThrowScope&);

EncodedJSValue JSC_HOST_CALL jsTestGlobalObjectInstanceFunctionTestPrivateFunction(ExecState* state)
{
    return BindingCaller<JSTestGlobalObject>::callOperation<jsTestGlobalObjectInstanceFunctionTestPrivateFunctionCaller, CastedThisErrorBehavior::Assert>(state, "testPrivateFunction");
}

static inline JSC::EncodedJSValue jsTestGlobalObjectInstanceFunctionTestPrivateFunctionCaller(JSC::ExecState* state, JSTestGlobalObject* castedThis, JSC::ThrowScope& throwScope)
{
    UNUSED_PARAM(state);
    UNUSED_PARAM(throwScope);
    auto& impl = castedThis->wrapped();
    impl.testPrivateFunction();
    return JSValue::encode(jsUndefined());
}

#endif

bool JSTestGlobalObjectOwner::isReachableFromOpaqueRoots(JSC::Handle<JSC::Unknown> handle, void*, SlotVisitor& visitor)
{
    UNUSED_PARAM(handle);
    UNUSED_PARAM(visitor);
    return false;
}

void JSTestGlobalObjectOwner::finalize(JSC::Handle<JSC::Unknown> handle, void* context)
{
    auto* jsTestGlobalObject = static_cast<JSTestGlobalObject*>(handle.slot()->asCell());
    auto& world = *static_cast<DOMWrapperWorld*>(context);
    uncacheWrapper(world, &jsTestGlobalObject->wrapped(), jsTestGlobalObject);
}

#if ENABLE(BINDING_INTEGRITY)
#if PLATFORM(WIN)
#pragma warning(disable: 4483)
extern "C" { extern void (*const __identifier("??_7TestGlobalObject@WebCore@@6B@")[])(); }
#else
extern "C" { extern void* _ZTVN7WebCore16TestGlobalObjectE[]; }
#endif
#endif

JSC::JSValue toJSNewlyCreated(JSC::ExecState*, JSDOMGlobalObject* globalObject, Ref<TestGlobalObject>&& impl)
{

#if ENABLE(BINDING_INTEGRITY)
    void* actualVTablePointer = *(reinterpret_cast<void**>(impl.ptr()));
#if PLATFORM(WIN)
    void* expectedVTablePointer = reinterpret_cast<void*>(__identifier("??_7TestGlobalObject@WebCore@@6B@"));
#else
    void* expectedVTablePointer = &_ZTVN7WebCore16TestGlobalObjectE[2];
#if COMPILER(CLANG)
    // If this fails TestGlobalObject does not have a vtable, so you need to add the
    // ImplementationLacksVTable attribute to the interface definition
    static_assert(__is_polymorphic(TestGlobalObject), "TestGlobalObject is not polymorphic");
#endif
#endif
    // If you hit this assertion you either have a use after free bug, or
    // TestGlobalObject has subclasses. If TestGlobalObject has subclasses that get passed
    // to toJS() we currently require TestGlobalObject you to opt out of binding hardening
    // by adding the SkipVTableValidation attribute to the interface IDL definition
    RELEASE_ASSERT(actualVTablePointer == expectedVTablePointer);
#endif
    return createWrapper<TestGlobalObject>(globalObject, WTFMove(impl));
}

JSC::JSValue toJS(JSC::ExecState* state, JSDOMGlobalObject* globalObject, TestGlobalObject& impl)
{
    return wrap(state, globalObject, impl);
}

TestGlobalObject* JSTestGlobalObject::toWrapped(JSC::VM& vm, JSC::JSValue value)
{
    if (auto* wrapper = jsDynamicDowncast<JSTestGlobalObject*>(vm, value))
        return &wrapper->wrapped();
    return nullptr;
}

}
