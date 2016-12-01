#pragma once

#include <vulkan/vulkan.h>
#include <iostream>
#include <cstring>

#define VK_VER_MAJOR(X) ((((uint32_t)(X))>>22)&0x3FF)
#define VK_VER_MINOR(X) ((((uint32_t)(X))>>12)&0x3FF)
#define VK_VER_PATCH(X) (((uint32_t)(X)) & 0xFFF)

class Vulkan{
	VkApplicationInfo applicationInfo;
	VkInstanceCreateInfo instanceInfo;
	VkInstance instance;
	VkResult result = VK_NOT_READY;

	//constructor
	//initializer
	void init();

	// error checker
	void error_check(VkResult error_code, const std::string&);
	void populate_instance_info(VkInstanceCreateInfo& ii);
	void populate_application_info(VkApplicationInfo& ai);

public:
	Vulkan();
};