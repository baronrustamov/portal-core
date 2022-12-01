/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.brave_news;

import android.content.Context;
import android.util.Pair;

import org.chromium.base.Log;
import org.chromium.brave_news.mojom.Article;
import org.chromium.brave_news.mojom.BraveNewsController;
import org.chromium.brave_news.mojom.CardType;
import org.chromium.brave_news.mojom.Channel;
import org.chromium.brave_news.mojom.Deal;
import org.chromium.brave_news.mojom.DisplayAd;
import org.chromium.brave_news.mojom.FeedItem;
import org.chromium.brave_news.mojom.FeedItemMetadata;
import org.chromium.brave_news.mojom.LocaleInfo;
import org.chromium.brave_news.mojom.PromotedArticle;
import org.chromium.brave_news.mojom.Publisher;
import org.chromium.brave_news.mojom.PublisherType;
import org.chromium.brave_news.mojom.UserEnabled;
import org.chromium.chrome.R;
import org.chromium.chrome.browser.brave_news.models.FeedItemCard;
import org.chromium.chrome.browser.brave_news.models.FeedItemsCard;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.List;
import java.util.Locale;
import java.util.Map;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class BraveNewsUtils {
    public static final int BRAVE_NEWS_VIEWD_CARD_TIME = 1000; // milliseconds
    private static Map<Integer, DisplayAd> sCurrentDisplayAds;
    private static String mLocale;
    private static List<Channel> mChannelList;
    private static List<Publisher> mGlobalPublisherList;
    private static List<Publisher> mPublisherList;
    private static List<Channel> mFollowingChannelList;
    private static List<Publisher> mFollowingPublisherList;
    // private static List<Publisher> mFollowingRssList;
    private static List<String> mSuggestedList;
    private static HashMap<String, Integer> mChannelIcons = new HashMap<>();

    public static String getPromotionIdItem(FeedItemsCard items) {
        String creativeInstanceId = "null";
        if (items.getFeedItems() != null) {
            for (FeedItemCard itemCard : items.getFeedItems()) {
                FeedItem item = itemCard.getFeedItem();
                FeedItemMetadata itemMetaData = new FeedItemMetadata();
                if (item.which() == FeedItem.Tag.PromotedArticle) {
                    PromotedArticle promotedArticle = item.getPromotedArticle();
                    creativeInstanceId = promotedArticle.creativeInstanceId;
                    break;
                }
            }
        }

        return creativeInstanceId;
    }

    public static void initCurrentAds() {
        sCurrentDisplayAds = new HashMap<>();
    }

    public static void putToDisplayAdsMap(Integer index, DisplayAd ad) {
        if (sCurrentDisplayAds == null) {
            sCurrentDisplayAds = new HashMap<>();
        }
        sCurrentDisplayAds.put(index, ad);
    }

    public static DisplayAd getFromDisplayAdsMap(Integer index) {
        if (sCurrentDisplayAds != null) {
            DisplayAd foundItem = sCurrentDisplayAds.get(index);
            return foundItem;
        }

        return null;
    }

    // method for logging news object. works by putting Log.d in the desired places of the parsing
    // of the object
    public static void logFeedItem(FeedItemsCard items, String id) {
        if (items != null) {
            if (items.getCardType() == CardType.DISPLAY_AD) {
                DisplayAd displayAd = items.getDisplayAd();
                if (displayAd != null) {
                    Log.d("bn", id + " DISPLAY_AD title: " + displayAd.title);
                }
            } else {
                if (items.getFeedItems() != null) {
                    int index = 0;
                    for (FeedItemCard itemCard : items.getFeedItems()) {
                        if (index > 50) {
                            return;
                        }
                        FeedItem item = itemCard.getFeedItem();

                        switch (item.which()) {
                            case FeedItem.Tag.Article:
                                Article article = item.getArticle();
                                FeedItemMetadata articleData = article.data;
                                break;
                            case FeedItem.Tag.PromotedArticle:
                                PromotedArticle promotedArticle = item.getPromotedArticle();
                                FeedItemMetadata promotedArticleData = promotedArticle.data;
                                String creativeInstanceId = promotedArticle.creativeInstanceId;
                                break;
                            case FeedItem.Tag.Deal:
                                Deal deal = item.getDeal();
                                FeedItemMetadata dealData = deal.data;
                                String offersCategory = deal.offersCategory;
                                break;
                        }
                        index++;
                    }
                }
            }
        }
    }

    public static void setChannelIcons() {
        mChannelIcons.put("Brave", R.drawable.ic_channel_brave);
        mChannelIcons.put("Business", R.drawable.ic_channel_business);
        mChannelIcons.put("Cars", R.drawable.ic_channel_cars);
        mChannelIcons.put("Crypto", R.drawable.ic_channel_crypto);
        mChannelIcons.put("Culture", R.drawable.ic_channel_culture);
        mChannelIcons.put("Entertainment", R.drawable.ic_channel_entertainment);
        mChannelIcons.put("Fashion", R.drawable.ic_channel_fashion);
        mChannelIcons.put("Film and TV", R.drawable.ic_channel_filmtv);
        mChannelIcons.put("Food", R.drawable.ic_channel_food);
        mChannelIcons.put("Fun", R.drawable.ic_channel_fun);
        mChannelIcons.put("Gaming", R.drawable.ic_channel_gaming);
        mChannelIcons.put("Health", R.drawable.ic_channel_health);
        mChannelIcons.put("Home", R.drawable.ic_channel_home);
        mChannelIcons.put("Music", R.drawable.ic_channel_music);
        mChannelIcons.put("Politics", R.drawable.ic_channel_politics);
        mChannelIcons.put("Science", R.drawable.ic_channel_science);
        mChannelIcons.put("Sports", R.drawable.ic_channel_sports);
        mChannelIcons.put("Technology", R.drawable.ic_channel_technology);
        mChannelIcons.put("Tech News", R.drawable.ic_channel_technology);
        mChannelIcons.put("Tech Reviews", R.drawable.ic_channel_technology);
        mChannelIcons.put("Top News", R.drawable.ic_channel_top_news);
        mChannelIcons.put("Travel", R.drawable.ic_channel_travel);
        mChannelIcons.put("US News", R.drawable.ic_channel_usnews);
        mChannelIcons.put("Weather", R.drawable.ic_channel_weather);
        mChannelIcons.put("World News", R.drawable.ic_channel_world_news);
    }

    public static HashMap<String, Integer> getChannelIcons() {
        return mChannelIcons;
    }

    public static void setLocale(String locale) {
        mLocale = locale;
    }

    public static String getLocale() {
        return mLocale;
    }

    public static void setChannelList(List<Channel> channelList) {
        mChannelList = channelList;
        setFollowingChannelList();
    }

    public static List<Channel> getChannelList() {
        return mChannelList;
    }

    public static void setPopularSources(List<Publisher> publisherList) {
        mPublisherList = publisherList;
        setFollowingPublisherList();
    }

    public static List<Publisher> getPopularSources() {
        return mPublisherList;
    }

    public static List<Publisher> getGlobalSources() {
        return mGlobalPublisherList;
    }

    public static void setSuggestedIds(List<String> suggestedList) {
        mSuggestedList = suggestedList;
    }

    public static List<Publisher> getSuggestedSources() {
        List<Publisher> suggestedPublisherList = new ArrayList<>();
        for (Publisher publisher : mPublisherList) {
            if (mSuggestedList.contains(publisher.publisherId)) {
                suggestedPublisherList.add(publisher);
            }
        }
        return suggestedPublisherList;
    }

    public static void updatePublishers(String publisherId, int userEnabled) {
        for (Publisher publisher : mPublisherList) {
            if (publisher.publisherId.equals(publisherId)) {
                publisher.userEnabledStatus = userEnabled;
                break;
            }
        }
    }

    public static void setFollowingPublisherList() {
        List<Publisher> publisherList = new ArrayList<>();
        // List<Publisher> rssList = new ArrayList<>();
        for (Publisher publisher : mGlobalPublisherList) {
            if (publisher.userEnabledStatus == UserEnabled.ENABLED
                    || (publisher.type == PublisherType.DIRECT_SOURCE
                            && publisher.userEnabledStatus != UserEnabled.DISABLED)) {
                /*rssList.add(publisher);
            } else if (publisher.userEnabledStatus == UserEnabled.ENABLED) {*/
                publisherList.add(publisher);
            }
        }
        mFollowingPublisherList = publisherList;
        // mFollowingRssList = rssList;
    }

    /*public static void updateFollowingPublisher(Publisher publisher) {

        if(publisher.userEnabledStatus == UserEnabled.ENABLED &&
    !mFollowingPublisherList.contains(publisher)) { mFollowingPublisherList.add(publisher); } else
    if(publisher.userEnabledStatus == UserEnabled.DISABLED &&
    mFollowingPublisherList.contains(publisher)) { mFollowingPublisherList.remove(publisher);
        }
    }*/

    public static List<Publisher> getFollowingPublisherList() {
        return mFollowingPublisherList;
    }

    /*public static List<Publisher> getFollowingRssList() {
        return mFollowingRssList;
    }*/

    public static void setFollowingChannelList() {
        List<Channel> channelList = new ArrayList<>();
        for (Channel channel : mChannelList) {
            List<String> subscribedLocalesList =
                    new ArrayList<>(Arrays.asList(channel.subscribedLocales));
            if (subscribedLocalesList.contains(mLocale)) {
                channelList.add(channel);
            }
        }
        mFollowingChannelList = channelList;
    }

    /*public static void updateFollowingChannel(Channel channel) {

        if(publisher.userEnabledStatus == UserEnabled.ENABLED &&
    !mFollowingChannelsList.contains(channel)) { mFollowingChannelsList.add(channel); } else
    if(publisher.userEnabledStatus == UserEnabled.DISABLED &&
    mFollowingChannelsList.contains(channel)) { mFollowingChannelsList.remove(channel);
        }
    }*/

    public static List<Channel> getFollowingChannelList() {
        return mFollowingChannelList;
    }

    public static List<Channel> searchChannel(String search) {
        List<Channel> channelList = new ArrayList<>();
        for (Channel channel : mChannelList) {
            if (channel.channelName.toLowerCase(Locale.ROOT).contains(search)) {
                channelList.add(channel);
            }
        }
        return channelList;
    }

    public static List<Publisher> /*Pair<List<Publisher>, List<Publisher>>*/ searchPublisher(
            String search) {
        List<Publisher> publisherList = new ArrayList<>();
        // List<Publisher> rssList = new ArrayList<>();
        for (Publisher publisher : mGlobalPublisherList) {
            if (publisher.publisherName.toLowerCase(Locale.ROOT).contains(search)
                    || publisher.categoryName.toLowerCase(Locale.ROOT).contains(search)
                    || publisher.feedSource.url.toLowerCase(Locale.ROOT).contains(search)
                    || publisher.siteUrl.url.toLowerCase(Locale.ROOT).contains(search)) {
                /*if (publisher.type == PublisherType.DIRECT_SOURCE) {
                    rssList.add(publisher);
                } else {*/
                publisherList.add(publisher);
                //}
            }
        }

        return publisherList; // new Pair(publisherList, rssList);
    }

    public static boolean searchPublisherForRss(String feedUrl) {
        boolean isFound = false;
        for (Publisher publisher : mGlobalPublisherList) {
            if (publisher.feedSource.url.equalsIgnoreCase(feedUrl)
                    || publisher.siteUrl.url.equalsIgnoreCase(feedUrl)) {
                isFound = true;
                break;
            }
        }
        return isFound;
    }

    public static void getBraveNewsSettingsData(BraveNewsController braveNewsController) {
        ExecutorService executors = Executors.newFixedThreadPool(1);
        Runnable runnable = new Runnable() {
            @Override
            public void run() {
                if (braveNewsController != null) {
                    Log.e("tapan", "before getLocale");
                    braveNewsController.getLocale((locale) -> {
                        Log.e("tapan", "after getLocale");
                        setLocale(locale);
                        getChannels(braveNewsController);
                        getPublishers(braveNewsController);
                        getSuggestedSources(braveNewsController);
                    });
                }
            }
        };

        executors.submit(runnable);
    }

    private static void getChannels(BraveNewsController braveNewsController) {
        Log.e("tapan", "before getChannels");
        braveNewsController.getChannels((channels) -> {
            Log.e("tapan", "after getChannels");
            List<Channel> channelList = new ArrayList<>();
            for (Map.Entry<String, Channel> entry : channels.entrySet()) {
                Channel channel = entry.getValue();
                channelList.add(channel);
            }

            Comparator<Channel> compareByName = (Channel o1, Channel o2)
                    -> o1.channelName.toLowerCase(Locale.ROOT)
                               .compareTo(o2.channelName.toLowerCase(Locale.ROOT));
            Collections.sort(channelList, compareByName);

            setChannelList(channelList);
        });
    }

    private static void getPublishers(BraveNewsController braveNewsController) {
        Log.e("tapan", "before getPublishers");
        braveNewsController.getPublishers((publishers) -> {
            Log.e("tapan", "after getPublishers");
            setPublishers(publishers);
        });
    }

    public static void setPublishers(Map<String, Publisher> publishers) {
        List<Publisher> globalPublisherList = new ArrayList<>();
        List<Publisher> publisherList = new ArrayList<>();
        HashMap<String, LocaleInfo> localesMap = new HashMap<>();
        for (Map.Entry<String, Publisher> entry : publishers.entrySet()) {
            Publisher publisher = entry.getValue();
            globalPublisherList.add(publisher);
            for (LocaleInfo localeInfo : publisher.locales) {
                if (localeInfo.locale.equals(mLocale)) {
                    localesMap.put(publisher.publisherId, localeInfo);
                    publisherList.add(publisher);
                    break;
                }
            }
        }

        Comparator<Publisher> compareByName = (Publisher o1, Publisher o2)
                -> o1.publisherName.toLowerCase(Locale.ROOT)
                           .compareTo(o2.publisherName.toLowerCase(Locale.ROOT));

        Collections.sort(globalPublisherList, compareByName);

        mGlobalPublisherList = globalPublisherList;

        Collections.sort(publisherList, compareByName);

        Comparator<Publisher> compareByRank = (Publisher o1, Publisher o2)
                -> Integer.compare(
                        localesMap.get(o1.publisherId).rank, localesMap.get(o2.publisherId).rank);

        Collections.sort(publisherList, compareByRank);

        setPopularSources(publisherList);
    }

    private static void getSuggestedSources(BraveNewsController braveNewsController) {
        Log.e("tapan", "before getSuggestedPublisherIds");
        braveNewsController.getSuggestedPublisherIds((publisherIds) -> {
            Log.e("tapan", "after getSuggestedPublisherIds");
            setSuggestedIds(Arrays.asList(publisherIds));
        });
    }
}
