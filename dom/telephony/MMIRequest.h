/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_mmirequest_h__
#define mozilla_dom_mmirequest_h__

#include "mozilla/dom/DOMRequest.h"
#include "nsCycleCollectionParticipant.h"

namespace mozilla {
namespace dom {

class MMIRequest : public DOMRequest
{
public:
  NS_DECL_ISUPPORTS_INHERITED
  //NS_DECL_CYCLE_COLLECTION_CLASS_INHERITED(MMIRequest, DOMRequest)

  MMIRequest(nsPIDOMWindow* aOwner);

  virtual JSObject* WrapObject(JSContext* aCx) MOZ_OVERRIDE;

private:
  ~MMIRequest();

};

} // namespace dom
} // namespace mozilla

#endif // mozilla_dom_mmirequest_h__
