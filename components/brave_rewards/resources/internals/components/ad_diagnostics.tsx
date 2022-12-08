/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

import * as React from 'react'

import { Button } from 'brave-ui/components'
import { ButtonWrapper, DiagnosticsEntry, Input, DiagnosticsHeader } from '../style'

// Utils
import { getLocale } from '../../../../common/locale'

interface Props {
  adDiagnostics: RewardsInternals.AdDiagnostics
  onGet: () => void
  setAdDiagnosticId: (adDiagnosticId: string) => void
}

const getEntries = (entries: RewardsInternals.AdDiagnosticsEntry[]) => {
  return entries.map(entry => (
    <DiagnosticsEntry key={entry.name}>
      {entry.name}: {entry.value}
    </DiagnosticsEntry>
  ))
}

const adDiagnosticsInfo = (adDiagnostics: RewardsInternals.AdDiagnostics) => {
  const { entries } = adDiagnostics

  return (
    <>
      <DiagnosticsHeader>
        {getLocale('adDiagnosticsInfo')}
      </DiagnosticsHeader>
      {
        (!entries || entries.length === 0)
          ? getLocale('adsNotInitialized')
          : getEntries(entries)
      }
    </>
  )
}

const adDiagnosticId = (props: Props) => {
  const handleDiagnosticIdChanged =
    React.useCallback((event: React.ChangeEvent<HTMLInputElement>) => {
      props.setAdDiagnosticId(event.target.value)
    }, [])

  return (
    <>
      <DiagnosticsHeader>
        {getLocale('adDiagnosticId')}
      </DiagnosticsHeader>
      <DiagnosticsEntry>
        <Input
          placeholder={getLocale('adDiagnosticId')}
          defaultValue={props.adDiagnostics.diagnosticId}
          onChange={handleDiagnosticIdChanged}
          maxLength={256}
        />
      </DiagnosticsEntry>
    </>
  )
}

export const AdDiagnostics = (props: Props) => {
  return (
    <>
      <ButtonWrapper>
        <Button
          text={getLocale('refreshButton')}
          size={'medium'}
          type={'accent'}
          onClick={props.onGet}
        />
      </ButtonWrapper>

      {adDiagnosticsInfo(props.adDiagnostics)}

      {adDiagnosticId(props)}
    </>
  )
}
