/*
 * Copyright (C) 2016 Apple Inc. All rights reserved.
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

WebInspector.HeapSnapshotProxy = class HeapSnapshotProxy extends WebInspector.Object
{
    constructor(snapshotObjectId, identifier, title, totalSize, totalObjectCount, liveSize, categories)
    {
        super();

        this._proxyObjectId = snapshotObjectId;

        this._identifier = identifier;
        this._title = title;
        this._totalSize = totalSize;
        this._totalObjectCount = totalObjectCount;
        this._liveSize = liveSize;
        this._categories = Map.fromObject(categories);

        console.assert(!this.invalid);

        if (!WebInspector.HeapSnapshotProxy.ValidSnapshotProxies)
            WebInspector.HeapSnapshotProxy.ValidSnapshotProxies = [];
        WebInspector.HeapSnapshotProxy.ValidSnapshotProxies.push(this);
    }

    // Static

    static deserialize(objectId, serializedSnapshot)
    {
        let {identifier, title, totalSize, totalObjectCount, liveSize, categories} = serializedSnapshot;
        return new WebInspector.HeapSnapshotProxy(objectId, identifier, title, totalSize, totalObjectCount, liveSize, categories);
    }

    static invalidateSnapshotProxies()
    {
        if (!WebInspector.HeapSnapshotProxy.ValidSnapshotProxies)
            return;

        for (let snapshotProxy of WebInspector.HeapSnapshotProxy.ValidSnapshotProxies)
            snapshotProxy._invalidate();

        WebInspector.HeapSnapshotProxy.ValidSnapshotProxies = null;
    }

    // Public

    get proxyObjectId() { return this._proxyObjectId; }
    get identifier() { return this._identifier; }
    get title() { return this._title; }
    get totalSize() { return this._totalSize; }
    get totalObjectCount() { return this._totalObjectCount; }
    get liveSize() { return this._liveSize; }
    get categories() { return this._categories; }
    get invalid() { return this._proxyObjectId === 0; }

    updateForCollectionEvent(event)
    {
        console.assert(!this.invalid);
        if (!event.data.affectedSnapshots.includes(this._identifier))
            return;

        this.update(() => {
            this.dispatchEventToListeners(WebInspector.HeapSnapshotProxy.Event.CollectedNodes, event.data);
        });
    }

    allocationBucketCounts(bucketSizes, callback)
    {
        console.assert(!this.invalid);
        WebInspector.HeapSnapshotWorkerProxy.singleton().callMethod(this._proxyObjectId, "allocationBucketCounts", bucketSizes, callback);
    }

    instancesWithClassName(className, callback)
    {
        console.assert(!this.invalid);
        WebInspector.HeapSnapshotWorkerProxy.singleton().callMethod(this._proxyObjectId, "instancesWithClassName", className, (serializedNodes) => {
            callback(serializedNodes.map(WebInspector.HeapSnapshotNodeProxy.deserialize.bind(null, this._proxyObjectId)));
        });
    }

    update(callback)
    {
        console.assert(!this.invalid);
        WebInspector.HeapSnapshotWorkerProxy.singleton().callMethod(this._proxyObjectId, "update", ({liveSize, categories}) => {
            this._liveSize = liveSize;
            this._categories = Map.fromObject(categories);
            callback();
        });
    }

    nodeWithIdentifier(nodeIdentifier, callback)
    {
        console.assert(!this.invalid);
        WebInspector.HeapSnapshotWorkerProxy.singleton().callMethod(this._proxyObjectId, "nodeWithIdentifier", nodeIdentifier, (serializedNode) => {
            callback(WebInspector.HeapSnapshotNodeProxy.deserialize(this._proxyObjectId, serializedNode));
        });
    }

    // Private

    _invalidate()
    {
        this._proxyObjectId = 0;
        this._liveSize = 0;

        this.dispatchEventToListeners(WebInspector.HeapSnapshotProxy.Event.Invalidated);
    }
};

WebInspector.HeapSnapshotProxy.Event = {
    CollectedNodes: "heap-snapshot-proxy-collected-nodes",
    Invalidated: "heap-snapshot-proxy-invalidated",
};
