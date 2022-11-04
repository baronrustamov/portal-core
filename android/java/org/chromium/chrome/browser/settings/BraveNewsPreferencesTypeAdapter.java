/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.settings;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Button;

import androidx.core.content.ContextCompat;
import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;

import org.chromium.base.Log;
import org.chromium.brave_news.mojom.Channel;
import org.chromium.chrome.R;

import java.util.List;

public class BraveNewsPreferencesTypeAdapter extends RecyclerView.Adapter<RecyclerView.ViewHolder> {
	
    private Context mContext;
    private BraveNewsController mBraveNewsController;
	private String mBraveNewsPreferencesType;
	private List<Channel> mChannelsList;

	private static int TYPE_CHANNELS = 1;

	public BraveNewsPreferencesTypeAdapter(Context context, BraveNewsController braveNewsController, String braveNewsPreferencesType, List<Channel> channelsList) {
		mContext = context;
        mBraveNewsController = braveNewsController;
        mBraveNewsPreferencesType = braveNewsPreferencesType;
        mChannelsList = channelsList;
	}

	@Override
    public void onBindViewHolder(@NonNull RecyclerView.ViewHolder holder, int position) {

    	if (holder instanceof ChannelsViewHolder) {
            ChannelsViewHolder channelsViewHolder = (ChannelsViewHolder) holder;

            Channel channel = mChannelsList.get(position);
            channelsViewHolder.channelName.setText(channel.channelName);

            if(channel.subscribed) {

                channelsViewHolder.btnFollowUnFollow.setBackgroundResource(R.drawable.white_rounded_bg);
                channelsViewHolder.btnFollowUnFollow.setElevation(2f);
                channelsViewHolder.btnFollowUnFollow.setText(R.string.unfollow);
                channelsViewHolder.btnFollowUnFollow.setTextColor(ContextCompat.getColor(mContext, R.color.news_settings_subtitle_color));

            } else {

                channelsViewHolder.btnFollowUnFollow.setBackgroundResource(R.drawable.blue_48_rounded_bg);
                channelsViewHolder.btnFollowUnFollow.setElevation(0f);
                channelsViewHolder.btnFollowUnFollow.setText(R.string.follow);
                channelsViewHolder.btnFollowUnFollow.setTextColor(ContextCompat.getColor(mContext, android.R.color.white));
            }

            channelsViewHolder.btnFollowUnFollow.setOnClickListener(view -> {

                channel.subscribed = !channel.subscribed;

                notifyItemChanged(position);
            });
        }
    }

    @Override
    public int getItemCount() {
    	return mChannelsList.size();
    }

    @NonNull
    @Override
    public RecyclerView.ViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {

    	View view;
        //if (viewType == TYPE_CHANNELS) {
            view = LayoutInflater.from(parent.getContext())
                           .inflate(R.layout.item_news_settings_channels, parent, false);
            return new ChannelsViewHolder(view);
        //}
    }

    @Override
    public int getItemViewType(int position) {
		return TYPE_CHANNELS;
    }

    public static class ChannelsViewHolder extends RecyclerView.ViewHolder {
        ImageView imageChannel;
        TextView channelName;
        Button btnFollowUnFollow;

        ChannelsViewHolder(View itemView) {
            super(itemView);
            this.imageChannel = (ImageView) itemView.findViewById(R.id.iv_channel);
            this.channelName = (TextView) itemView.findViewById(R.id.tv_channel);
            this.btnFollowUnFollow = (Button) itemView.findViewById(R.id.btn_follow);
        }
    }
}