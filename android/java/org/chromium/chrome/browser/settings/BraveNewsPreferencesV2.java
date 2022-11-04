/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.settings;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;
import android.os.Bundle;
import android.widget.CompoundButton;
import androidx.appcompat.widget.SwitchCompat;

import org.chromium.base.Log;
import org.chromium.chrome.R;
import org.chromium.brave_news.mojom.BraveNewsController;
import org.chromium.brave_news.mojom.Channel;
import org.chromium.brave_news.mojom.Publisher;
import java.util.Map;
import org.chromium.chrome.browser.brave_news.BraveNewsControllerFactory;
import org.chromium.chrome.browser.settings.BravePreferenceFragment;
import org.chromium.chrome.browser.preferences.BravePrefServiceBridge;
import org.chromium.chrome.browser.preferences.BravePreferenceKeys;
import org.chromium.chrome.browser.preferences.SharedPreferencesManager;
import org.chromium.chrome.browser.util.BraveConstants;
import org.chromium.mojo.bindings.ConnectionErrorHandler;
import org.chromium.mojo.system.MojoException;
import org.chromium.components.browser_ui.settings.FragmentSettingsLauncher;
import org.chromium.components.browser_ui.settings.SettingsLauncher;
import android.content.Intent;

public class BraveNewsPreferencesV2 extends BravePreferenceFragment implements ConnectionErrorHandler, FragmentSettingsLauncher {

    private SwitchCompat mSwitchShowNews;
    private View mDivider;
    private TextView mTvSearch;
    private TextView mTvFollowingCount;
    private View mLayoutPopularSources;
    private View mLayoutSuggested;
    private View mLayoutChannels;
    private View mLayoutFollowing;

    private BraveNewsController mBraveNewsController;

    // SettingsLauncher injected from main Settings Activity.
    private SettingsLauncher mSettingsLauncher;

    @Override
    public View onCreateView(
            LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        
        return inflater.inflate(R.layout.brave_news_settings, container, false);
    }

    @Override
    public void onActivityCreated(Bundle savedInstanceState) {
        getActivity().setTitle(R.string.brave_news_title);

        super.onActivityCreated(savedInstanceState);

        initBraveNewsController();

        mSwitchShowNews = (SwitchCompat) getView().findViewById(R.id.switch_show_news);
        mDivider = getView().findViewById(R.id.divider);
        mTvSearch = (TextView) getView().findViewById(R.id.tv_search);
        mTvFollowingCount = (TextView) getView().findViewById(R.id.tv_following_count);
        mLayoutPopularSources = (View) getView().findViewById(R.id.layout_popular_sources);
        mLayoutSuggested = (View) getView().findViewById(R.id.layout_suggested);
        mLayoutChannels = (View) getView().findViewById(R.id.layout_channels);
        mLayoutFollowing = (View) getView().findViewById(R.id.layout_following);

        onClickViews();

        boolean isShowNewsOn = BravePrefServiceBridge.getInstance().getShowNews();
        mSwitchShowNews.setChecked(isShowNewsOn);
        //getBraveNewsData();
    }

    private void onClickViews() {

        mSwitchShowNews.setOnCheckedChangeListener((compoundButton, b) -> {
            onShowNewsToggle(b);
        });

        mLayoutPopularSources.setOnClickListener(view -> {

        });

        mLayoutSuggested.setOnClickListener(view -> {

        });

        mLayoutChannels.setOnClickListener(view -> {

            Bundle fragmentArgs = new Bundle();
            fragmentArgs.putString(BraveConstants.BRAVE_NEWS_PREFERENCES_TYPE, BraveNewsPreferencesType.Channels.toString());
            Intent intent = mSettingsLauncher.createSettingsActivityIntent(
                getActivity(), BraveNewsPreferencesDetails.class.getName(), fragmentArgs);
            startActivity(intent);
        });

        mLayoutFollowing.setOnClickListener(view -> {

        });
    }

    private void onShowNewsToggle(boolean isEnable) {

        Log.e("tapan","onShowNewsToggle:"+isEnable);
        BravePrefServiceBridge.getInstance().setShowNews(isEnable);
        
        SharedPreferencesManager.getInstance().writeBoolean(BravePreferenceKeys.BRAVE_NEWS_PREF_SHOW_NEWS, isEnable);
        
        if(isEnable) {
        
            BravePrefServiceBridge.getInstance().setNewsOptIn(true);

            mTvSearch.setVisibility(View.VISIBLE);
            mLayoutPopularSources.setVisibility(View.VISIBLE);
            mLayoutSuggested.setVisibility(View.VISIBLE);
            mLayoutChannels.setVisibility(View.VISIBLE);
            mLayoutFollowing.setVisibility(View.VISIBLE);

        } else {

            mTvSearch.setVisibility(View.GONE);
            mLayoutPopularSources.setVisibility(View.GONE);
            mLayoutSuggested.setVisibility(View.GONE);
            mLayoutChannels.setVisibility(View.GONE);
            mLayoutFollowing.setVisibility(View.GONE);
        }
    }

    private void getBraveNewsData() {

        mBraveNewsController.getLocale((locale) -> {
            Log.e("tapan","locale:"+locale);
        });
        
        mBraveNewsController.getChannels((channels) -> {
            Log.e("tapan","channels:"+channels.size());

            for (Map.Entry<String, Channel> entry : channels.entrySet()) {
                Channel channel = entry.getValue();
                Log.e("tapan","channelName:"+channel.channelName+", subscribed:"+channel.subscribed);
            }
        });

        mBraveNewsController.getPublishers((publishers) -> {
            Log.e("tapan","publishers:"+publishers.size());

            for (Map.Entry<String, Publisher> entry : publishers.entrySet()) {
                Publisher publisher = entry.getValue();
                Log.e("tapan","categoryName:"+publisher.categoryName+", publisherName:"+publisher.publisherName+", id:"+publisher.publisherId);
            }
        });

        mBraveNewsController.getSuggestedPublisherIds((suggestedPublisherIds) -> {
            Log.e("tapan","suggestedPublisherIds:"+suggestedPublisherIds.length);

            for (String suggested : suggestedPublisherIds) {
                Log.e("tapan","suggested:"+suggested);
            }
        });
    }

    private void initBraveNewsController() {
        if (mBraveNewsController != null) {
            return;
        }

        mBraveNewsController =
                BraveNewsControllerFactory.getInstance().getBraveNewsController(this);
    }

    @Override
    public void setSettingsLauncher(SettingsLauncher settingsLauncher) {
        Log.e("tapan","settingsLauncher");
        mSettingsLauncher = settingsLauncher;
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
