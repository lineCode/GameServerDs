// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: gate_login_msg.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_gate_5flogin_5fmsg_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_gate_5flogin_5fmsg_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3014000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3014000 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata_lite.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>
#include "CsCmd.pb.h"
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_gate_5flogin_5fmsg_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_gate_5flogin_5fmsg_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxiliaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[2]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const ::PROTOBUF_NAMESPACE_ID::uint32 offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_gate_5flogin_5fmsg_2eproto;
namespace pb {
class SSGate2LLoginAuthReq;
class SSGate2LLoginAuthReqDefaultTypeInternal;
extern SSGate2LLoginAuthReqDefaultTypeInternal _SSGate2LLoginAuthReq_default_instance_;
class SSLogin2GLoginAuthRsp;
class SSLogin2GLoginAuthRspDefaultTypeInternal;
extern SSLogin2GLoginAuthRspDefaultTypeInternal _SSLogin2GLoginAuthRsp_default_instance_;
}  // namespace pb
PROTOBUF_NAMESPACE_OPEN
template<> ::pb::SSGate2LLoginAuthReq* Arena::CreateMaybeMessage<::pb::SSGate2LLoginAuthReq>(Arena*);
template<> ::pb::SSLogin2GLoginAuthRsp* Arena::CreateMaybeMessage<::pb::SSLogin2GLoginAuthRsp>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace pb {

enum SS_GATE_LOGIN_CMD : int {
  SS_GATE_LOGIN_DEFUALT = 0,
  SS_GATE_LOGIN_LOGINAUTH_REQ = 30001,
  SS_LOGIN_GATE_LOGINAUTH_RSP = 30002,
  SS_GATE_LOGIN_CMD_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::min(),
  SS_GATE_LOGIN_CMD_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::max()
};
bool SS_GATE_LOGIN_CMD_IsValid(int value);
constexpr SS_GATE_LOGIN_CMD SS_GATE_LOGIN_CMD_MIN = SS_GATE_LOGIN_DEFUALT;
constexpr SS_GATE_LOGIN_CMD SS_GATE_LOGIN_CMD_MAX = SS_LOGIN_GATE_LOGINAUTH_RSP;
constexpr int SS_GATE_LOGIN_CMD_ARRAYSIZE = SS_GATE_LOGIN_CMD_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* SS_GATE_LOGIN_CMD_descriptor();
template<typename T>
inline const std::string& SS_GATE_LOGIN_CMD_Name(T enum_t_value) {
  static_assert(::std::is_same<T, SS_GATE_LOGIN_CMD>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function SS_GATE_LOGIN_CMD_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    SS_GATE_LOGIN_CMD_descriptor(), enum_t_value);
}
inline bool SS_GATE_LOGIN_CMD_Parse(
    ::PROTOBUF_NAMESPACE_ID::ConstStringParam name, SS_GATE_LOGIN_CMD* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<SS_GATE_LOGIN_CMD>(
    SS_GATE_LOGIN_CMD_descriptor(), name, value);
}
// ===================================================================

class SSGate2LLoginAuthReq PROTOBUF_FINAL :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:pb.SSGate2LLoginAuthReq) */ {
 public:
  inline SSGate2LLoginAuthReq() : SSGate2LLoginAuthReq(nullptr) {}
  virtual ~SSGate2LLoginAuthReq();

  SSGate2LLoginAuthReq(const SSGate2LLoginAuthReq& from);
  SSGate2LLoginAuthReq(SSGate2LLoginAuthReq&& from) noexcept
    : SSGate2LLoginAuthReq() {
    *this = ::std::move(from);
  }

