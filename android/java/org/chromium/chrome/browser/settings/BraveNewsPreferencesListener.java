/**
 * Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

package org.chromium.chrome.browser.settings;

import org.chromium.brave_news.mojom.Channel;
import org.chromium.url.mojom.Url;

public interface BraveNewsPreferencesListener {

    public void onChannelSubscribed(int position, Channel channel, boolean isSubscribed);
    public void onPublisherPref(String publisherId, int userEnabled);
    public void findFeeds(String url);
    public void subscribeToNewDirectFeed(int position, Url feedUrl, boolean isFromFeed);
}
