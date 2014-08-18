/* -*- Mode: IDL; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

[Pref="dom.telephony.enabled"]
interface MMIRequest : DOMRequest {
  /**
   * If successful, the request's onsuccess will be called. And the request's
   * result will be an object containing information about the operation.
   * @see MozMMIResult for the detail of result.
   *
   * Otherwise, the request's onerror will be called, and the request's error
   * will be a DOMMMIError.
   * @see DOMMMIError for the detail of error.
   */
};
