/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef dom_mobileconnection_src_ipc_MobileConnectionIPCSerialiser_h
#define dom_mobileconnection_src_ipc_MobileConnectionIPCSerialiser_h

#include "ipc/IPCMessageUtils.h"
#include "MozMobileConnectionBinding.h"

namespace IPC {

struct MozCallForwardingOptions : public mozilla::dom::MozCallForwardingOptions
{
  bool operator==(const MozCallForwardingOptions& aOther) const
  {
    return // Compare mActive
           ((!mActive.WasPassed() && !aOther.mActive.WasPassed()) ||
            (mActive.WasPassed() && aOther.mActive.WasPassed() &&
             mActive.Value() == aOther.mActive.Value())) &&
           // Compare mAction
           ((!mAction.WasPassed() && !aOther.mAction.WasPassed()) ||
            (mAction.WasPassed() && aOther.mAction.WasPassed() &&
             mAction.Value() == aOther.mAction.Value())) &&
           // Compare mReason
           ((!mReason.WasPassed() && !aOther.mReason.WasPassed()) ||
            (mReason.WasPassed() && aOther.mReason.WasPassed() &&
             mReason.Value() == aOther.mReason.Value())) &&
           // Compare mNumber
           ((!mNumber.WasPassed() && !aOther.mNumber.WasPassed()) ||
            (mNumber.WasPassed() && aOther.mNumber.WasPassed() &&
             mNumber.Value() == aOther.mNumber.Value())) &&
           // Compare mTimeSeconds
           ((!mTimeSeconds.WasPassed() && !aOther.mTimeSeconds.WasPassed()) ||
            (mTimeSeconds.WasPassed() && aOther.mTimeSeconds.WasPassed() &&
             mTimeSeconds.Value() == aOther.mTimeSeconds.Value())) &&
           // Compare mServiceClass
           ((!mServiceClass.WasPassed() && !aOther.mServiceClass.WasPassed()) ||
            (mServiceClass.WasPassed() && aOther.mServiceClass.WasPassed() &&
             mServiceClass.Value() == aOther.mServiceClass.Value()));
  };
};

/**
 * MozCallForwardingOptions Serialize/De-serialize.
 */
template <>
struct ParamTraits<MozCallForwardingOptions>
{
  typedef MozCallForwardingOptions paramType;

  // Function to serialize a MozCallForwardingOptions.
  static void Write(Message *aMsg, const paramType& aParam)
  {
    bool wasPassed = false;
    bool isNull = false;

    // Write mActive
    wasPassed = aParam.mActive.WasPassed();
    WriteParam(aMsg, wasPassed);
    if (wasPassed) {
      isNull = aParam.mActive.Value().IsNull();
      WriteParam(aMsg, isNull);
      if (!isNull) {
        WriteParam(aMsg, aParam.mActive.Value().Value());
      }
    }

    // Write mAction
    wasPassed = aParam.mAction.WasPassed();
    WriteParam(aMsg, wasPassed);
    if (wasPassed) {
      isNull = aParam.mAction.Value().IsNull();
      WriteParam(aMsg, isNull);
      if (!isNull) {
        WriteParam(aMsg, aParam.mAction.Value().Value());
      }
    }

    // Write mReason
    wasPassed = aParam.mReason.WasPassed();
    WriteParam(aMsg, wasPassed);
    if (wasPassed) {
      isNull = aParam.mReason.Value().IsNull();
      WriteParam(aMsg, isNull);
      if (!isNull) {
        WriteParam(aMsg, aParam.mReason.Value().Value());
      }
    }

    // Write mNumber
    wasPassed = aParam.mNumber.WasPassed();
    WriteParam(aMsg, wasPassed);
    if (wasPassed) {
      WriteParam(aMsg, aParam.mNumber.Value());
    }

    // Write mTimeSeconds
    wasPassed = aParam.mTimeSeconds.WasPassed();
    WriteParam(aMsg, wasPassed);
    if (wasPassed) {
      isNull = aParam.mTimeSeconds.Value().IsNull();
      WriteParam(aMsg, isNull);
      if (!isNull) {
        WriteParam(aMsg, aParam.mTimeSeconds.Value().Value());
      }
    }

    // Write mServiceClass
    wasPassed = aParam.mServiceClass.WasPassed();
    WriteParam(aMsg, wasPassed);
    if (wasPassed) {
      isNull = aParam.mServiceClass.Value().IsNull();
      WriteParam(aMsg, isNull);
      if (!isNull) {
        WriteParam(aMsg, aParam.mServiceClass.Value().Value());
      }
    }
  }

  // Function to de-serialize a MozCallForwardingOptions.
  static bool Read(const Message *aMsg, void **aIter, paramType* aResult)
  {
    bool wasPassed = false;
    bool isNull = false;

    // Read mActive
    if (!ReadParam(aMsg, aIter, &wasPassed)) {
      return false;
    }
    if (wasPassed) {
      aResult->mActive.Construct();
      if (!ReadParam(aMsg, aIter, &isNull)) {
        return false;
      }

      if (!isNull) {
        if (!ReadParam(aMsg, aIter, &aResult->mActive.Value().SetValue())) {
          return false;
        }
      }
    }

    // Read mAction
    if (!ReadParam(aMsg, aIter, &wasPassed)) {
      return false;
    }
    if (wasPassed) {
      aResult->mAction.Construct();
      if (!ReadParam(aMsg, aIter, &isNull)) {
        return false;
      }

      if (!isNull) {
        if (!ReadParam(aMsg, aIter, &aResult->mAction.Value().SetValue())) {
          return false;
        }
      }
    }

    // Read mReason
    if (!ReadParam(aMsg, aIter, &wasPassed)) {
      return false;
    }
    if (wasPassed) {
      aResult->mReason.Construct();
      if (!ReadParam(aMsg, aIter, &isNull)) {
        return false;
      }

      if (!isNull) {
        if (!ReadParam(aMsg, aIter, &aResult->mReason.Value().SetValue())) {
          return false;
        }
      }
    }

    // Read mNumber
    if (!ReadParam(aMsg, aIter, &wasPassed)) {
      return false;
    }
    if (wasPassed) {
      if (!ReadParam(aMsg, aIter, &aResult->mNumber.Construct())) {
        return false;
      }
    }

    // Read mTimeSeconds
    if (!ReadParam(aMsg, aIter, &wasPassed)) {
      return false;
    }
    if (wasPassed) {
      aResult->mTimeSeconds.Construct();
      if (!ReadParam(aMsg, aIter, &isNull)) {
        return false;
      }

      if (!isNull) {
        if (!ReadParam(aMsg, aIter, &aResult->mTimeSeconds.Value().SetValue())) {
          return false;
        }
      }
    }

    // Read mServiceClass
    if (!ReadParam(aMsg, aIter, &wasPassed)) {
      return false;
    }
    if (wasPassed) {
      aResult->mServiceClass.Construct();
      if (!ReadParam(aMsg, aIter, &isNull)) {
        return false;
      }

      if (!isNull) {
        if (!ReadParam(aMsg, aIter, &aResult->mServiceClass.Value().SetValue())) {
          return false;
        }
      }
    }

    return true;
  }
};

} // namespace IPC

#endif // dom_mobileconnection_src_ipc_MobileConnectionIPCSerialiser_h