  inline SSGate2LLoginAuthReq& operator=(const SSGate2LLoginAuthReq& from) {
    CopyFrom(from);
    return *this;
  }
  inline SSGate2LLoginAuthReq& operator=(SSGate2LLoginAuthReq&& from) noexcept {
    if (GetArena() == from.GetArena()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return GetMetadataStatic().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return GetMetadataStatic().reflection;
  }
  static const SSGate2LLoginAuthReq& default_instance();

  static inline const SSGate2LLoginAuthReq* internal_default_instance() {
    return reinterpret_cast<const SSGate2LLoginAuthReq*>(
               &_SSGate2LLoginAuthReq_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(SSGate2LLoginAuthReq& a, SSGate2LLoginAuthReq& b) {
    a.Swap(&b);
  }
  inline void Swap(SSGate2LLoginAuthReq* other) {
    if (other == this) return;
    if (GetArena() == other->GetArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(SSGate2LLoginAuthReq* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline SSGate2LLoginAuthReq* New() const final {
    return CreateMaybeMessage<SSGate2LLoginAuthReq>(nullptr);
  }

  SSGate2LLoginAuthReq* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<SSGate2LLoginAuthReq>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const SSGate2LLoginAuthReq& from);
  void MergeFrom(const SSGate2LLoginAuthReq& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* _InternalSerialize(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(SSGate2LLoginAuthReq* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "pb.SSGate2LLoginAuthReq";
  }
  protected:
  explicit SSGate2LLoginAuthReq(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_gate_5flogin_5fmsg_2eproto);
    return ::descriptor_table_gate_5flogin_5fmsg_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kReqFieldNumber = 2,
    kRequestIdFieldNumber = 1,
  };
  // .pb.LoginAuthCsReq req = 2;
  bool has_req() const;
  private:
  bool _internal_has_req() const;
  public:
  void clear_req();
  const ::pb::LoginAuthCsReq& req() const;
  ::pb::LoginAuthCsReq* release_req();
  ::pb::LoginAuthCsReq* mutable_req();
  void set_allocated_req(::pb::LoginAuthCsReq* req);
  private:
  const ::pb::LoginAuthCsReq& _internal_req() const;
  ::pb::LoginAuthCsReq* _internal_mutable_req();
  public:
  void unsafe_arena_set_allocated_req(
      ::pb::LoginAuthCsReq* req);
  ::pb::LoginAuthCsReq* unsafe_arena_release_req();

  // uint32 request_id = 1;
  void clear_request_id();
  ::PROTOBUF_NAMESPACE_ID::uint32 request_id() const;
  void set_request_id(::PROTOBUF_NAMESPACE_ID::uint32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint32 _internal_request_id() const;
  void _internal_set_request_id(::PROTOBUF_NAMESPACE_ID::uint32 value);
  public:

  // @@protoc_insertion_point(class_scope:pb.SSGate2LLoginAuthReq)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::pb::LoginAuthCsReq* req_;
  ::PROTOBUF_NAMESPACE_ID::uint32 request_id_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_gate_5flogin_5fmsg_2eproto;
};
// -------------------------------------------------------------------

class SSLogin2GLoginAuthRsp PROTOBUF_FINAL :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:pb.SSLogin2GLoginAuthRsp) */ {
 public:
  inline SSLogin2GLoginAuthRsp() : SSLogin2GLoginAuthRsp(nullptr) {}
  virtual ~SSLogin2GLoginAuthRsp();

  SSLogin2GLoginAuthRsp(const SSLogin2GLoginAuthRsp& from);
  SSLogin2GLoginAuthRsp(SSLogin2GLoginAuthRsp&& from) noexcept
    : SSLogin2GLoginAuthRsp() {
    *this = ::std::move(from);
  }

  inline SSLogin2GLoginAuthRsp& operator=(const SSLogin2GLoginAuthRsp& from) {
    CopyFrom(from);
    return *this;
  }
  inline SSLogin2GLoginAuthRsp& operator=(SSLogin2GLoginAuthRsp&& from) noexcept {
    if (GetArena() == from.GetArena()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return GetMetadataStatic().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return GetMetadataStatic().reflection;
  }
  static const SSLogin2GLoginAuthRsp& default_instance();

  static inline const SSLogin2GLoginAuthRsp* internal_default_instance() {
    return reinterpret_cast<const SSLogin2GLoginAuthRsp*>(
               &_SSLogin2GLoginAuthRsp_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  friend void swap(SSLogin2GLoginAuthRsp& a, SSLogin2GLoginAuthRsp& b) {
    a.Swap(&b);
  }
  inline void Swap(SSLogin2GLoginAuthRsp* other) {
    if (other == this) return;
    if (GetArena() == other->GetArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(SSLogin2GLoginAuthRsp* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline SSLogin2GLoginAuthRsp* New() const final {
    return CreateMaybeMessage<SSLogin2GLoginAuthRsp>(nullptr);
  }

  SSLogin2GLoginAuthRsp* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<SSLogin2GLoginAuthRsp>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const SSLogin2GLoginAuthRsp& from);
  void MergeFrom(const SSLogin2GLoginAuthRsp& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* _InternalSerialize(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(SSLogin2GLoginAuthRsp* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "pb.SSLogin2GLoginAuthRsp";
  }
  protected:
  explicit SSLogin2GLoginAuthRsp(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_gate_5flogin_5fmsg_2eproto);
    return ::descriptor_table_gate_5flogin_5fmsg_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kRspFieldNumber = 2,
    kRequestIdFieldNumber = 1,
  };
  // .pb.LoginAuthScRsp rsp = 2;
  bool has_rsp() const;
  private:
  bool _internal_has_rsp() const;
  public:
  void clear_rsp();
  const ::pb::LoginAuthScRsp& rsp() const;
  ::pb::LoginAuthScRsp* release_rsp();
  ::pb::LoginAuthScRsp* mutable_rsp();
  void set_allocated_rsp(::pb::LoginAuthScRsp* rsp);
  private:
  const ::pb::LoginAuthScRsp& _internal_rsp() const;
  ::pb::LoginAuthScRsp* _internal_mutable_rsp();
  public:
  void unsafe_arena_set_allocated_rsp(
      ::pb::LoginAuthScRsp* rsp);
  ::pb::LoginAuthScRsp* unsafe_arena_release_rsp();

  // uint32 request_id = 1;
  void clear_request_id();
  ::PROTOBUF_NAMESPACE_ID::uint32 request_id() const;
  void set_request_id(::PROTOBUF_NAMESPACE_ID::uint32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint32 _internal_request_id() const;
  void _internal_set_request_id(::PROTOBUF_NAMESPACE_ID::uint32 value);
  public:

  // @@protoc_insertion_point(class_scope:pb.SSLogin2GLoginAuthRsp)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::pb::LoginAuthScRsp* rsp_;
  ::PROTOBUF_NAMESPACE_ID::uint32 request_id_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_gate_5flogin_5fmsg_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// SSGate2LLoginAuthReq

// uint32 request_id = 1;
inline void SSGate2LLoginAuthReq::clear_request_id() {
  request_id_ = 0u;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 SSGate2LLoginAuthReq::_internal_request_id() const {
  return request_id_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 SSGate2LLoginAuthReq::request_id() const {
  // @@protoc_insertion_point(field_get:pb.SSGate2LLoginAuthReq.request_id)
  return _internal_request_id();
}
inline void SSGate2LLoginAuthReq::_internal_set_request_id(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  
  request_id_ = value;
}
inline void SSGate2LLoginAuthReq::set_request_id(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  _internal_set_request_id(value);
  // @@protoc_insertion_point(field_set:pb.SSGate2LLoginAuthReq.request_id)
}

// .pb.LoginAuthCsReq req = 2;
inline bool SSGate2LLoginAuthReq::_internal_has_req() const {
  return this != internal_default_instance() && req_ != nullptr;
}
inline bool SSGate2LLoginAuthReq::has_req() const {
  return _internal_has_req();
}
inline const ::pb::LoginAuthCsReq& SSGate2LLoginAuthReq::_internal_req() const {
  const ::pb::LoginAuthCsReq* p = req_;
  return p != nullptr ? *p : reinterpret_cast<const ::pb::LoginAuthCsReq&>(
      ::pb::_LoginAuthCsReq_default_instance_);
}
inline const ::pb::LoginAuthCsReq& SSGate2LLoginAuthReq::req() const {
  // @@protoc_insertion_point(field_get:pb.SSGate2LLoginAuthReq.req)
  return _internal_req();
}
inline void SSGate2LLoginAuthReq::unsafe_arena_set_allocated_req(
    ::pb::LoginAuthCsReq* req) {
  if (GetArena() == nullptr) {
    delete reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(req_);
  }
  req_ = req;
  if (req) {
    
  } else {
    
  }
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:pb.SSGate2LLoginAuthReq.req)
}
inline ::pb::LoginAuthCsReq* SSGate2LLoginAuthReq::release_req() {
  
  ::pb::LoginAuthCsReq* temp = req_;
  req_ = nullptr;
  if (GetArena() != nullptr) {
    temp = ::PROTOBUF_NAMESPACE_ID::internal::DuplicateIfNonNull(temp);
  }
  return temp;
}
inline ::pb::LoginAuthCsReq* SSGate2LLoginAuthReq::unsafe_arena_release_req() {
  // @@protoc_insertion_point(field_release:pb.SSGate2LLoginAuthReq.req)
  
  ::pb::LoginAuthCsReq* temp = req_;
  req_ = nullptr;
  return temp;
}
inline ::pb::LoginAuthCsReq* SSGate2LLoginAuthReq::_internal_mutable_req() {
  
  if (req_ == nullptr) {
    auto* p = CreateMaybeMessage<::pb::LoginAuthCsReq>(GetArena());
    req_ = p;
  }
  return req_;
}
inline ::pb::LoginAuthCsReq* SSGate2LLoginAuthReq::mutable_req() {
  // @@protoc_insertion_point(field_mutable:pb.SSGate2LLoginAuthReq.req)
  return _internal_mutable_req();
}
inline void SSGate2LLoginAuthReq::set_allocated_req(::pb::LoginAuthCsReq* req) {
  ::PROTOBUF_NAMESPACE_ID::Arena* message_arena = GetArena();
  if (message_arena == nullptr) {
    delete reinterpret_cast< ::PROTOBUF_NAMESPACE_ID::MessageLite*>(req_);
  }
  if (req) {
    ::PROTOBUF_NAMESPACE_ID::Arena* submessage_arena =
      reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(req)->GetArena();
    if (message_arena != submessage_arena) {
      req = ::PROTOBUF_NAMESPACE_ID::internal::GetOwnedMessage(
          message_arena, req, submessage_arena);
    }
    
  } else {
    
  }
  req_ = req;
  // @@protoc_insertion_point(field_set_allocated:pb.SSGate2LLoginAuthReq.req)
}

// -------------------------------------------------------------------

// SSLogin2GLoginAuthRsp

// uint32 request_id = 1;
inline void SSLogin2GLoginAuthRsp::clear_request_id() {
  request_id_ = 0u;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 SSLogin2GLoginAuthRsp::_internal_request_id() const {
  return request_id_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 SSLogin2GLoginAuthRsp::request_id() const {
  // @@protoc_insertion_point(field_get:pb.SSLogin2GLoginAuthRsp.request_id)
  return _internal_request_id();
}
inline void SSLogin2GLoginAuthRsp::_internal_set_request_id(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  
  request_id_ = value;
}
inline void SSLogin2GLoginAuthRsp::set_request_id(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  _internal_set_request_id(value);
  // @@protoc_insertion_point(field_set:pb.SSLogin2GLoginAuthRsp.request_id)
}

// .pb.LoginAuthScRsp rsp = 2;
inline bool SSLogin2GLoginAuthRsp::_internal_has_rsp() const {
  return this != internal_default_instance() && rsp_ != nullptr;
}
inline bool SSLogin2GLoginAuthRsp::has_rsp() const {
  return _internal_has_rsp();
}
inline const ::pb::LoginAuthScRsp& SSLogin2GLoginAuthRsp::_internal_rsp() const {
  const ::pb::LoginAuthScRsp* p = rsp_;
  return p != nullptr ? *p : reinterpret_cast<const ::pb::LoginAuthScRsp&>(
      ::pb::_LoginAuthScRsp_default_instance_);
}
inline const ::pb::LoginAuthScRsp& SSLogin2GLoginAuthRsp::rsp() const {
  // @@protoc_insertion_point(field_get:pb.SSLogin2GLoginAuthRsp.rsp)
  return _internal_rsp();
}
inline void SSLogin2GLoginAuthRsp::unsafe_arena_set_allocated_rsp(
    ::pb::LoginAuthScRsp* rsp) {
  if (GetArena() == nullptr) {
    delete reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(rsp_);
  }
  rsp_ = rsp;
  if (rsp) {
    
  } else {
    
  }
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:pb.SSLogin2GLoginAuthRsp.rsp)
}
inline ::pb::LoginAuthScRsp* SSLogin2GLoginAuthRsp::release_rsp() {
  
  ::pb::LoginAuthScRsp* temp = rsp_;
  rsp_ = nullptr;
  if (GetArena() != nullptr) {
    temp = ::PROTOBUF_NAMESPACE_ID::internal::DuplicateIfNonNull(temp);
  }
  return temp;
}
inline ::pb::LoginAuthScRsp* SSLogin2GLoginAuthRsp::unsafe_arena_release_rsp() {
  // @@protoc_insertion_point(field_release:pb.SSLogin2GLoginAuthRsp.rsp)
  
  ::pb::LoginAuthScRsp* temp = rsp_;
  rsp_ = nullptr;
  return temp;
}
inline ::pb::LoginAuthScRsp* SSLogin2GLoginAuthRsp::_internal_mutable_rsp() {
  
  if (rsp_ == nullptr) {
    auto* p = CreateMaybeMessage<::pb::LoginAuthScRsp>(GetArena());
    rsp_ = p;
  }
  return rsp_;
}
inline ::pb::LoginAuthScRsp* SSLogin2GLoginAuthRsp::mutable_rsp() {
  // @@protoc_insertion_point(field_mutable:pb.SSLogin2GLoginAuthRsp.rsp)
  return _internal_mutable_rsp();
}
inline void SSLogin2GLoginAuthRsp::set_allocated_rsp(::pb::LoginAuthScRsp* rsp) {
  ::PROTOBUF_NAMESPACE_ID::Arena* message_arena = GetArena();
  if (message_arena == nullptr) {
    delete reinterpret_cast< ::PROTOBUF_NAMESPACE_ID::MessageLite*>(rsp_);
  }
  if (rsp) {
    ::PROTOBUF_NAMESPACE_ID::Arena* submessage_arena =
      reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(rsp)->GetArena();
    if (message_arena != submessage_arena) {
      rsp = ::PROTOBUF_NAMESPACE_ID::internal::GetOwnedMessage(
          message_arena, rsp, submessage_arena);
    }
    
  } else {
    
  }
  rsp_ = rsp;
  // @@protoc_insertion_point(field_set_allocated:pb.SSLogin2GLoginAuthRsp.rsp)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace pb

PROTOBUF_NAMESPACE_OPEN

template <> struct is_proto_enum< ::pb::SS_GATE_LOGIN_CMD> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::pb::SS_GATE_LOGIN_CMD>() {
  return ::pb::SS_GATE_LOGIN_CMD_descriptor();
}

PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_gate_5flogin_5fmsg_2eproto