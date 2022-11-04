/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.settings;

import android.view.LayoutInflater;
import android.view.View;
import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.view.ViewGroup;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;
import androidx.recyclerview.widget.DividerItemDecoration;
import androidx.core.content.ContextCompat;

import org.chromium.base.Log;
import org.chromium.chrome.R;
import org.chromium.brave_news.mojom.BraveNewsController;
import org.chromium.chrome.browser.brave_news.BraveNewsControllerFactory;
import org.chromium.chrome.browser.util.BraveConstants;
import org.chromium.brave_news.mojom.Channel;
import org.chromium.brave_news.mojom.Publisher;
import org.chromium.chrome.browser.settings.BravePreferenceFragment;
import org.chromium.mojo.bindings.ConnectionErrorHandler;
import org.chromium.mojo.system.MojoException;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

public class BraveNewsPreferencesDetails extends BravePreferenceFragment implements ConnectionErrorHandler {

    private BraveNewsController mBraveNewsController;

    private RecyclerView mRecyclerView;
    private String mBraveNewsPreferencesType;

    @Override
    public View onCreateView(
            LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        
        return inflater.inflate(R.layout.brave_news_settings_details, container, false);
    }

    @Override
    public void onActivityCreated(Bundle savedInstanceState) {
        getActivity().setTitle(R.string.channels);

        super.onActivityCreated(savedInstanceState);

        initBraveNewsController();

        mRecyclerView = (RecyclerView) getView().findViewById(R.id.recyclerview);

        mBraveNewsPreferencesType = getArguments().getString(BraveConstants.BRAVE_NEWS_PREFERENCES_TYPE);
        Log.e("tapan","mBraveNewsPreferencesType:"+mBraveNewsPreferencesType);
        getBraveNewsData();
    }

    private void getBraveNewsData() {

        if(mBraveNewsPreferencesType.equalsIgnoreCase(BraveNewsPreferencesType.Channels.toString())) {

            if(mBraveNewsController!=null) {
                mBraveNewsController.getChannels((channels) -> {

                    List<Channel> channelsList = new ArrayList<>();
                    for (Map.Entry<String, Channel> entry : channels.entrySet()) {
                        Channel channel = entry.getValue();
                        channelsList.add(channel);
                        Log.e("tapan","channelName:"+channel.channelName+", subscribed:"+channel.subscribed);
                    }

                    setRecyclerView(channelsList);
                });
            }
        }
    }

    private void setRecyclerView(List<Channel> channelsList) {

        LinearLayoutManager linearLayoutManager =
                new LinearLayoutManager(getActivity(), LinearLayoutManager.VERTICAL, false);
        mRecyclerView.setLayoutManager(linearLayoutManager);
        BraveNewsPreferencesTypeAdapter adapter = new BraveNewsPreferencesTypeAdapter(getActivity(), mBraveNewsController, mBraveNewsPreferencesType, channelsList);
        mRecyclerView.setAdapter(adapter);

        DividerItemDecoration horizontalDecoration = new DividerItemDecoration(mRecyclerView.getContext(),
                DividerItemDecoration.VERTICAL);
        Drawable horizontalDivider = ContextCompat.getDrawable(getActivity(), R.drawable.brave_news_settings_list_divider);
        horizontalDecoration.setDrawable(horizontalDivider);
        mRecyclerView.addItemDecoration(horizontalDecoration);
    }

    private void initBraveNewsController() {
        if (mBraveNewsController != null) {
            return;
        }

        mBraveNewsController =
                BraveNewsControllerFactory.getInstance().getBraveNewsController(this);
    }

    @Override
    public void onConnectionError(MojoException e) {
        if (mBraveNewsController != null) {
            mBraveNewsController.close();
        }
        mBraveNewsController = null;
        initBraveNewsController();
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        if (mBraveNewsController != null) {
            mBraveNewsController.close();
        }
    }
}
