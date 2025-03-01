/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

import * as React from 'react'

import { AdaptiveCaptchaInfo, Host, HostState } from '../lib/interfaces'
import { AdaptiveCaptchaView } from '../components/adaptive_captcha_view'
import { Notification } from '../../shared/components/notifications'
import { localeStrings } from './locale_strings'
import { createStateManager } from '../../shared/lib/state_manager'

import { LocaleContext, createLocaleContextForTesting } from '../../shared/lib/locale_context'
import { WithThemeVariables } from '../../shared/components/with_theme_variables'
import { NotificationCard } from '../components/notification_card'

import { App } from '../components/app'

import grantCaptchaImageURL from './grant_captcha_image.png'
import * as mojom from '../../shared/lib/mojom'

export default {
  title: 'Rewards/Panel'
}

const locale = createLocaleContextForTesting(localeStrings)

function actionLogger (name: string) {
  return (...args: any[]) => {
    console.log(name, ...args)
  }
}

function createHost (): Host {
  const stateManager = createStateManager<HostState>({
    openTime: Date.now(),
    loading: false,
    requestedView: null,
    rewardsEnabled: true,
    settings: {
      adsEnabled: true,
      adsPerHour: 3,
      autoContributeEnabled: true,
      autoContributeAmount: 5
    },
    options: {
      autoContributeAmounts: [1, 5, 10, 15],
      externalWalletRegions: new Map([
        ['uphold', { allow: ['US'], block: [] }],
        ['gemini', { allow: [], block: ['US'] }]
      ])
    },
    grantCaptchaInfo: null && {
      id: '123',
      imageURL: grantCaptchaImageURL,
      hint: 'square',
      status: 'pending',
      verifying: false,
      grantInfo: {
        id: 'grant123',
        createdAt: Date.now(),
        claimableUntil: Date.now() + 120_000,
        expiresAt: Date.now() + 120_000,
        amount: 10,
        type: 'ads'
      }
    },
    adaptiveCaptchaInfo: null && {
      url: '',
      status: 'pending'
    },
    externalWalletProviders: ['uphold', 'gemini'],
    balance: 10.2,
    exchangeInfo: {
      rate: 0.75,
      currency: 'USD'
    },
    earningsInfo: {
      earningsThisMonth: 1.2,
      earningsLastMonth: 2.4,
      nextPaymentDate: Date.now() + 1000 * 60 * 60 * 24 * 3
    },
    payoutStatus: {
      uphold: 'complete'
    },
    publisherInfo: {
      id: 'brave.com',
      name: 'brave.com',
      icon: 'https://brave.com/static-assets/images/brave-favicon.png',
      platform: null,
      attentionScore: 0.17,
      autoContributeEnabled: true,
      monthlyTip: 5,
      supportedWalletProviders: []
    },
    publisherRefreshing: false,
    externalWallet: null && {
      provider: 'uphold',
      username: 'brave123',
      status: mojom.WalletStatus.kConnected,
      links: {}
    },
    summaryData: {
      adEarnings: 10,
      autoContributions: 10,
      oneTimeTips: -2,
      monthlyTips: -19,
      pendingTips: 0
    },
    notifications: [
      {
        type: 'monthly-tip-completed',
        id: '1',
        timeStamp: Date.now() - 100
      }
    ] && [],
    availableCountries: ['US'],
    declaredCountry: 'US',
    userType: 'unconnected',
    publishersVisitedCount: 4
  })

  return {
    get state () { return stateManager.getState() },

    addListener: stateManager.addListener,

    refreshPublisherStatus () {
      console.log('refreshPublisherStatus')
    },

    enableRewards () {
      stateManager.update({
        rewardsEnabled: true,
        declaredCountry: 'US'
      })
      return Promise.resolve('success')
    },

    setAdsEnabled (adsEnabled) {
      stateManager.update({
        settings: {
          ...stateManager.getState().settings,
          adsEnabled
        }
      })
    },

    setAdsPerHour (adsPerHour) {
      stateManager.update({
        settings: {
          ...stateManager.getState().settings,
          adsPerHour
        }
      })
    },

    setIncludeInAutoContribute (enabled) {
      const { publisherInfo } = stateManager.getState()
      if (publisherInfo) {
        stateManager.update({
          publisherInfo: {
            ...publisherInfo,
            autoContributeEnabled: enabled
          }
        })
      }
    },

    openAdaptiveCaptchaSupport () {
      console.log('openAdaptiveCaptchaSupport')
    },

    openRewardsSettings () {
      console.log('openRewardsSettings')
    },

    sendTip () {
      console.log('sendTip')
    },

    handleMonthlyTipAction (action) {
      switch (action) {
        case 'update': {
          console.log('updateMonthlyTip')
          break
        }
        case 'cancel': {
          const { publisherInfo } = stateManager.getState()
          if (publisherInfo) {
            stateManager.update({
              publisherInfo: {
                ...publisherInfo,
                monthlyTip: 0
              }
            })
          }
        }
      }
    },

    handleExternalWalletAction (action) {
      console.log('externalWalletAction', action)
    },

    handleNotificationAction (action) {
      console.log('notificationAction', action)
    },

    dismissNotification (notification) {
      const { notifications } = stateManager.getState()
      stateManager.update({
        notifications: notifications.filter((n) => {
          return n.id !== notification.id
        })
      })
    },

    solveGrantCaptcha (solution) {
      console.log('solveGrantCaptcha', solution)
      const { grantCaptchaInfo } = stateManager.getState()
      if (!grantCaptchaInfo) {
        return
      }
      stateManager.update({
        grantCaptchaInfo: {
          ...grantCaptchaInfo,
          status: 'passed'
        }
      })
    },

    clearGrantCaptcha () {
      stateManager.update({
        grantCaptchaInfo: null
      })
    },

    clearAdaptiveCaptcha () {
      stateManager.update({
        adaptiveCaptchaInfo: null
      })
    },

    handleAdaptiveCaptchaResult (result) {
      const { adaptiveCaptchaInfo } = stateManager.getState()
      if (!adaptiveCaptchaInfo) {
        return
      }

      console.log('handleAdaptiveCaptcha', result)

      switch (result) {
        case 'success':
          stateManager.update({
            adaptiveCaptchaInfo: { ...adaptiveCaptchaInfo, status: 'success' }
          })
          break
        case 'failure':
        case 'error':
          break
      }
    },
    onAppRendered () {
      console.log('onAppRendered')
    }
  }
}

