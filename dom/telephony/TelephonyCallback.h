/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_TelephonyCallback_h
#define mozilla_dom_TelephonyCallback_h

#include "mozilla/dom/ToJSValue.h"
#include "nsCOMPtr.h"
#include "nsITelephonyService.h"
#include "nsJSUtils.h"
#include "nsString.h"

class nsPIDOMWindow;

namespace mozilla {
namespace dom {

class MMIRequest;
class Promise;
class Telephony;

namespace telephony {

class TelephonyCallback MOZ_FINAL : public nsITelephonyCallback
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSITELEPHONYCALLBACK

  TelephonyCallback(nsPIDOMWindow* aWindow, Telephony* aTelephony,
                    Promise* aPromise, uint32_t aServiceId);

  nsresult
  NotifyDialMMISuccess(const nsAString& aStatusMessage);

  template<typename T>
  nsresult
  NotifyDialMMISuccess(const nsAString& aStatusMessage, const T& aInfo)
  {
    AutoJSAPI jsapi;
    if (!NS_WARN_IF(jsapi.Init(mWindow))) {
      return NS_ERROR_FAILURE;
    }

    JSContext* cx = jsapi.cx();
    JS::Rooted<JS::Value> info(cx);

    if (!ToJSValue(cx, aInfo, &info)) {
      JS_ClearPendingException(cx);
      return NS_ERROR_TYPE_ERR;
    }

    return NotifyDialMMISuccess(aStatusMessage, info);
  }

  nsresult
  NotifyDialMMISuccess(const nsAString& aStatusMessage,
                       JS::Handle<JS::Value> aInfo);

  nsresult
  NotifyDialMMISuccess(const MozMMIResult& aResult);

private:
  ~TelephonyCallback() {}

  nsCOMPtr<nsPIDOMWindow> mWindow;
  nsRefPtr<Telephony> mTelephony;
  nsRefPtr<Promise> mPromise;
  uint32_t mServiceId;

  nsRefPtr<MMIRequest> mMMIRequest;
  nsString mServiceCode;
};

} // namespace dom
} // namespace mozilla
} // namespace telephony

#endif // mozilla_dom_TelephonyCallback_h
