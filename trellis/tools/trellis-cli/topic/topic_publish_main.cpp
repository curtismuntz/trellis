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

#include <google/protobuf/util/json_util.h>

#include <cxxopts.hpp>

#include "trellis/core/node.hpp"
#include "trellis/tools/trellis-cli/constants.hpp"
#include "trellis/tools/trellis-cli/monitoring_utils.hpp"

namespace trellis {
namespace tools {
namespace cli {

using namespace trellis::core;

int topic_publish_main(int argc, char* argv[]) {
  cxxopts::Options options(topic_publish_command.data(), topic_publish_command_desc.data());
  options.add_options()("t,topic", "topic name", cxxopts::value<std::string>())(
      "b,body", "message body in JSON", cxxopts::value<std::string>())("c,count", "message count",
                                                                       cxxopts::value<int>()->default_value("1"))(
      "d,delay", "discovery delay milliseconds", cxxopts::value<int>()->default_value("1500"))(
      "r,rate", "publish rate hz", cxxopts::value<int>()->default_value("1"))("h,help", "print usage");
  auto result = options.parse(argc, argv);
  if (result.count("help") || !result.count("topic") || !result.count("body")) {
    std::cout << options.help() << std::endl;
    return 1;
  }

  const std::string topic = result["topic"].as<std::string>();
  const std::string body = result["body"].as<std::string>();
  const int count = result["count"].as<int>();
  const int rate = result["rate"].as<int>();
  const int delay_ms = result["delay"].as<int>();
  const int interval_ms = 1000 / rate;

  Node node(root_command.data());

  // Delay to give time for discovery
  std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));

  MonitorUtil mutil;

  auto message = mutil.GetMessageFromTopic(topic);

  if (message == nullptr) {
    std::cerr << "Could not get proto message from descriptor set." << std::endl;
    return 1;
  }
  auto pub = node.CreateDynamicPublisher(topic, message);

  google::protobuf::util::JsonParseOptions json_options;
  json_options.ignore_unknown_fields = false;
  json_options.case_insensitive_enum_parsing = false;

  auto r = google::protobuf::util::JsonStringToMessage(body, message.get(), json_options);

  std::cout << "Echoing " << count << " messages at " << rate << " hz to topic " << topic << std::endl;

  // Delay to give time for connection(s) to establish
  std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));

  for (int i = 0; i < count && node.RunOnce(); ++i) {
    pub->Send(*message);
    if (i < count - 1) {
      std::this_thread::sleep_for(std::chrono::milliseconds(interval_ms));
    }
  }

  return 0;
}

}  // namespace cli
}  // namespace tools
}  // namespace trellis
