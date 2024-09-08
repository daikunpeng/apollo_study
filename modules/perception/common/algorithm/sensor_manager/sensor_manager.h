/******************************************************************************
 * Copyright 2018 The Apollo Authors. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/
#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "cyber/common/macros.h"
#include "modules/perception/common/base/camera.h"
#include "modules/perception/common/base/distortion_model.h"
#include "modules/perception/common/base/sensor_meta.h"
#include "modules/perception/common/perception_gflags.h"

namespace apollo {
namespace perception {
namespace algorithm {

using apollo::perception::base::BaseCameraDistortionModel;
using apollo::perception::base::BaseCameraModel;



// **类定义：**
// `SensorManager` 类是一个单例类，负责管理传感器信息并提供方法来访问和查询传感器数据。

// **方法：**

// * `Init()`: 初始化传感器管理器。
// * `IsSensorExist(const std::string& name) const`: 检查是否存在一个名为 `name` 的传感器。
// * `GetSensorInfo(const std::string& name, apollo::perception::base::SensorInfo* sensor_info) const`: 获取名为 `name` 的传感器的信息。
// * `GetDistortCameraModel(const std::string& name) const` 和 `GetUndistortCameraModel(const std::string& name) const`: 获取名为 `name` 的传感器的畸变和非畸变相机模型。
// * `IsHdLidar`、`IsLdLidar`、`IsLidar`、`IsRadar`、`IsCamera`、`IsUltrasonic` 和 `IsMainSensor` 方法：检查名为 `name` 的传感器或类型是否为特定类型（例如 HD LiDAR、LD LiDAR 等）。
// * `GetFrameId(const std::string& name) const`: 获取名为 `name` 的传感器的帧 ID。
// * `IntrinsicPath(const std::string& frame_id)`: 一个私有方法，构造名为 `frame_id` 的传感器的内在路径。

// 注意：这个类是为了管理传感器信息并提供方法来访问和查询传感器数据，但它不提供任何实际的传感器数据或功能。

class SensorManager {
 public:
  bool Init();

  bool IsSensorExist(const std::string& name) const;

  bool GetSensorInfo(const std::string& name,
                     apollo::perception::base::SensorInfo* sensor_info) const;

  std::shared_ptr<BaseCameraDistortionModel> GetDistortCameraModel(
      const std::string& name) const;

  std::shared_ptr<BaseCameraModel> GetUndistortCameraModel(
      const std::string& name) const;

  // sensor type functions
  bool IsHdLidar(const std::string& name) const;
  bool IsHdLidar(const apollo::perception::base::SensorType& type) const;
  bool IsLdLidar(const std::string& name) const;
  bool IsLdLidar(const apollo::perception::base::SensorType& type) const;

  bool IsLidar(const std::string& name) const;
  bool IsLidar(const apollo::perception::base::SensorType& type) const;
  bool IsRadar(const std::string& name) const;
  bool IsRadar(const apollo::perception::base::SensorType& type) const;
  bool IsCamera(const std::string& name) const;
  bool IsCamera(const apollo::perception::base::SensorType& type) const;
  bool IsUltrasonic(const std::string& name) const;
  bool IsUltrasonic(const apollo::perception::base::SensorType& type) const;

  bool IsMainSensor(const std::string& name) const;

  // sensor frame id function
  std::string GetFrameId(const std::string& name) const;

 private:
  inline std::string IntrinsicPath(const std::string& frame_id) {
    std::string intrinsics =
        FLAGS_obs_sensor_intrinsic_path + "/" + frame_id + "_intrinsics.yaml";
    return intrinsics;
  }

 private:
  std::mutex mutex_;
  bool inited_ = false;

  std::unordered_map<std::string, apollo::perception::base::SensorInfo>
      sensor_info_map_;
  std::unordered_map<std::string, std::shared_ptr<BaseCameraDistortionModel>>
      distort_model_map_;
  std::unordered_map<std::string, std::shared_ptr<BaseCameraModel>>
      undistort_model_map_;

  DECLARE_SINGLETON(SensorManager)
};

}  // namespace algorithm
}  // namespace perception
}  // namespace apollo
