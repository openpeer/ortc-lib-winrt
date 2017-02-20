// Generated by zsLibEventingTool


#include "cx_custom_Org_Ortc_MediaSource.h"
#include "cx_custom_WebRtcMediaSource.h"

#include <wrapper/generated/cx/cx_Helpers.h>

//------------------------------------------------------------------------------
::Org::Ortc::MediaSource^ Org::Ortc::MediaSource::ToCx(wrapper::org::ortc::MediaSourcePtr value)
{
  if (!value) return nullptr;
  auto result = ref new MediaSource(WrapperCreate{});
  result->native_ = value;
  return result;
}

//------------------------------------------------------------------------------
wrapper::org::ortc::MediaSourcePtr Org::Ortc::MediaSource::FromCx(::Org::Ortc::MediaSource^ value)
{
  if (nullptr == value) return wrapper::org::ortc::MediaSourcePtr();
  return value->native_;
}

//------------------------------------------------------------------------------
Windows::Media::Core::IMediaSource^ Org::Ortc::MediaSource::Source::get()
{
  if (!native_) {throw ref new Platform::NullReferenceException();}

  zsLib::AutoLock lock(lock_);

  auto any = native_->get_source();
  auto data = std::dynamic_pointer_cast<Data>(any);
  if (!data) {
    data = make_shared<Data>();
    Platform::String^ id = "stream";
    ComPtr<ABI::Windows::Media::Core::IMediaSource> comSource;

    WebRtcMediaSource::CreateMediaSource(&comSource, native_->get_track(), id);
    data->source_ = reinterpret_cast<Windows::Media::Core::IMediaSource^>(comSource.Get());
    native_->set_source(data);
  }
  return data->source_;
}
