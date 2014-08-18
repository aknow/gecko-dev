/* -*- Mode: c++; c-basic-offset: 2; indent-tabs-mode: nil; tab-width: 40 -*- */
/* vim: set ts=2 et sw=2 tw=40: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "MMIRequest.h"

#include "mozilla/dom/DOMError.h"
#include "mozilla/dom/MMIRequestBinding.h"

using namespace mozilla::dom;
using mozilla::ErrorResult;

MMIRequest::MMIRequest(nsPIDOMWindow* aOwner)
  : DOMRequest(aOwner)
{}

MMIRequest::~MMIRequest()
{}

JSObject*
MMIRequest::WrapObject(JSContext* aCx)
{
  return MMIRequestBinding::Wrap(aCx, this);
}

//NS_IMPL_CYCLE_COLLECTION_INHERITED(MMIRequest, DOMRequest)

NS_INTERFACE_MAP_BEGIN_CYCLE_COLLECTION_INHERITED(MMIRequest)
NS_INTERFACE_MAP_END_INHERITING(DOMRequest)

NS_IMPL_ADDREF_INHERITED(MMIRequest, DOMRequest)
NS_IMPL_RELEASE_INHERITED(MMIRequest, DOMRequest)

// MMIRequest WebIDL
