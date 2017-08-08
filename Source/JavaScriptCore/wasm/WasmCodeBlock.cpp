
/*
 * Copyright (C) 2017 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "WasmCodeBlock.h"

#if ENABLE(WEBASSEMBLY)

#include "WasmBBQPlanInlines.h"
#include "WasmCallee.h"
#include "WasmWorklist.h"

namespace JSC { namespace Wasm {

CodeBlock::CodeBlock(MemoryMode mode, ModuleInformation& moduleInformation)
    : m_calleeCount(moduleInformation.internalFunctionCount())
    , m_mode(mode)
{
    RefPtr<CodeBlock> protectedThis = this;
    m_plan = adoptRef(*new BBQPlan(nullptr, makeRef(moduleInformation), BBQPlan::FullCompile, createSharedTask<Plan::CallbackType>([this, protectedThis = WTFMove(protectedThis)] (VM*, Plan&) {
        auto locker = holdLock(m_lock);
        if (m_plan->failed()) {
            m_errorMessage = m_plan->errorMessage();
            m_plan = nullptr;
            return;
        }

        // FIXME: we should eventually collect the BBQ code.
        m_callees.resize(m_calleeCount);
        m_jsCallees.resize(m_calleeCount);
        m_optimizedCallees.resize(m_calleeCount);
        m_wasmIndirectCallEntryPoints.resize(m_calleeCount);

        m_plan->initializeCallees([&] (unsigned calleeIndex, Ref<Wasm::Callee>&& jsEntrypointCallee, Ref<Wasm::Callee>&& wasmEntrypointCallee) {
            m_jsCallees[calleeIndex] = WTFMove(jsEntrypointCallee);
            m_callees[calleeIndex] = WTFMove(wasmEntrypointCallee);
            m_wasmIndirectCallEntryPoints[calleeIndex] = m_callees[calleeIndex]->entrypoint();
        });

        m_wasmToWasmExitStubs = m_plan->takeWasmToWasmExitStubs();
        m_wasmToWasmCallsites = m_plan->takeWasmToWasmCallsites();
        m_tierUpCounts = m_plan->takeTierUpCounts();

        m_plan = nullptr;
    })));

    m_plan->setMode(mode);
    auto& worklist = Wasm::ensureWorklist();
    // Note, immediately after we enqueue the plan, there is a chance the above callback will be called.
    worklist.enqueue(makeRef(*m_plan.get()));
}

CodeBlock::~CodeBlock() { }

void CodeBlock::waitUntilFinished()
{
    RefPtr<Plan> plan;
    {
        auto locker = holdLock(m_lock);
        plan = m_plan;
    }

    if (plan) {
        auto& worklist = Wasm::ensureWorklist();
        worklist.completePlanSynchronously(*plan.get());
    }
    // else, if we don't have a plan, we're already compiled.
}

void CodeBlock::compileAsync(VM& vm, AsyncCompilationCallback&& task)
{
    RefPtr<Plan> plan;
    {
        auto locker = holdLock(m_lock);
        plan = m_plan;
    }

    if (plan) {
        // We don't need to keep a RefPtr on the Plan because the worklist will keep
        // a RefPtr on the Plan until the plan finishes notifying all of its callbacks.
        RefPtr<CodeBlock> protectedThis = this;
        plan->addCompletionTask(vm, createSharedTask<Plan::CallbackType>([this, task = WTFMove(task), protectedThis = WTFMove(protectedThis)] (VM* vm, Plan&) {
            ASSERT(vm);
            task->run(*vm, makeRef(*this));
        }));
    } else
        task->run(vm, makeRef(*this));
}

bool CodeBlock::isSafeToRun(MemoryMode memoryMode)
{
    if (!runnable())
        return false;

    switch (m_mode) {
    case Wasm::MemoryMode::BoundsChecking:
        return true;
    case Wasm::MemoryMode::Signaling:
        // Code being in Signaling mode means that it performs no bounds checks.
        // Its memory, even if empty, absolutely must also be in Signaling mode
        // because the page protection detects out-of-bounds accesses.
        return memoryMode == Wasm::MemoryMode::Signaling;
    case Wasm::MemoryMode::NumberOfMemoryModes:
        break;
    }
    RELEASE_ASSERT_NOT_REACHED();
    return false;
}

} } // namespace JSC::Wasm

#endif // ENABLE(WEBASSEMBLY)
