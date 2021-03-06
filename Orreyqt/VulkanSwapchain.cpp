#include <spdlog/spdlog.h>
#include "VulkanSwapChain.h"
namespace vko {

	std::vector<VulkanTools::ImageResources> VulkanSwapchain::GetImages()
	{
		return swapchainImages;
	}

	void VulkanSwapchain::Destroy()
	{
		device.destroyImageView(depthImage.imageView);
		device.destroyImage(depthImage.image);
		device.freeMemory(depthImage.memory);

		device.destroyImageView(multiSampleImage.imageView);
		device.destroyImage(multiSampleImage.image);
		device.freeMemory(multiSampleImage.memory);

		for (auto& images : swapchainImages)
		{
			device.destroyImageView(images.imageView);
		}

		//swapchainImages会在交换链销毁时自动清除
		device.destroySwapchainKHR(swapchain);
	}
}