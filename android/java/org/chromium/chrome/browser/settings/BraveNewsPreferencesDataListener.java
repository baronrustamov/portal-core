/**
 * Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

package org.chromium.chrome.browser.settings;

public interface BraveNewsPreferencesDataListener {
    public void onChannelReceived();
    public void onPublisherReceived();
    public void onSuggestionsReceived();
}
