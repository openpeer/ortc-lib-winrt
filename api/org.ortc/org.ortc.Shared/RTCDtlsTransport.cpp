#include "pch.h"

#include "RTCCertificate.h"
#include "RTCDtlsTransport.h"
#include "RTCIceTransport.h"
#include "helpers.h"
#include "Error.h"

#include <openpeer/services/IHelper.h>

#include <zsLib/SafeInt.h>

using Platform::Collections::Vector;
using Platform::Array;
using Platform::Object;

using namespace ortc;

namespace org
{
  namespace ortc
  {
    ZS_DECLARE_TYPEDEF_PTR(internal::Helper, UseHelper)

    namespace internal
    {

      RTCDtlsFingerprint^ ToCx(const ICertificateTypes::Fingerprint &input)
      {
        auto result = ref new RTCDtlsFingerprint();
        result->Algorithm = Helper::ToCx(input.mAlgorithm);
        result->Value = Helper::ToCx(input.mValue);
        return result;
      }

      RTCDtlsFingerprint^ ToCx(ICertificateTypes::FingerprintPtr input)
      {
        if (!input) return nullptr;
        return ToCx(*input);
      }

      ICertificateTypes::FingerprintPtr FromCx(RTCDtlsFingerprint^ input)
      {
        if (nullptr == input) return ICertificateTypes::FingerprintPtr();
        auto result = make_shared<ICertificateTypes::Fingerprint>();
        result->mAlgorithm = Helper::FromCx(input->Algorithm);
        result->mValue = Helper::FromCx(input->Value);
        return result;
      }

      RTCDtlsParameters^ ToCx(const IDTLSTransportTypes::Parameters &input)
      {
        auto result = ref new RTCDtlsParameters();
        result->Role = Helper::Convert(input.mRole);
        if (input.mFingerprints.size() > 0)
        {
          result->Fingerprints = ref new Vector<RTCDtlsFingerprint^>();
          for (auto iter = input.mFingerprints.begin(); iter != input.mFingerprints.end(); ++iter)
          {
            auto &value = (*iter);
            result->Fingerprints->Append(ToCx(value));
          }
        }
        return result;
      }

      RTCDtlsParameters^ ToCx(IDTLSTransportTypes::ParametersPtr input)
      {
        if (!input) return nullptr;
        return ToCx(*input);
      }

      IDTLSTransportTypes::ParametersPtr FromCx(RTCDtlsParameters^ input)
      {
        if (nullptr == input) return IDTLSTransportTypes::ParametersPtr();
        auto result = make_shared<IDTLSTransportTypes::Parameters>();
        result->mRole = Helper::Convert(input->Role);
        if (input->Fingerprints)
        {
          for (RTCDtlsFingerprint^ value : input->Fingerprints)
          {
            if (nullptr == value) continue;
            result->mFingerprints.push_front(*FromCx(value));
          }
        }
        return result;
      }

    } // namespace internal


    RTCDtlsTransport::RTCDtlsTransport() :
      _nativeDelegatePointer(nullptr),
      _nativePointer(nullptr)
    {
    }

    RTCDtlsTransport^ RTCDtlsTransport::Convert(IDTLSTransportPtr transport)
    {
      RTCDtlsTransport^ result = ref new RTCDtlsTransport();
      result->_nativePointer = transport;
      return result;
    }

    RTCDtlsTransport::RTCDtlsTransport(RTCIceTransport^ transport, IVector<RTCCertificate^>^ certificates) :
      _nativeDelegatePointer(new RTCDtlsTransportDelegate())
    {
      ORG_ORTC_THROW_INVALID_PARAMETERS_IF(nullptr == certificates)

      _nativeDelegatePointer->SetOwnerObject(this);

      auto nativeTransport = RTCIceTransport::Convert(transport);

      IDTLSTransport::CertificateList coreCertificates;
      for (RTCCertificate^ cert : certificates)
      {
        ICertificatePtr nativeCert = RTCCertificate::Convert(cert);
        ORG_ORTC_THROW_INVALID_PARAMETERS_IF(!nativeCert)
        coreCertificates.push_back(nativeCert);
      }

      try
      {
        _nativePointer = IDTLSTransport::create(_nativeDelegatePointer, nativeTransport, coreCertificates);
      }
      catch (const InvalidParameters & e)
      {
        ORG_ORTC_THROW_INVALID_PARAMETERS()
      }
      catch (const InvalidStateError &e)
      {
        ORG_ORTC_THROW_INVALID_STATE(UseHelper::ToCx(e.what()))
      }
    }

    RTCDtlsParameters^ RTCDtlsTransport::GetLocalParameters()
    {
      if (_nativePointer)
      {
        return internal::ToCx(_nativePointer->getLocalParameters());
      }
      return nullptr;
    }

    RTCDtlsParameters^ RTCDtlsTransport::GetRemoteParameters()
    {
      if (_nativePointer)
      {
        return internal::ToCx(_nativePointer->getRemoteParameters());
      }
      return nullptr;
    }

