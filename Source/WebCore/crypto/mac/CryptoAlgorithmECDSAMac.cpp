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
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "CryptoAlgorithmECDSA.h"

#if ENABLE(SUBTLE_CRYPTO)

#include "CommonCryptoDERUtilities.h"
#include "CommonCryptoUtilities.h"
#include "CryptoAlgorithmEcdsaParams.h"
#include "CryptoKeyEC.h"
#include "ExceptionCode.h"
#include "ScriptExecutionContext.h"
#include <pal/crypto/CryptoDigest.h>

namespace WebCore {

inline std::optional<PAL::CryptoDigest::Algorithm> cryptoDigestAlgorithm(CryptoAlgorithmIdentifier hashFunction)
{
    switch (hashFunction) {
    case CryptoAlgorithmIdentifier::SHA_1:
        return PAL::CryptoDigest::Algorithm::SHA_1;
    case CryptoAlgorithmIdentifier::SHA_224:
        return PAL::CryptoDigest::Algorithm::SHA_224;
    case CryptoAlgorithmIdentifier::SHA_256:
        return PAL::CryptoDigest::Algorithm::SHA_256;
    case CryptoAlgorithmIdentifier::SHA_384:
        return PAL::CryptoDigest::Algorithm::SHA_384;
    case CryptoAlgorithmIdentifier::SHA_512:
        return PAL::CryptoDigest::Algorithm::SHA_512;
    default:
        return std::nullopt;
    }
}

static ExceptionOr<Vector<uint8_t>> signECDSA(CryptoAlgorithmIdentifier hash, const PlatformECKey key, size_t keyLengthInBytes, const Vector<uint8_t>& data)
{
    CCDigestAlgorithm digestAlgorithm;
    if (!getCommonCryptoDigestAlgorithm(hash, digestAlgorithm))
        return Exception { OperationError };

    auto cryptoDigestAlgorithm = WebCore::cryptoDigestAlgorithm(hash);
    if (!cryptoDigestAlgorithm)
        return Exception { OperationError };
    auto digest = PAL::CryptoDigest::create(*cryptoDigestAlgorithm);
    if (!digest)
        return Exception { OperationError };
    digest->addBytes(data.data(), data.size());
    auto digestData = digest->computeHash();

    // The signature produced by CCECCryptorSignHash is in DER format.
    // tag + length(1) + tag + length(1) + InitialOctet(?) + keyLength in bytes + tag + length(1) + InitialOctet(?) + keyLength in bytes
    Vector<uint8_t> signature(8 + keyLengthInBytes * 2);
    size_t signatureSize = signature.size();

    CCCryptorStatus status = CCECCryptorSignHash(key, digestData.data(), digestData.size(), signature.data(), &signatureSize);
    if (status)
        return Exception { OperationError };

    // FIXME: <rdar://problem/31618371>
    // convert the DER binary into r + s
    Vector<uint8_t> newSignature;
    newSignature.reserveCapacity(keyLengthInBytes * 2);
    size_t offset = 3; // skip tag, length, tag

    // If r < keyLengthInBytes, fill the head of r with 0s.
    size_t bytesToCopy = keyLengthInBytes;
    if (signature[offset] < keyLengthInBytes) {
        newSignature.resize(keyLengthInBytes - signature[offset]);
        memset(newSignature.data(), InitialOctet, keyLengthInBytes - signature[offset]);
        bytesToCopy = signature[offset];
    } else if (signature[offset] > keyLengthInBytes) // Otherwise skip the leading 0s of r.
        offset += signature[offset] - keyLengthInBytes;
    offset++; // skip length
    ASSERT_WITH_SECURITY_IMPLICATION(signature.size() > offset + bytesToCopy);
    newSignature.append(signature.data() + offset, bytesToCopy);
    offset += bytesToCopy + 1; // skip r, tag

    // If s < keyLengthInBytes, fill the head of s with 0s.
    bytesToCopy = keyLengthInBytes;
    if (signature[offset] < keyLengthInBytes) {
        size_t pos = newSignature.size();
        newSignature.resize(pos + keyLengthInBytes - signature[offset]);
        memset(newSignature.data() + pos, InitialOctet, keyLengthInBytes - signature[offset]);
        bytesToCopy = signature[offset];
    } else if (signature[offset] > keyLengthInBytes) // Otherwise skip the leading 0s of s.
        offset += signature[offset] - keyLengthInBytes;
    offset++; // skip length
    ASSERT_WITH_SECURITY_IMPLICATION(signature.size() >= offset + bytesToCopy);
    newSignature.append(signature.data() + offset, bytesToCopy);

    return WTFMove(newSignature);
}

static ExceptionOr<bool> verifyECDSA(CryptoAlgorithmIdentifier hash, const PlatformECKey key, size_t keyLengthInBytes, const Vector<uint8_t>& signature, const Vector<uint8_t> data)
{
    CCDigestAlgorithm digestAlgorithm;
    if (!getCommonCryptoDigestAlgorithm(hash, digestAlgorithm))
        return Exception { OperationError };

    auto cryptoDigestAlgorithm = WebCore::cryptoDigestAlgorithm(hash);
    if (!cryptoDigestAlgorithm)
        return Exception { OperationError };
    auto digest = PAL::CryptoDigest::create(*cryptoDigestAlgorithm);
    if (!digest)
        return Exception { OperationError };
    digest->addBytes(data.data(), data.size());
    auto digestData = digest->computeHash();

    if (signature.size() != keyLengthInBytes * 2)
        return false;

    // FIXME: <rdar://problem/31618371>
    // Convert the signature into DER format.
    // tag + length(1) + tag + length(1) + InitialOctet(?) + r + tag + length(1) + InitialOctet(?) + s
    // Skip any heading 0s of r and s.
    size_t rStart = 0;
    while (rStart < keyLengthInBytes && !signature[rStart])
        rStart++;
    size_t sStart = keyLengthInBytes;
    while (rStart < signature.size() && !signature[sStart])
        sStart++;

    // InitialOctet is needed when the first byte of r/s is larger than or equal to 128.
    bool rNeedsInitialOctet = signature[rStart] >= 128;
    bool sNeedsInitialOctet = signature[sStart] >= 128;

    // Construct the DER signature.
    Vector<uint8_t> newSignature;
    newSignature.reserveCapacity(6 + keyLengthInBytes * 3  + rNeedsInitialOctet + sNeedsInitialOctet - rStart - sStart);
    newSignature.append(SequenceMark);
    addEncodedASN1Length(newSignature, 4 + keyLengthInBytes * 3  + rNeedsInitialOctet + sNeedsInitialOctet - rStart - sStart);
    newSignature.append(IntegerMark);
    addEncodedASN1Length(newSignature, keyLengthInBytes + rNeedsInitialOctet - rStart);
    if (rNeedsInitialOctet)
        newSignature.append(InitialOctet);
    newSignature.append(signature.data() + rStart, keyLengthInBytes - rStart);
    newSignature.append(IntegerMark);
    addEncodedASN1Length(newSignature, keyLengthInBytes * 2 + sNeedsInitialOctet - sStart);
    if (sNeedsInitialOctet)
        newSignature.append(InitialOctet);
    newSignature.append(signature.data() + sStart, keyLengthInBytes * 2 - sStart);

    uint32_t valid;
    CCCryptorStatus status = CCECCryptorVerifyHash(key, digestData.data(), digestData.size(), newSignature.data(), newSignature.size(), &valid);
    if (status)
        return Exception { OperationError };
    return valid;
}

void CryptoAlgorithmECDSA::platformSign(std::unique_ptr<CryptoAlgorithmParameters>&& parameters, Ref<CryptoKey>&& key, Vector<uint8_t>&& data, VectorCallback&& callback, ExceptionCallback&& exceptionCallback, ScriptExecutionContext& context, WorkQueue& workQueue)
{
    context.ref();
    workQueue.dispatch([parameters = WTFMove(parameters), key = WTFMove(key), data = WTFMove(data), callback = WTFMove(callback), exceptionCallback = WTFMove(exceptionCallback), &context]() mutable {
        auto& ecKey = downcast<CryptoKeyEC>(key.get());
        auto& ecParameters = downcast<CryptoAlgorithmEcdsaParams>(*parameters);

        auto result = signECDSA(ecParameters.hashIdentifier, ecKey.platformKey(), ecKey.keySizeInBits() / 8, data);
        if (result.hasException()) {
            // We should only dereference callbacks after being back to the Document/Worker threads.
            context.postTask([exceptionCallback = WTFMove(exceptionCallback), ec = result.releaseException().code(), callback = WTFMove(callback)](ScriptExecutionContext& context) {
                exceptionCallback(ec);
                context.deref();
            });
            return;
        }
        // We should only dereference callbacks after being back to the Document/Worker threads.
        context.postTask([callback = WTFMove(callback), result = result.releaseReturnValue(), exceptionCallback = WTFMove(exceptionCallback)](ScriptExecutionContext& context) {
            callback(result);
            context.deref();
        });
    });
}

void CryptoAlgorithmECDSA::platformVerify(std::unique_ptr<CryptoAlgorithmParameters>&& parameters, Ref<CryptoKey>&& key, Vector<uint8_t>&& signature, Vector<uint8_t>&& data, BoolCallback&& callback, ExceptionCallback&& exceptionCallback, ScriptExecutionContext& context, WorkQueue& workQueue)
{
    context.ref();
    workQueue.dispatch([parameters = WTFMove(parameters), key = WTFMove(key), signature = WTFMove(signature), data = WTFMove(data), callback = WTFMove(callback), exceptionCallback = WTFMove(exceptionCallback), &context]() mutable {
        auto& ecKey = downcast<CryptoKeyEC>(key.get());
        auto& ecParameters = downcast<CryptoAlgorithmEcdsaParams>(*parameters);

        auto result = verifyECDSA(ecParameters.hashIdentifier, ecKey.platformKey(), ecKey.keySizeInBits() / 8, signature, data);
        if (result.hasException()) {
            // We should only dereference callbacks after being back to the Document/Worker threads.
            context.postTask([exceptionCallback = WTFMove(exceptionCallback), ec = result.releaseException().code(), callback = WTFMove(callback)](ScriptExecutionContext& context) {
                exceptionCallback(ec);
                context.deref();
            });
            return;
        }
        // We should only dereference callbacks after being back to the Document/Worker threads.
        context.postTask([callback = WTFMove(callback), result = result.releaseReturnValue(), exceptionCallback = WTFMove(exceptionCallback)](ScriptExecutionContext& context) {
            callback(result);
            context.deref();
        });
    });
}

} // namespace WebCore

#endif // ENABLE(SUBTLE_CRYPTO)