/*
 * Copyright (C) 2021 Agtonomy
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include "monitoring_utils.hpp"

#include <google/protobuf/descriptor.pb.h>

#include <algorithm>
#include <sstream>

namespace trellis {
namespace tools {
namespace cli {

std::ostream& operator<<(std::ostream& ostream, const eCAL::pb::Topic& topic) {
  ostream << "tname        : " << topic.tname() << std::endl;      // topic name
  ostream << "ttype        : " << topic.ttype() << std::endl;      // topic type
  ostream << "direction    : " << topic.direction() << std::endl;  // direction (publisher, subscriber)
  ostream << "hname        : " << topic.hname() << std::endl;      // host name
  ostream << "pid          : " << topic.pid() << std::endl;        // process id
  ostream << "tid          : " << topic.tid() << std::endl;        // topic id
  return ostream;
}
std::ostream& operator<<(std::ostream& ostream, const eCAL::pb::Process& node) {
  ostream << "rclock                 : " << node.rclock() << std::endl;
  ostream << "hname                  : " << node.hname() << std::endl;
  ostream << "pid                    : " << node.pid() << std::endl;
  ostream << "pname                  : " << node.pname() << std::endl;
  ostream << "uname                  : " << node.uname() << std::endl;
  ostream << "pparam                 : " << node.pparam() << std::endl;
  ostream << "pmemory                : " << node.pmemory() << std::endl;
  ostream << "pcpu                   : " << node.pcpu() << std::endl;
  ostream << "usrptime               : " << node.usrptime() << std::endl;
  ostream << "datawrite              : " << node.datawrite() << std::endl;
  ostream << "dataread               : " << node.dataread() << std::endl;

  // TODO(bsirang) add these if desired
  // ostream << "state                  :" << node.state() << std::endl;
  // ostream << "tsync_state            :" << node.tsync_state() << std::endl;
  // ostream << "tsync_mod_name         :" << node.tsync_mod_name() << std::endl;
  // ostream << "component_init_state   :" << node.component_init_state() << std::endl;
  ostream << "component_init_info    :" << node.component_init_info() << std::endl;
  return ostream;
}

std::ostream& operator<<(std::ostream& ostream, const eCAL::pb::Host& host) {
  ostream << "hname        : " << host.hname() << std::endl;
  ostream << "os.osname    : " << host.os().osname() << std::endl;
  return ostream;
}

std::ostream& operator<<(std::ostream& ostream, const eCAL::pb::Service& service) {
  ostream << "rclock     : " << service.rclock() << std::endl;
  ostream << "hname      : " << service.hname() << std::endl;
  ostream << "pname      : " << service.pname() << std::endl;
  ostream << "uname      : " << service.uname() << std::endl;
  ostream << "pid        : " << service.pid() << std::endl;
  ostream << "sname      : " << service.sname() << std::endl;
  ostream << "tcp_port   : " << service.tcp_port() << std::endl;
  return ostream;
}

std::ostream& operator<<(std::ostream& ostream, const eCAL::pb::Method& method) {
  ostream << "mname          : " << method.mname() << std::endl;
  ostream << "req_type       : " << method.req_type() << std::endl;
  ostream << "resp_type      : " << method.resp_type() << std::endl;
  ostream << "call_count     : " << method.call_count() << std::endl;
  return ostream;
}

MonitorUtil::MonitorUtil() { static_cast<void>(UpdateSnapshot()); }

const eCAL::pb::Monitoring& MonitorUtil::UpdateSnapshot() {
  std::string snapshot_raw;
  eCAL::pb::Monitoring snapshot;
  if (eCAL::Monitoring::GetMonitoring(snapshot_raw)) {
    snapshot.ParseFromString(snapshot_raw);
    snapshot_raw_ = snapshot_raw;
    snapshot_ = snapshot;
  }
  return snapshot_;
}

std::shared_ptr<google::protobuf::Message> MonitorUtil::GetMessageFromTopic(const std::string& topic) {
  std::string topic_type = eCAL::Util::GetTypeName(topic);
  topic_type = topic_type.substr(topic_type.find_first_of(':') + 1, topic_type.size());
  topic_type = topic_type.substr(topic_type.find_last_of('.') + 1, topic_type.size());

  if (topic_type.size() == 0) {
    std::stringstream msgstream;
    msgstream << "Could not retrieve topic type for " << topic << ". Are there any subscribers online?";
    throw std::runtime_error(msgstream.str());
  }

  std::string topic_description = eCAL::Util::GetDescription(topic);
  if (topic_description.size() == 0) {
    std::stringstream msgstream;
    msgstream << "Could not retrieve topic description for " << topic << ".";
    throw std::runtime_error(msgstream.str());
  }

  google::protobuf::FileDescriptorSet proto_desc;
  proto_desc.ParseFromString(topic_description);
  std::string error_s;

  auto message = std::shared_ptr<google::protobuf::Message>{
      decoder_.GetProtoMessageFromDescriptorSet(proto_desc, topic_type, error_s)};

  if (error_s.size() > 0) {
    throw std::runtime_error(error_s);
  }

  return message;
}

void MonitorUtil::PrintTopics() const {
  const auto& topics = snapshot_.topics();
  PrintEntries<eCAL::pb::Topic>(topics);
}

void MonitorUtil::PrintNodes() const {
  const auto& nodes = snapshot_.processes();
  PrintEntries<eCAL::pb::Process>(nodes);
}

void MonitorUtil::PrintHosts() const {
  const auto& hosts = snapshot_.hosts();
  PrintEntries<eCAL::pb::Host>(hosts);
}

void MonitorUtil::PrintServices() const {
  const auto& services = snapshot_.services();
  PrintEntries<eCAL::pb::Service>(services);
}

void MonitorUtil::PrintServiceInfo(const std::string service_name) const {
  const auto& services = snapshot_.services();
  auto filter = [service_name](const eCAL::pb::Service& service) { return service.sname() == service_name; };
  auto it = GetFilteredIterator<eCAL::pb::Service>(services, filter);
  if (it == services.end()) {
    std::cerr << "Didn't find any service with the name " << service_name << std::endl;
    return;
  }

  // We expect only one match, but we'll iterate over everything that came out
  // of the filter regardless
  while (it != services.end()) {
    const auto& service = *it;
    std::cout << "=============================================================" << std::endl;
    std::cout << service;
    std::cout << std::endl << std::endl;
    std::cout << "Methods:" << std::endl;
    const auto& methods = service.methods();
    for (const auto& method : methods) {
      std::cout << method;
    }
    ++it;
  }
}

}  // namespace cli
}  // namespace tools
}  // namespace trellis
