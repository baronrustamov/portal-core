/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_OMNIBOX_OMNIBOX_VIEW_VIEWS_H_
#define BRAVE_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_OMNIBOX_OMNIBOX_VIEW_VIEWS_H_

#include "src/chrome/browser/ui/views/omnibox/omnibox_view_views.h"
#include "ui/views/controls/textfield/textfield_controller.h"

#define friendly_suggestion_text_ \
  friendly_suggestion_text_;      \
  friend class BraveOmniboxViewViews

#define OnAfterCutOrCopy                                           \
  OnAfterCutOrCopy(ui::ClipboardBuffer clipboard_buffer) override; \
  bool SelectedTextIsURL() override;                               \
  bool IsCleanLinkCommand(int command_id) const override;          \
  void OnSanitizedCopy
#include "src/chrome/browser/ui/views/omnibox/omnibox_view_views.h"
#undef OnAfterCutOrCopy
#undef friendly_suggestion_text_

#endif  // BRAVE_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_OMNIBOX_OMNIBOX_VIEW_VIEWS_H_
