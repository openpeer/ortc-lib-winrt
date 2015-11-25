﻿#pragma once

#include <ortc/types.h>
#include <ortc/IICEGatherer.h>
#include <collection.h>

using namespace ortc;

using Windows::Foundation::Collections::IVector;

namespace ortc_winrt_api
{

  ZS_DECLARE_CLASS_PTR(RTCIceGathererDelegate)

  ref class RTCIceGatherer;
  ref class RTCIceCandidate;

  class RTCIceGathererDelegate : public IICEGathererDelegate
  {
  public:
     virtual void onICEGathererStateChanged(
      IICEGathererPtr gatherer,
      IICEGatherer::States state
      );

    virtual void onICEGathererLocalCandidate(
      IICEGathererPtr gatherer,
      CandidatePtr candidate
      );

    virtual void onICEGathererLocalCandidateComplete(
      IICEGathererPtr gatherer,
      CandidateCompletePtr candidate
      );

    virtual void onICEGathererLocalCandidateGone(
      IICEGathererPtr gatherer,
      CandidatePtr candidate
      );

    virtual void onICEGathererError(
      IICEGathererPtr gatherer,
      ErrorCode errorCode,
      zsLib::String errorReason
      );

    RTCIceGatherer^ _gatherer;

    void SetOwnerObject(RTCIceGatherer^ owner) { _gatherer = owner; }
  };

  public ref class RTCIceServer sealed
  {
    std::vector<Platform::String^> mURLs;
    Platform::String^     mUserName;
    Platform::String^     mCredential;
  };

  public enum class RTCIceGatherPolicy
  {
    IceGatherPolicy_All,
    IceGatherPolicy_Nohost,
    IceGatherPolicy_Relay
  };

  public enum class RTCIceGathererState {
    State_New,
    State_Gathering,
    State_Complete,
    State_Closed,
  };

  public ref class RTCIceGatherOptions sealed
  {
    RTCIceGatherPolicy     gatherPolicy;
    std::vector<RTCIceServer^> iceServers;
  };

  public enum class RTCIceProtocol
  {
    Protocol_UDP,
    Protocol_TCP
  };

  public enum class RTCIceCandidateType {
    CandidateType_Host,
    CandidateType_Srflex,
    CandidateType_Prflx,
    CandidateType_Relay,
  };

  public enum class RTCIceTcpCandidateType {
    TCPCandidateType_Active,
    TCPCandidateType_Passive,
    TCPCandidateType_SO,
  };


  public ref class RTCIceCandidate sealed
  {
  public:
    property Platform::String^            InterfaceType;
    property Platform::String^            Foundation;
    property uint32                       Priority;
    property uint32                       UnfreezePriority;
    property RTCIceProtocol               Protocol;
    property Platform::String^            IP;
    property uint16                       Port;
    property RTCIceCandidateType          CandidateType;
    property RTCIceTcpCandidateType       TCPType;
    property Platform::String^            RelatedAddress;
    property uint16                       RelatedPort;
  };

  public ref class RTCIceCandidateComplete sealed
  {
  public:
    property bool Complete;
  };

  public ref class RTCIceGathererError sealed
  {
  public:
    property uint16 ErrorCode;
    property Platform::String^ ErrorReason;
  };

  public ref class RTCIceParameters sealed
  {
  public:
    property Platform::String^ UsernameFragment;
    property Platform::String^ Password;
  };

  //------------------------------------------
  // Events and Delegates
  //------------------------------------------
  
  // State change event and delegate
  public ref class RTCIceGathererStateChangeEvent sealed {
  public:
    property RTCIceGathererState State
    {
      RTCIceGathererState  get(){ return m_state; }
      void  set(RTCIceGathererState value){ m_state = value; }
    }

  private:
    RTCIceGathererState m_state;
  };

  public delegate void RTCIceGathererStateChangedDelegate(RTCIceGathererStateChangeEvent^ evt);


  // Candidate event and delegate
  public ref class RTCIceGathererCandidateEvent sealed {
  public:
    property RTCIceCandidate^ Candidate
    {
      RTCIceCandidate^  get(){ return m_candidate; }
      void  set(RTCIceCandidate^ value){ m_candidate = value; }
    }

  private:
    RTCIceCandidate^ m_candidate;
  };

  public delegate void RTCIceGathererLocalCandidateDelegate(RTCIceGathererCandidateEvent^ evt);
  public delegate void RTCIceGathererLocalCandidateGoneDelegate(RTCIceGathererCandidateEvent^ evt);

  // Candidate complete event and delegate

  public ref class RTCIceGathererCandidateCompleteEvent sealed {
  public:
    property RTCIceCandidateComplete^ Complete;
  };

  public delegate void RTCIceGathererCandidateCompleteDelegate(RTCIceGathererCandidateCompleteEvent^ evt);

  // Error event and delegate
  public ref class RTCIceGathererErrorEvent sealed {
  public:
    property RTCIceGathererError^ Error
    {
      RTCIceGathererError^  get(){ return m_error; }
      void  set(RTCIceGathererError^ value){ m_error = value; }
    }

  private:
    RTCIceGathererError^ m_error;
  };

  public delegate void RTCIceGathererErrorDelegate(RTCIceGathererErrorEvent^ evt);

  //------------------------------------------
  // End Events and Delegates
  //------------------------------------------

	public ref class RTCIceGatherer sealed
	{
    friend class RTCIceGathererDelegate;
    friend class FetchNativePointer;
	public:
    RTCIceGatherer();
    RTCIceGatherer(RTCIceGatherOptions^ options);

    RTCIceParameters^ getLocalParameters();
    IVector<RTCIceCandidate^>^ getLocalCandidates();
    RTCIceGatherer^ createAssociatedGatherer();

    void close();

  private:
    IICEGathererPtr mNativePointer;
    RTCIceGathererDelegatePtr mNativeDelegatePointer;

  public:

    event RTCIceGathererStateChangedDelegate^       OnICEGathererStateChanged;
    event RTCIceGathererLocalCandidateDelegate^     OnICEGathererLocalCandidate;
    event RTCIceGathererCandidateCompleteDelegate^  OnICEGathererCandidateComplete;
    event RTCIceGathererLocalCandidateGoneDelegate^ OnICEGathererLocalCandidateGone;
    event RTCIceGathererErrorDelegate^              OnICEGathererError;
	};
}
