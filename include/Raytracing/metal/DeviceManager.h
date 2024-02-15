/**
 * @file DeviceManager.h
 * @author ayano
 * @date 2/13/24
 * @brief
 */

#ifndef RAYTRACING_DEVICEMANAGER_H
#define RAYTRACING_DEVICEMANAGER_H

#include "Metal.hpp"

class DeviceManager {
public:
	DeviceManager(const DeviceManager&) = delete;
	DeviceManager& operator=(const DeviceManager&) = delete;
	DeviceManager(DeviceManager&&) = delete;
	DeviceManager& operator=(DeviceManager&&) = delete;

	static DeviceManager& Instance() {
		static DeviceManager instance;
		return instance;
	}

	static NS::SharedPtr<MTL::Device> GetDevice() {
		static NS::SharedPtr<MTL::Device> device = NS::TransferPtr(MTL::CreateSystemDefaultDevice());
		return device;
	}

	static NS::SharedPtr<MTL::CommandQueue> GetCommandQueue() {
		static NS::SharedPtr<MTL::CommandQueue> commandQueue = NS::TransferPtr(GetDevice()->newCommandQueue());
		return commandQueue;
	}


private:
	DeviceManager() = default;
};


#endif // RAYTRACING_DEVICEMANAGER_H
