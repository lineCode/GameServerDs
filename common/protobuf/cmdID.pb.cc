// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: cmdID.proto

#include "cmdID.pb.h"

#include <algorithm>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
namespace pb {
}  // namespace pb
static constexpr ::PROTOBUF_NAMESPACE_ID::Metadata* file_level_metadata_cmdID_2eproto = nullptr;
static const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* file_level_enum_descriptors_cmdID_2eproto[1];
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_cmdID_2eproto = nullptr;
const ::PROTOBUF_NAMESPACE_ID::uint32 TableStruct_cmdID_2eproto::offsets[1] = {};
static constexpr ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema* schemas = nullptr;
static constexpr ::PROTOBUF_NAMESPACE_ID::Message* const* file_default_instances = nullptr;

const char descriptor_table_protodef_cmdID_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\013cmdID.proto\022\002pb*\220\005\n\005CmdID\022\022\n\016ClientInp"
  "utMsg\020\000\022\024\n\020WorldSnapshotMsg\020\001\022\027\n\023ReadyIn"
  "RoomCsReqMsg\020\002\022\027\n\023TryLinkTeamScReqMsg\020\003\022"
  "\027\n\023TeamHeartBeatReqMsg\020\004\022\025\n\021CmdLoginCsAu"
  "thReq\020b\022\025\n\021CmdLoginScAuthRsp\020c\022\021\n\rCmdLog"
  "inCsReq\020d\022\021\n\rCmdLoginScRsp\020e\022\026\n\022CmdCreat"
  "eRoomCsReq\020f\022\026\n\022CmdCreateRoomScRsp\020g\022\025\n\021"
  "CmdEnterRoomCsReq\020h\022\025\n\021CmdEnterRoomScRsp"
  "\020i\022\025\n\021CmdLeaveRoomCsReq\020j\022\025\n\021CmdLeaveRoo"
  "mScRsp\020k\022\027\n\023CmdChooseLevelCsReq\020l\022\027\n\023Cmd"
  "ChooseLevelScRsp\020m\022\027\n\023CmdReadyInRoomCsRe"
  "q\020n\022\027\n\023CmdReadyInRoomScRsp\020o\022\027\n\023CmdTryLi"
  "nkTeamScRsp\020p\022\031\n\025CmdTeamHeartBeatCsReq\020q"
  "\022\025\n\021CmdGameStartScReq\020r\022\024\n\020CmdGameOverCs"
  "Req\020s\022\036\n\031CmdNotifyRoomStatusChange\020\311\001\022\031\n"
  "\024CmdNotifyRoomListMsg\020\312\001\022\027\n\022CmdNotifySCR"
  "oomMsg\020\313\001\022\031\n\024CmdNotifySCBattleMsg\020\314\001b\006pr"
  "oto3"
  ;
static const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable*const descriptor_table_cmdID_2eproto_deps[1] = {
};
static ::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase*const descriptor_table_cmdID_2eproto_sccs[1] = {
};
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_cmdID_2eproto_once;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_cmdID_2eproto = {
  false, false, descriptor_table_protodef_cmdID_2eproto, "cmdID.proto", 684,
  &descriptor_table_cmdID_2eproto_once, descriptor_table_cmdID_2eproto_sccs, descriptor_table_cmdID_2eproto_deps, 0, 0,
  schemas, file_default_instances, TableStruct_cmdID_2eproto::offsets,
  file_level_metadata_cmdID_2eproto, 0, file_level_enum_descriptors_cmdID_2eproto, file_level_service_descriptors_cmdID_2eproto,
};

// Force running AddDescriptors() at dynamic initialization time.
static bool dynamic_init_dummy_cmdID_2eproto = (static_cast<void>(::PROTOBUF_NAMESPACE_ID::internal::AddDescriptors(&descriptor_table_cmdID_2eproto)), true);
namespace pb {
const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* CmdID_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_cmdID_2eproto);
  return file_level_enum_descriptors_cmdID_2eproto[0];
}
bool CmdID_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 98:
    case 99:
    case 100:
    case 101:
    case 102:
    case 103:
    case 104:
    case 105:
    case 106:
    case 107:
    case 108:
    case 109:
    case 110:
    case 111:
    case 112:
    case 113:
    case 114:
    case 115:
    case 201:
    case 202:
    case 203:
    case 204:
      return true;
    default:
      return false;
  }
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace pb
PROTOBUF_NAMESPACE_OPEN
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