    IVector<Object^>^ RTCDtlsTransport::GetRemoteCertificates()
    {
      auto ret = ref new Vector<Object^>();

      if (_nativePointer)
      {
        IDTLSTransportTypes::SecureByteBlockListPtr certificates = _nativePointer->getRemoteCertificates();

        if (certificates->size() > 0)
        {
          for (IDTLSTransportTypes::SecureByteBlockList::iterator it = certificates->begin(); it != certificates->end(); ++it)
          {
            Array<byte>^ arr = ref new Array<byte>((*it).BytePtr(), SafeInt<unsigned int>((*it).SizeInBytes()));
            ret->Append(arr);
          }
        }
      }

      return ret;
    }

    void RTCDtlsTransport::Start(RTCDtlsParameters^ remoteParameters)
    {
      ORG_ORTC_THROW_INVALID_STATE_IF(!_nativePointer)
      ORG_ORTC_THROW_INVALID_PARAMETERS_IF(!remoteParameters)

      try
      {
        _nativePointer->start(*internal::FromCx(remoteParameters));
      }
      catch (const InvalidParameters & e)
      {
        ORG_ORTC_THROW_INVALID_PARAMETERS()
      }
      catch (const InvalidStateError &e)
      {
        ORG_ORTC_THROW_INVALID_STATE(UseHelper::ToCx(e.what()))
      }
    }

    void RTCDtlsTransport::Stop()
    {
      if (_nativePointer)
      {
        _nativePointer->stop();
      }
    }

    Platform::String^ RTCDtlsTransport::ToString()
    {
      throw ref new Platform::NotImplementedException();
    }

    Platform::String^ RTCDtlsTransport::ToString(RTCDtlsTransportState value)
    {
      return UseHelper::ToCx(IDTLSTransport::toString(UseHelper::Convert(value)));
    }

    Platform::String^ RTCDtlsTransport::ToString(RTCDtlsRole value)
    {
      return UseHelper::ToCx(IDTLSTransport::toString(UseHelper::Convert(value)));
    }

    RTCDtlsTransportState RTCDtlsTransport::ToState(Platform::String^ str)
    {
      return UseHelper::Convert(IDTLSTransport::toState(UseHelper::FromCx(str).c_str()));
    }

    RTCDtlsRole RTCDtlsTransport::ToRole(Platform::String^ str)
    {
      return UseHelper::Convert(IDTLSTransport::toRole(UseHelper::FromCx(str).c_str()));
    }

    RTCDtlsTransportState RTCDtlsTransport::State::get()
    {
      if (_nativePointer)
        return (RTCDtlsTransportState)_nativePointer->state();
      else
        return RTCDtlsTransportState::Closed;
    }

    IVector<RTCCertificate^>^ RTCDtlsTransport::Certificates::get()
    {
      if (!_nativePointer) return nullptr;

      auto ret = ref new Vector<RTCCertificate^>();

      IDTLSTransport::CertificateListPtr certificates = _nativePointer->certificates();
      for (IDTLSTransport::CertificateList::iterator it = certificates->begin(); it != certificates->end(); ++it)
      {
        ret->Append(RTCCertificate::Convert(*it));
      }
    
      return ret;
    }

    RTCIceTransport^ RTCDtlsTransport::IceTransport::get()
    {
      if (!_nativePointer) return nullptr;
      return RTCIceTransport::Convert(_nativePointer->transport());
    }

    void RTCDtlsTransportDelegate::onDTLSTransportStateChange(
      IDTLSTransportPtr transport,
      IDTLSTransport::States state
      )
    {
      auto evt = ref new RTCDtlsTransportStateChangeEvent();
      evt->State = UseHelper::Convert(state);
      _transport->OnDtlsTransportStateChanged(evt);
    }

    void RTCDtlsTransportDelegate::onDTLSTransportError(
      IDTLSTransportPtr transport,
      ErrorCode errorCode,
      zsLib::String errorReason
      )
    {
      auto evt = ref new RTCDtlsTransportErrorEvent();
      evt->Error->ErrorCode = errorCode;
      evt->Error->ErrorReason = UseHelper::ToCx(errorReason);
      _transport->OnDtlsTransportError(evt);
    }

    //---------------------------------------------------------------------------
    // RTCDtlsParameters methods
    //---------------------------------------------------------------------------
    Platform::String^ RTCDtlsParameters::ToJsonString()
    {
      auto params = internal::FromCx(this);
      return UseHelper::ToCx(openpeer::services::IHelper::toString(params->createElement("DtlsParameters")));
    }

    RTCDtlsParameters^ RTCDtlsParameters::FromJsonString(Platform::String^ jsonString)
    {
      auto params = make_shared<IDTLSTransport::Parameters>(IDTLSTransport::Parameters::Parameters(openpeer::services::IHelper::toJSON(UseHelper::FromCx(jsonString).c_str())));
      return internal::ToCx(params);
    }
  } // namespace ortc
} // namespace org
