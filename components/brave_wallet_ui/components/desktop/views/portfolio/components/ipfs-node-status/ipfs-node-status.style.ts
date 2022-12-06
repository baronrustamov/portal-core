// Copyright (c) 2022 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.

import styled from 'styled-components'

import PositiveStatus from '../../../../../../assets/svg-icons/nft-ipfs/positive-status.svg'

export const IpfsNodeStatusWrapper = styled.div`
  display: flex;
  flex-direction: row;
  gap: 8px;
  align-items: center;
  justify-content: center;
`

export const StatusIcon = styled.div`
  width: 8px;
  height: 8px;
  mask-image: url(${PositiveStatus});
  -webkit-mask-image: url(${PositiveStatus});
  background-color: #51CF66;
`

export const Text = styled.p`
  font-family: 'Poppins';
  font-style: normal;
  font-weight: 400;
  font-size: 12px;
  line-height: 14px;
  color: ${p => p.theme.color.text03};
  margin: 0;
  padding: 0;
`
