/*
 * Copyright (C) 2015 Apple Inc. All rights reserved.
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

WebInspector.StorageTabContentView = class StorageTabContentView extends WebInspector.ContentBrowserTabContentView
{
    constructor(identifier)
    {
        let {image, title} = WebInspector.StorageTabContentView.tabInfo();
        let tabBarItem = new WebInspector.GeneralTabBarItem(image, title);
        let detailsSidebarPanels = [WebInspector.applicationCacheDetailsSidebarPanel, WebInspector.indexedDatabaseDetailsSidebarPanel];

        super(identifier || "storage", "storage", tabBarItem, WebInspector.StorageSidebarPanel, detailsSidebarPanels);
    }

    static tabInfo()
    {
        return {
            image: "Images/Storage.svg",
            title: WebInspector.UIString("Storage"),
        };
    }

    static isTabAllowed()
    {
        return !!window.DOMStorageAgent || !!window.DatabaseAgent || !!window.IndexedDBAgent;
    }

    // Public

    get type()
    {
        return WebInspector.StorageTabContentView.Type;
    }

    get supportsSplitContentBrowser()
    {
        return true;
    }

    canShowRepresentedObject(representedObject)
    {
        return representedObject instanceof WebInspector.DOMStorageObject || representedObject instanceof WebInspector.CookieStorageObject ||
            representedObject instanceof WebInspector.DatabaseTableObject || representedObject instanceof WebInspector.DatabaseObject ||
            representedObject instanceof WebInspector.ApplicationCacheFrame || representedObject instanceof WebInspector.IndexedDatabaseObjectStore ||
            representedObject instanceof WebInspector.IndexedDatabase || representedObject instanceof WebInspector.IndexedDatabaseObjectStoreIndex;
    }
};

WebInspector.StorageTabContentView.Type = "storage";
