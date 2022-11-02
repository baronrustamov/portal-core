/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import { createReducer } from 'redux-act'
import * as Actions from '../actions/wallet_page_actions'
import {
  BraveWallet,
  PageState,
  NFTMetadataReturnType,
  ImportAccountErrorType
} from '../../constants/types'
import {
  WalletCreatedPayloadType,
  RecoveryWordsAvailablePayloadType,
  SelectAssetPayloadType,
  ImportWalletErrorPayloadType,
  ShowRecoveryPhrasePayload
} from '../constants/action_types'

const defaultState: PageState = {
  hasInitialized: false,
  showAddModal: false,
  showRecoveryPhrase: false,
  invalidMnemonic: false,
  importAccountError: undefined,
  importWalletError: { hasError: false },
  selectedTimeline: BraveWallet.AssetPriceTimeframe.OneDay,
  selectedAsset: undefined,
  isFetchingNFTMetadata: true,
  nftMetadata: undefined,
  nftMetadataError: undefined,
  selectedAssetFiatPrice: undefined,
  selectedAssetCryptoPrice: undefined,
  selectedAssetPriceHistory: [],
  portfolioPriceHistory: [],
  isFetchingPriceHistory: false,
  showIsRestoring: false,
  setupStillInProgress: false,
  isCryptoWalletsInitialized: false,
  isMetaMaskInitialized: false,
  isImportWalletsCheckComplete: false,
  importWalletAttempts: 0,
  walletTermsAcknowledged: false,
  selectedCoinMarket: undefined
}

export const createPageReducer = (initialState: PageState) => {
  const reducer = createReducer<PageState>({}, initialState)
  reducer.on(Actions.walletCreated.type, (state: PageState, payload: WalletCreatedPayloadType) => {
    return {
      ...state,
      mnemonic: payload.mnemonic,
      setupStillInProgress: true
    }
  })

  reducer.on(Actions.recoveryWordsAvailable.type, (state: PageState, payload: RecoveryWordsAvailablePayloadType) => {
    return {
      ...state,
      mnemonic: payload.mnemonic
    }
  })

  reducer.on(Actions.walletSetupComplete.type, (state: PageState, payload?: boolean): PageState => {
    // complete setup unless explicitly halted
    const setupStillInProgress = !payload

    return {
      ...state,
      mnemonic: undefined,
      setupStillInProgress
    }
  })

  reducer.on(Actions.walletBackupComplete.type, (state: PageState) => {
    const newState = {
      ...state,
      showRecoveryPhrase: false
    }
    delete newState.mnemonic
    return newState
  })

  reducer.on(Actions.showRecoveryPhrase.type, (state: PageState, payload: ShowRecoveryPhrasePayload) => {
    return {
      ...state,
      showRecoveryPhrase: payload.show
    }
  })

  reducer.on(Actions.hasMnemonicError.type, (state: PageState, payload: boolean) => {
    return {
      ...state,
      invalidMnemonic: payload
    }
  })

  reducer.on(Actions.updateSelectedAsset.type, (state: PageState, payload: BraveWallet.BlockchainToken) => {
    return {
      ...state,
      selectedAsset: payload
    }
  })

  reducer.on(Actions.updatePriceInfo.type, (state: PageState, payload: SelectAssetPayloadType) => {
    const history = payload.priceHistory ? payload.priceHistory.values : []
    return {
      ...state,
      selectedAssetFiatPrice: payload.defaultFiatPrice,
      selectedAssetCryptoPrice: payload.defaultCryptoPrice,
      selectedAssetPriceHistory: history,
      selectedTimeline: payload.timeFrame,
      isFetchingPriceHistory: false
    }
  })

  reducer.on(Actions.setIsFetchingPriceHistory.type, (state: PageState, payload: boolean) => {
    return {
      ...state,
      isFetchingPriceHistory: payload
    }
  })

  reducer.on(Actions.setShowIsRestoring.type, (state: PageState, payload: boolean) => {
    return {
      ...state,
      showIsRestoring: payload
    }
  })

  reducer.on(Actions.setImportAccountError.type, (state: PageState, payload: ImportAccountErrorType) => {
    return {
      ...state,
      importAccountError: payload
    }
  })

  reducer.on(Actions.setImportWalletError.type, (state: PageState, {
    hasError,
    errorMessage,
    incrementAttempts
  }: ImportWalletErrorPayloadType) => {
    return {
      ...state,
      importWalletError: { hasError, errorMessage },
      importWalletAttempts: incrementAttempts ? state.importWalletAttempts + 1 : state.importWalletAttempts
    }
  })

  reducer.on(Actions.setShowAddModal.type, (state: PageState, payload: boolean) => {
    return {
      ...state,
      showAddModal: payload
    }
  })

  reducer.on(Actions.setCryptoWalletsInitialized.type, (state: PageState, payload: boolean): PageState => {
    return {
      ...state,
      isCryptoWalletsInitialized: payload
    }
  })

  reducer.on(Actions.setMetaMaskInitialized.type, (state: PageState, payload: boolean) => {
    return {
      ...state,
      isMetaMaskInitialized: payload
    }
  })

  reducer.on(Actions.updateNFTMetadata.type, (state: PageState, payload: NFTMetadataReturnType) => {
    return {
      ...state,
      nftMetadata: payload
    }
  })

  reducer.on(Actions.setIsFetchingNFTMetadata.type, (state: PageState, payload: boolean) => {
    return {
      ...state,
      isFetchingNFTMetadata: payload
    }
  })

  reducer.on(Actions.updateNftMetadataError.type, (state: PageState, payload: string | undefined) => {
    return {
      ...state,
      nftMetadataError: payload
    }
  })

  reducer.on(Actions.setImportWalletsCheckComplete.type, (state: PageState, payload: boolean): PageState => {
    return {
      ...state,
      isImportWalletsCheckComplete: payload
    }
  })

  reducer.on(Actions.agreeToWalletTerms.type, (state: PageState): PageState => {
    return {
      ...state,
      walletTermsAcknowledged: true
    }
  })

  reducer.on(Actions.selectCoinMarket.type, (state: PageState, payload: BraveWallet.CoinMarket) => {
    return {
      ...state,
      selectedCoinMarket: payload
    }
  })
  return reducer
}

const reducer = createPageReducer(defaultState)
export const pageReducer = reducer
export default reducer
