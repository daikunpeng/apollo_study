/* Copyright 2017 The Apollo Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
=========================================================================*/

#include "gflags/gflags.h"

#include "modules/common_msgs/map_msgs/map.pb.h"

#include "cyber/common/file.h"
#include "cyber/common/log.h"
#include "modules/map/hdmap/hdmap_util.h"

/**
 * A map tool to transform .txt map to .bin map
 */

DEFINE_string(output_dir, "/tmp", "output map directory");

int main(int argc, char *argv[]) {
  google::InitGoogleLogging(argv[0]);
  FLAGS_alsologtostderr = true;

  google::ParseCommandLineFlags(&argc, &argv, true);

  const auto map_filename = FLAGS_map_dir + "/base_map.txt";
  apollo::hdmap::Map pb_map; // 定义一个Map类型的pb_map
  if (!apollo::cyber::common::GetProtoFromFile(map_filename, &pb_map)) {//GetProtoFromFile 函数是Apollo 实现的 google::protobuf的接口的封装，用于解析protobuf 文件
    // 这意味着 apollo 的高精度地图格式是基于protobuf 实现的
    AERROR << "Failed to load txt map from " << map_filename;
    return -1;
  } else {
    AINFO << "Loaded txt map from " << map_filename;
  }

  // Note: 
  // 1. imap 库是apollo 工作人员开发的用于转换opendrive 格式到apollo 高精度地图格式的库
  // 2. 这个库就是实现了 opendrive 格式到apollo 高精度地图格式的转换

  const std::string output_bin_file = FLAGS_output_dir + "/base_map.bin";
  if (!apollo::cyber::common::SetProtoToBinaryFile(pb_map, output_bin_file)) {//SetProtoToBinaryFile 函数是Apollo 实现的 google::protobuf的接口的封装，用于将protobuf 消息序列化为二进制文件
    AERROR << "Failed to generate binary base map";
    return -1;
  }

  pb_map.Clear();
  ACHECK(apollo::cyber::common::GetProtoFromFile(output_bin_file, &pb_map))
      << "Failed to load generated binary base map";

  AINFO << "Successfully converted .txt map to .bin map: " << output_bin_file;

  return 0;
}
