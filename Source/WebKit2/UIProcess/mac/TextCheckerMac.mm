/*
 * Copyright (C) 2010, 2011 Apple Inc. All rights reserved.
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

#import "config.h"
#import "TextChecker.h"

#if PLATFORM(MAC)

#import "TextCheckerState.h"
#import <WebCore/NSSpellCheckerSPI.h>
#import <WebCore/NotImplemented.h>
#import <wtf/RetainPtr.h>
#import <wtf/text/StringView.h>

@interface NSSpellChecker (WebNSSpellCheckerDetails)
- (NSString *)languageForWordRange:(NSRange)range inString:(NSString *)string orthography:(NSOrthography *)orthography;
@end

static NSString* const WebAutomaticSpellingCorrectionEnabled = @"WebAutomaticSpellingCorrectionEnabled";
static NSString* const WebContinuousSpellCheckingEnabled = @"WebContinuousSpellCheckingEnabled";
static NSString* const WebGrammarCheckingEnabled = @"WebGrammarCheckingEnabled";
static NSString* const WebSmartInsertDeleteEnabled = @"WebSmartInsertDeleteEnabled";
static NSString* const WebAutomaticQuoteSubstitutionEnabled = @"WebAutomaticQuoteSubstitutionEnabled";
static NSString* const WebAutomaticDashSubstitutionEnabled = @"WebAutomaticDashSubstitutionEnabled";
static NSString* const WebAutomaticLinkDetectionEnabled = @"WebAutomaticLinkDetectionEnabled";
static NSString* const WebAutomaticTextReplacementEnabled = @"WebAutomaticTextReplacementEnabled";

// FIXME: this needs to be removed and replaced with NSTextCheckingSuppressInitialCapitalizationKey as soon as
// rdar://problem/26800924 is fixed.

static NSString* const WebTextCheckingSuppressInitialCapitalizationKey = @"SuppressInitialCapitalization";

using namespace WebCore;

namespace WebKit {

TextCheckerState textCheckerState;

static bool shouldAutomaticTextReplacementBeEnabled()
{
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    if (![defaults objectForKey:WebAutomaticTextReplacementEnabled])
        return [NSSpellChecker isAutomaticTextReplacementEnabled];
    return [defaults boolForKey:WebAutomaticTextReplacementEnabled];
}

static bool shouldAutomaticSpellingCorrectionBeEnabled()
{
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    if (![defaults objectForKey:WebAutomaticSpellingCorrectionEnabled])
        return [NSSpellChecker isAutomaticTextReplacementEnabled];
    return [defaults boolForKey:WebAutomaticSpellingCorrectionEnabled];
}

static bool shouldAutomaticQuoteSubstitutionBeEnabled()
{
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    if (![defaults objectForKey:WebAutomaticQuoteSubstitutionEnabled])
        return [NSSpellChecker isAutomaticQuoteSubstitutionEnabled];

    return [defaults boolForKey:WebAutomaticQuoteSubstitutionEnabled];
}

static bool shouldAutomaticDashSubstitutionBeEnabled()
{
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    if (![defaults objectForKey:WebAutomaticDashSubstitutionEnabled])
        return [NSSpellChecker isAutomaticDashSubstitutionEnabled];

    return [defaults boolForKey:WebAutomaticDashSubstitutionEnabled];
}

static void initializeState()
{
    static bool didInitializeState = false;

    if (didInitializeState)
        return;

    textCheckerState.isContinuousSpellCheckingEnabled = [[NSUserDefaults standardUserDefaults] boolForKey:WebContinuousSpellCheckingEnabled] && TextChecker::isContinuousSpellCheckingAllowed();
    textCheckerState.isGrammarCheckingEnabled = [[NSUserDefaults standardUserDefaults] boolForKey:WebGrammarCheckingEnabled];
    textCheckerState.isAutomaticTextReplacementEnabled = shouldAutomaticTextReplacementBeEnabled();
    textCheckerState.isAutomaticSpellingCorrectionEnabled = shouldAutomaticSpellingCorrectionBeEnabled();
    textCheckerState.isAutomaticQuoteSubstitutionEnabled = shouldAutomaticQuoteSubstitutionBeEnabled();
    textCheckerState.isAutomaticDashSubstitutionEnabled = shouldAutomaticDashSubstitutionBeEnabled();
    textCheckerState.isAutomaticLinkDetectionEnabled = [[NSUserDefaults standardUserDefaults] boolForKey:WebAutomaticLinkDetectionEnabled];

    didInitializeState = true;
}

const TextCheckerState& TextChecker::state()
{
    initializeState();
    return textCheckerState;
}
    
static bool testingModeEnabled = false;

void TextChecker::setTestingMode(bool enabled)
{
    if (enabled && !testingModeEnabled) {
        [[NSUserDefaults standardUserDefaults] setBool:textCheckerState.isContinuousSpellCheckingEnabled forKey:WebContinuousSpellCheckingEnabled];
        [[NSUserDefaults standardUserDefaults] setBool:textCheckerState.isGrammarCheckingEnabled forKey:WebGrammarCheckingEnabled];
        [[NSUserDefaults standardUserDefaults] setBool:textCheckerState.isAutomaticSpellingCorrectionEnabled forKey:WebAutomaticSpellingCorrectionEnabled];
        [[NSUserDefaults standardUserDefaults] setBool:textCheckerState.isAutomaticQuoteSubstitutionEnabled forKey:WebAutomaticQuoteSubstitutionEnabled];
        [[NSUserDefaults standardUserDefaults] setBool:textCheckerState.isAutomaticDashSubstitutionEnabled forKey:WebAutomaticDashSubstitutionEnabled];
        [[NSUserDefaults standardUserDefaults] setBool:textCheckerState.isAutomaticLinkDetectionEnabled forKey:WebAutomaticLinkDetectionEnabled];
        [[NSUserDefaults standardUserDefaults] setBool:textCheckerState.isAutomaticTextReplacementEnabled forKey:WebAutomaticTextReplacementEnabled];
        [[NSUserDefaults standardUserDefaults] setBool:isSmartInsertDeleteEnabled() forKey:WebSmartInsertDeleteEnabled];
    }
    testingModeEnabled = enabled;
}

bool TextChecker::isTestingMode()
{
    return testingModeEnabled;
}

bool TextChecker::isContinuousSpellCheckingAllowed()
{
    static bool allowContinuousSpellChecking = true;
    static bool readAllowContinuousSpellCheckingDefault = false;

    if (!readAllowContinuousSpellCheckingDefault) {
        if ([[NSUserDefaults standardUserDefaults] objectForKey:@"NSAllowContinuousSpellChecking"])
            allowContinuousSpellChecking = [[NSUserDefaults standardUserDefaults] boolForKey:@"NSAllowContinuousSpellChecking"];

        readAllowContinuousSpellCheckingDefault = true;
    }

    return allowContinuousSpellChecking;
}

void TextChecker::setContinuousSpellCheckingEnabled(bool isContinuousSpellCheckingEnabled)
{
    if (state().isContinuousSpellCheckingEnabled == isContinuousSpellCheckingEnabled)
        return;
                                                                                      
    textCheckerState.isContinuousSpellCheckingEnabled = isContinuousSpellCheckingEnabled;
    if (!testingModeEnabled)
        [[NSUserDefaults standardUserDefaults] setBool:isContinuousSpellCheckingEnabled forKey:WebContinuousSpellCheckingEnabled];

    // FIXME: preflight the spell checker.
}

void TextChecker::setGrammarCheckingEnabled(bool isGrammarCheckingEnabled)
{
    if (state().isGrammarCheckingEnabled == isGrammarCheckingEnabled)
        return;

    textCheckerState.isGrammarCheckingEnabled = isGrammarCheckingEnabled;
    if (!testingModeEnabled)
        [[NSUserDefaults standardUserDefaults] setBool:isGrammarCheckingEnabled forKey:WebGrammarCheckingEnabled];
    [[NSSpellChecker sharedSpellChecker] updatePanels];
    
    // We call preflightSpellChecker() when turning continuous spell checking on, but we don't need to do that here
    // because grammar checking only occurs on code paths that already preflight spell checking appropriately.
}

void TextChecker::setAutomaticSpellingCorrectionEnabled(bool isAutomaticSpellingCorrectionEnabled)
{
    if (state().isAutomaticSpellingCorrectionEnabled == isAutomaticSpellingCorrectionEnabled)
        return;

    textCheckerState.isAutomaticSpellingCorrectionEnabled = isAutomaticSpellingCorrectionEnabled;
    if (!testingModeEnabled)
        [[NSUserDefaults standardUserDefaults] setBool:isAutomaticSpellingCorrectionEnabled forKey:WebAutomaticSpellingCorrectionEnabled];

    [[NSSpellChecker sharedSpellChecker] updatePanels];
}

void TextChecker::setAutomaticQuoteSubstitutionEnabled(bool isAutomaticQuoteSubstitutionEnabled)
{
    if (state().isAutomaticQuoteSubstitutionEnabled == isAutomaticQuoteSubstitutionEnabled)
        return;

    textCheckerState.isAutomaticQuoteSubstitutionEnabled = isAutomaticQuoteSubstitutionEnabled;
    if (!testingModeEnabled)
        [[NSUserDefaults standardUserDefaults] setBool:isAutomaticQuoteSubstitutionEnabled forKey:WebAutomaticQuoteSubstitutionEnabled];
    
    [[NSSpellChecker sharedSpellChecker] updatePanels];
}

void TextChecker::setAutomaticDashSubstitutionEnabled(bool isAutomaticDashSubstitutionEnabled)
{
    if (state().isAutomaticDashSubstitutionEnabled == isAutomaticDashSubstitutionEnabled)
        return;

    textCheckerState.isAutomaticDashSubstitutionEnabled = isAutomaticDashSubstitutionEnabled;
    if (!testingModeEnabled)
        [[NSUserDefaults standardUserDefaults] setBool:isAutomaticDashSubstitutionEnabled forKey:WebAutomaticDashSubstitutionEnabled];

    [[NSSpellChecker sharedSpellChecker] updatePanels];
}

void TextChecker::setAutomaticLinkDetectionEnabled(bool isAutomaticLinkDetectionEnabled)
{
    if (state().isAutomaticLinkDetectionEnabled == isAutomaticLinkDetectionEnabled)
        return;
    
    textCheckerState.isAutomaticLinkDetectionEnabled = isAutomaticLinkDetectionEnabled;
    if (!testingModeEnabled)
        [[NSUserDefaults standardUserDefaults] setBool:isAutomaticLinkDetectionEnabled forKey:WebAutomaticLinkDetectionEnabled];
    
    [[NSSpellChecker sharedSpellChecker] updatePanels];
}

void TextChecker::setAutomaticTextReplacementEnabled(bool isAutomaticTextReplacementEnabled)
{
    if (state().isAutomaticTextReplacementEnabled == isAutomaticTextReplacementEnabled)
        return;
    
    textCheckerState.isAutomaticTextReplacementEnabled = isAutomaticTextReplacementEnabled;
    if (!testingModeEnabled)
        [[NSUserDefaults standardUserDefaults] setBool:isAutomaticTextReplacementEnabled forKey:WebAutomaticTextReplacementEnabled];

    [[NSSpellChecker sharedSpellChecker] updatePanels];
}

static bool smartInsertDeleteEnabled;
    
bool TextChecker::isSmartInsertDeleteEnabled()
{
    static bool readSmartInsertDeleteEnabledDefault;

    if (!readSmartInsertDeleteEnabledDefault) {
        smartInsertDeleteEnabled = ![[NSUserDefaults standardUserDefaults] objectForKey:WebSmartInsertDeleteEnabled] || [[NSUserDefaults standardUserDefaults] boolForKey:WebSmartInsertDeleteEnabled];

        readSmartInsertDeleteEnabledDefault = true;
    }

    return smartInsertDeleteEnabled;
}

void TextChecker::setSmartInsertDeleteEnabled(bool flag)
{
    if (flag == isSmartInsertDeleteEnabled())
        return;

    smartInsertDeleteEnabled = flag;

    if (!testingModeEnabled)
        [[NSUserDefaults standardUserDefaults] setBool:flag forKey:WebSmartInsertDeleteEnabled];
}

void TextChecker::didChangeAutomaticTextReplacementEnabled()
{
    textCheckerState.isAutomaticTextReplacementEnabled = shouldAutomaticTextReplacementBeEnabled();
    [[NSSpellChecker sharedSpellChecker] updatePanels];
}

void TextChecker::didChangeAutomaticSpellingCorrectionEnabled()
{
    textCheckerState.isAutomaticSpellingCorrectionEnabled = shouldAutomaticSpellingCorrectionBeEnabled();
    [[NSSpellChecker sharedSpellChecker] updatePanels];
}

void TextChecker::didChangeAutomaticQuoteSubstitutionEnabled()
{
    textCheckerState.isAutomaticQuoteSubstitutionEnabled = shouldAutomaticQuoteSubstitutionBeEnabled();
    [[NSSpellChecker sharedSpellChecker] updatePanels];
}

void TextChecker::didChangeAutomaticDashSubstitutionEnabled()
{
    textCheckerState.isAutomaticDashSubstitutionEnabled = shouldAutomaticDashSubstitutionBeEnabled();
    [[NSSpellChecker sharedSpellChecker] updatePanels];
}

bool TextChecker::substitutionsPanelIsShowing()
{
    return [[[NSSpellChecker sharedSpellChecker] substitutionsPanel] isVisible];
}

void TextChecker::toggleSubstitutionsPanelIsShowing()
{
    NSPanel *substitutionsPanel = [[NSSpellChecker sharedSpellChecker] substitutionsPanel];
    if ([substitutionsPanel isVisible]) {
        [substitutionsPanel orderOut:nil];
        return;
    }
    [substitutionsPanel orderFront:nil];
}

void TextChecker::continuousSpellCheckingEnabledStateChanged(bool enabled)
{
    textCheckerState.isContinuousSpellCheckingEnabled = enabled;
}

void TextChecker::grammarCheckingEnabledStateChanged(bool enabled)
{
    textCheckerState.isGrammarCheckingEnabled = enabled;
}

int64_t TextChecker::uniqueSpellDocumentTag(WebPageProxy*)
{
    return [NSSpellChecker uniqueSpellDocumentTag];
}

void TextChecker::closeSpellDocumentWithTag(int64_t tag)
{
    [[NSSpellChecker sharedSpellChecker] closeSpellDocumentWithTag:tag];
}

#if USE(UNIFIED_TEXT_CHECKING)

Vector<TextCheckingResult> TextChecker::checkTextOfParagraph(int64_t spellDocumentTag, StringView text, int32_t insertionPoint, uint64_t checkingTypes, bool initialCapitalizationEnabled)
{
    Vector<TextCheckingResult> results;

    RetainPtr<NSString> textString = text.createNSStringWithoutCopying();
    NSDictionary *options = nil;
#if HAVE(ADVANCED_SPELL_CHECKING)
    options = @{ NSTextCheckingInsertionPointKey : @(insertionPoint),
                 WebTextCheckingSuppressInitialCapitalizationKey : @(!initialCapitalizationEnabled) };
#else
    options = @{ WebTextCheckingSuppressInitialCapitalizationKey : @(!initialCapitalizationEnabled) };
#endif
    NSArray *incomingResults = [[NSSpellChecker sharedSpellChecker] checkString:textString.get()
                                                                          range:NSMakeRange(0, text.length())
                                                                          types:checkingTypes | NSTextCheckingTypeOrthography
                                                                        options:options
                                                         inSpellDocumentWithTag:spellDocumentTag 
                                                                    orthography:NULL
                                                                      wordCount:NULL];
    for (NSTextCheckingResult *incomingResult in incomingResults) {
        NSRange resultRange = [incomingResult range];
        NSTextCheckingType resultType = [incomingResult resultType];
        ASSERT(resultRange.location != NSNotFound);
        ASSERT(resultRange.length > 0);
        if (resultType == NSTextCheckingTypeSpelling && (checkingTypes & NSTextCheckingTypeSpelling)) {
            TextCheckingResult result;
            result.type = TextCheckingTypeSpelling;
            result.location = resultRange.location;
            result.length = resultRange.length;
            results.append(result);
        } else if (resultType == NSTextCheckingTypeGrammar && (checkingTypes & NSTextCheckingTypeGrammar)) {
            TextCheckingResult result;
            NSArray *details = [incomingResult grammarDetails];
            result.type = TextCheckingTypeGrammar;
            result.location = resultRange.location;
            result.length = resultRange.length;
            for (NSDictionary *incomingDetail in details) {
                ASSERT(incomingDetail);
                GrammarDetail detail;
                NSValue *detailRangeAsNSValue = [incomingDetail objectForKey:NSGrammarRange];
                ASSERT(detailRangeAsNSValue);
                NSRange detailNSRange = [detailRangeAsNSValue rangeValue];
                ASSERT(detailNSRange.location != NSNotFound);
                ASSERT(detailNSRange.length > 0);
                detail.location = detailNSRange.location;
                detail.length = detailNSRange.length;
                detail.userDescription = [incomingDetail objectForKey:NSGrammarUserDescription];
                NSArray *guesses = [incomingDetail objectForKey:NSGrammarCorrections];
                for (NSString *guess in guesses)
                    detail.guesses.append(String(guess));
                result.details.append(detail);
            }
            results.append(result);
        } else if (resultType == NSTextCheckingTypeLink && (checkingTypes & NSTextCheckingTypeLink)) {
            TextCheckingResult result;
            result.type = TextCheckingTypeLink;
            result.location = resultRange.location;
            result.length = resultRange.length;
            result.replacement = [[incomingResult URL] absoluteString];
            results.append(result);
        } else if (resultType == NSTextCheckingTypeQuote && (checkingTypes & NSTextCheckingTypeQuote)) {
            TextCheckingResult result;
            result.type = TextCheckingTypeQuote;
            result.location = resultRange.location;
            result.length = resultRange.length;
            result.replacement = [incomingResult replacementString];
            results.append(result);
        } else if (resultType == NSTextCheckingTypeDash && (checkingTypes & NSTextCheckingTypeDash)) {
            TextCheckingResult result;
            result.type = TextCheckingTypeDash;
            result.location = resultRange.location;
            result.length = resultRange.length;
            result.replacement = [incomingResult replacementString];
            results.append(result);
        } else if (resultType == NSTextCheckingTypeReplacement && (checkingTypes & NSTextCheckingTypeReplacement)) {
            TextCheckingResult result;
            result.type = TextCheckingTypeReplacement;
            result.location = resultRange.location;
            result.length = resultRange.length;
            result.replacement = [incomingResult replacementString];
            results.append(result);
        } else if (resultType == NSTextCheckingTypeCorrection && (checkingTypes & NSTextCheckingTypeCorrection)) {
            TextCheckingResult result;
            result.type = TextCheckingTypeCorrection;
            result.location = resultRange.location;
            result.length = resultRange.length;
            result.replacement = [incomingResult replacementString];
            results.append(result);
        }
    }

    return results;
}

#endif

void TextChecker::checkSpellingOfString(int64_t, StringView, int32_t&, int32_t&)
{
    // Mac uses checkTextOfParagraph instead.
    notImplemented();
}

void TextChecker::checkGrammarOfString(int64_t, StringView, Vector<WebCore::GrammarDetail>&, int32_t&, int32_t&)
{
    // Mac uses checkTextOfParagraph instead.
    notImplemented();
}

bool TextChecker::spellingUIIsShowing()
{
    return [[[NSSpellChecker sharedSpellChecker] spellingPanel] isVisible];
}

void TextChecker::toggleSpellingUIIsShowing()
{
    NSPanel *spellingPanel = [[NSSpellChecker sharedSpellChecker] spellingPanel];
    if ([spellingPanel isVisible])
        [spellingPanel orderOut:nil];
    else
        [spellingPanel orderFront:nil];
}

void TextChecker::updateSpellingUIWithMisspelledWord(int64_t, const String& misspelledWord)
{
    [[NSSpellChecker sharedSpellChecker] updateSpellingPanelWithMisspelledWord:misspelledWord];
}

void TextChecker::updateSpellingUIWithGrammarString(int64_t, const String& badGrammarPhrase, const GrammarDetail& grammarDetail)
{
    RetainPtr<NSMutableArray> corrections = adoptNS([[NSMutableArray alloc] init]);
    for (size_t i = 0; i < grammarDetail.guesses.size(); ++i) {
        NSString *guess = grammarDetail.guesses[i];
        [corrections addObject:guess];
    }

    NSRange grammarRange = NSMakeRange(grammarDetail.location, grammarDetail.length);
    NSString *grammarUserDescription = grammarDetail.userDescription;
    RetainPtr<NSDictionary> grammarDetailDict = adoptNS([[NSDictionary alloc] initWithObjectsAndKeys:[NSValue valueWithRange:grammarRange], NSGrammarRange, grammarUserDescription, NSGrammarUserDescription, corrections.get(), NSGrammarCorrections, nil]);

    [[NSSpellChecker sharedSpellChecker] updateSpellingPanelWithGrammarString:badGrammarPhrase detail:grammarDetailDict.get()];
}

void TextChecker::getGuessesForWord(int64_t spellDocumentTag, const String& word, const String& context, int32_t insertionPoint, Vector<String>& guesses, bool initialCapitalizationEnabled)
{
    NSString* language = nil;
    NSOrthography* orthography = nil;
    NSSpellChecker *checker = [NSSpellChecker sharedSpellChecker];
    NSDictionary *options = nil;
#if HAVE(ADVANCED_SPELL_CHECKING)
    options = @{ NSTextCheckingInsertionPointKey : @(insertionPoint),
                 WebTextCheckingSuppressInitialCapitalizationKey : @(!initialCapitalizationEnabled) };
#else
    options = @{ WebTextCheckingSuppressInitialCapitalizationKey : @(!initialCapitalizationEnabled) };
#endif
    if (context.length()) {
        [checker checkString:context range:NSMakeRange(0, context.length()) types:NSTextCheckingTypeOrthography options:options inSpellDocumentWithTag:spellDocumentTag orthography:&orthography wordCount:0];
        language = [checker languageForWordRange:NSMakeRange(0, context.length()) inString:context orthography:orthography];
    }
    NSArray* stringsArray = [checker guessesForWordRange:NSMakeRange(0, word.length()) inString:word language:language inSpellDocumentWithTag:spellDocumentTag];

    for (NSString *guess in stringsArray)
        guesses.append(guess);
}

void TextChecker::learnWord(int64_t, const String& word)
{
    [[NSSpellChecker sharedSpellChecker] learnWord:word];
}

void TextChecker::ignoreWord(int64_t spellDocumentTag, const String& word)
{
    [[NSSpellChecker sharedSpellChecker] ignoreWord:word inSpellDocumentWithTag:spellDocumentTag];
}

void TextChecker::requestCheckingOfString(Ref<TextCheckerCompletion>&&, int32_t)
{
    notImplemented();
}

} // namespace WebKit

#endif // PLATFORM(MAC)