export function MainPanel () {
  const [host] = React.useState(() => createHost())
  return (
    <div className='brave-theme-dark'>
      <LocaleContext.Provider value={locale}>
        <App host={host} />
      </LocaleContext.Provider>
    </div>
  )
}

export function Notification () {
  return (
    <LocaleContext.Provider value={locale}>
      <WithThemeVariables>
        <div style={{ width: '375px' }}>
          <NotificationCard
            notification={{
              type: 'monthly-tip-completed',
              id: '123',
              timeStamp: Date.now()
            }}
          />
        </div>
      </WithThemeVariables>
    </LocaleContext.Provider>
  )
}

export function GrantNotification () {
  return (
    <LocaleContext.Provider value={locale}>
      <WithThemeVariables>
        <div style={{ width: '375px' }}>
          <NotificationCard
            notification={{
              type: 'grant-available',
              id: '123',
              grantInfo: {
                id: '123',
                type: 'ads',
                amount: 1.25,
                createdAt: Date.now(),
                expiresAt: Date.now() + 1000 * 60 * 60 * 24 * 5
              },
              timeStamp: Date.now()
            } as any}
          />
        </div>
      </WithThemeVariables>
    </LocaleContext.Provider>
  )
}

export function AdaptiveCaptcha () {
  const adaptiveCaptchaInfo: AdaptiveCaptchaInfo = {
    url: '',
    status: 'pending'
  }
  return (
    <LocaleContext.Provider value={locale}>
      <WithThemeVariables>
        <div>
          <AdaptiveCaptchaView
            adaptiveCaptchaInfo={adaptiveCaptchaInfo}
            onClose={actionLogger('onClose')}
            onCaptchaResult={actionLogger('onCaptchaResult')}
            onContactSupport={actionLogger('onContactSupport')}
          />
        </div>
      </WithThemeVariables>
    </LocaleContext.Provider>
  )
}
