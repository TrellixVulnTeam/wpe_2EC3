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
#include "CryptoAlgorithmAES_GCM.h"

#if ENABLE(SUBTLE_CRYPTO)

#include "CryptoAlgorithmAesGcmParams.h"
#include "CryptoAlgorithmAesKeyParams.h"
#include "CryptoKeyAES.h"
#include "ExceptionCode.h"

namespace WebCore {

static const char* const ALG128 = "A128GCM";
static const char* const ALG192 = "A192GCM";
static const char* const ALG256 = "A256GCM";
#if __WORDSIZE >= 64
static const uint64_t PlainTextMaxLength = 549755813632ULL; // 2^39 - 256
#endif
static const uint8_t DefaultTagLength = 128;
static const uint8_t ValidTagLengths[] = { 32, 64, 96, 104, 112, 120, 128 };

static inline bool usagesAreInvalidForCryptoAlgorithmAES_GCM(CryptoKeyUsageBitmap usages)
{
    return usages & (CryptoKeyUsageSign | CryptoKeyUsageVerify | CryptoKeyUsageDeriveKey | CryptoKeyUsageDeriveBits);
}

static inline bool tagLengthIsValid(uint8_t tagLength)
{
    for (size_t i = 0; i < sizeof(ValidTagLengths); i++) {
        if (tagLength == ValidTagLengths[i])
            return true;
    }
    return false;
}

Ref<CryptoAlgorithm> CryptoAlgorithmAES_GCM::create()
{
    return adoptRef(*new CryptoAlgorithmAES_GCM);
}

CryptoAlgorithmIdentifier CryptoAlgorithmAES_GCM::identifier() const
{
    return s_identifier;
}

void CryptoAlgorithmAES_GCM::encrypt(std::unique_ptr<CryptoAlgorithmParameters>&& parameters, Ref<CryptoKey>&& key, Vector<uint8_t>&& plainText, VectorCallback&& callback, ExceptionCallback&& exceptionCallback, ScriptExecutionContext& context, WorkQueue& workQueue)
{
    ASSERT(parameters);
    auto& aesParameters = downcast<CryptoAlgorithmAesGcmParams>(*parameters);

#if __WORDSIZE >= 64
    if (plainText.size() > PlainTextMaxLength) {
        exceptionCallback(OperationError);
        return;
    }
    if (aesParameters.ivVector().size() > UINT64_MAX) {
        exceptionCallback(OperationError);
        return;
    }
    if (aesParameters.additionalDataVector().size() > UINT64_MAX) {
        exceptionCallback(OperationError);
        return;
    }
#endif

    aesParameters.tagLength = aesParameters.tagLength ? aesParameters.tagLength : DefaultTagLength;
    if (!tagLengthIsValid(*(aesParameters.tagLength))) {
        exceptionCallback(OperationError);
        return;
    }

    platformEncrypt(WTFMove(parameters), WTFMove(key), WTFMove(plainText), WTFMove(callback), WTFMove(exceptionCallback), context, workQueue);
}

void CryptoAlgorithmAES_GCM::decrypt(std::unique_ptr<CryptoAlgorithmParameters>&& parameters, Ref<CryptoKey>&& key, Vector<uint8_t>&& cipherText, VectorCallback&& callback, ExceptionCallback&& exceptionCallback, ScriptExecutionContext& context, WorkQueue& workQueue)
{
    ASSERT(parameters);
    auto& aesParameters = downcast<CryptoAlgorithmAesGcmParams>(*parameters);

    aesParameters.tagLength = aesParameters.tagLength ? aesParameters.tagLength : DefaultTagLength;
    if (!tagLengthIsValid(*(aesParameters.tagLength))) {
        exceptionCallback(OperationError);
        return;
    }
    if (cipherText.size() < *(aesParameters.tagLength) / 8) {
        exceptionCallback(OperationError);
        return;
    }

#if __WORDSIZE >= 64
    if (aesParameters.ivVector().size() > UINT64_MAX) {
        exceptionCallback(OperationError);
        return;
    }
    if (aesParameters.additionalDataVector().size() > UINT64_MAX) {
        exceptionCallback(OperationError);
        return;
    }
#endif

    platformDecrypt(WTFMove(parameters), WTFMove(key), WTFMove(cipherText), WTFMove(callback), WTFMove(exceptionCallback), context, workQueue);
}

void CryptoAlgorithmAES_GCM::generateKey(const CryptoAlgorithmParameters& parameters, bool extractable, CryptoKeyUsageBitmap usages, KeyOrKeyPairCallback&& callback, ExceptionCallback&& exceptionCallback, ScriptExecutionContext&)
{
    const auto& aesParameters = downcast<CryptoAlgorithmAesKeyParams>(parameters);

    if (usagesAreInvalidForCryptoAlgorithmAES_GCM(usages)) {
        exceptionCallback(SYNTAX_ERR);
        return;
    }

    auto result = CryptoKeyAES::generate(CryptoAlgorithmIdentifier::AES_GCM, aesParameters.length, extractable, usages);
    if (!result) {
        exceptionCallback(OperationError);
        return;
    }

    callback(WTFMove(result));
}

void CryptoAlgorithmAES_GCM::importKey(SubtleCrypto::KeyFormat format, KeyData&& data, const std::unique_ptr<CryptoAlgorithmParameters>&& parameters, bool extractable, CryptoKeyUsageBitmap usages, KeyCallback&& callback, ExceptionCallback&& exceptionCallback)
{
    ASSERT(parameters);
    if (usagesAreInvalidForCryptoAlgorithmAES_GCM(usages)) {
        exceptionCallback(SYNTAX_ERR);
        return;
    }

    RefPtr<CryptoKeyAES> result;
    switch (format) {
    case SubtleCrypto::KeyFormat::Raw:
        result = CryptoKeyAES::importRaw(parameters->identifier, WTFMove(WTF::get<Vector<uint8_t>>(data)), extractable, usages);
        break;
    case SubtleCrypto::KeyFormat::Jwk: {
        auto checkAlgCallback = [](size_t length, const String& alg) -> bool {
            switch (length) {
            case CryptoKeyAES::s_length128:
                return alg.isNull() || alg == ALG128;
            case CryptoKeyAES::s_length192:
                return alg.isNull() || alg == ALG192;
            case CryptoKeyAES::s_length256:
                return alg.isNull() || alg == ALG256;
            }
            return false;
        };
        result = CryptoKeyAES::importJwk(parameters->identifier, WTFMove(WTF::get<JsonWebKey>(data)), extractable, usages, WTFMove(checkAlgCallback));
        break;
    }
    default:
        exceptionCallback(NOT_SUPPORTED_ERR);
        return;
    }
    if (!result) {
        exceptionCallback(DataError);
        return;
    }

    callback(*result);
}

void CryptoAlgorithmAES_GCM::exportKey(SubtleCrypto::KeyFormat format, Ref<CryptoKey>&& key, KeyDataCallback&& callback, ExceptionCallback&& exceptionCallback)
{
    const auto& aesKey = downcast<CryptoKeyAES>(key.get());

    if (aesKey.key().isEmpty()) {
        exceptionCallback(OperationError);
        return;
    }

    KeyData result;
    switch (format) {
    case SubtleCrypto::KeyFormat::Raw:
        result = Vector<uint8_t>(aesKey.key());
        break;
    case SubtleCrypto::KeyFormat::Jwk: {
        JsonWebKey jwk = aesKey.exportJwk();
        switch (aesKey.key().size() * 8) {
        case CryptoKeyAES::s_length128:
            jwk.alg = String(ALG128);
            break;
        case CryptoKeyAES::s_length192:
            jwk.alg = String(ALG192);
            break;
        case CryptoKeyAES::s_length256:
            jwk.alg = String(ALG256);
            break;
        default:
            ASSERT_NOT_REACHED();
        }
        result = WTFMove(jwk);
        break;
    }
    default:
        exceptionCallback(NOT_SUPPORTED_ERR);
        return;
    }

    callback(format, WTFMove(result));
}

ExceptionOr<size_t> CryptoAlgorithmAES_GCM::getKeyLength(const CryptoAlgorithmParameters& parameters)
{
    return CryptoKeyAES::getKeyLength(parameters);
}

}

#endif // ENABLE(SUBTLE_CRYPTO)
