#include "Vulkan.h"
#include <vector>

Vulkan::Vulkan(){
	init();
}

void Vulkan::init(){
	//initialize instance creation information structure -----------
	populate_instance_info(instanceInfo);

	// initialize application information structure ----------------
	populate_application_info(applicationInfo);

	// create instance ---------------------------------------------
	result = vkCreateInstance(&instanceInfo, NULL, &instance);
	error_check(result, "Instance creation");

	// enumerate devices ---------------------------------------------
	uint32_t deviceCount = 0;
	result = vkEnumeratePhysicalDevices(instance, &deviceCount, NULL);
	error_check(result, "Device Enumeration");
	std::cout << "Devices enumerated: " << deviceCount << std::endl;

	std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
	result = vkEnumeratePhysicalDevices(instance, &deviceCount, &physicalDevices[0]);
	
	VkPhysicalDeviceProperties deviceProperties;

	// list of device properties per device
	std::vector<VkPhysicalDeviceProperties> devicesProperties(deviceCount);
	for (uint32_t i = 0; i < deviceCount; ++i) {
		VkPhysicalDeviceProperties deviceProperties;
	    memset(&deviceProperties, 0, sizeof deviceProperties);
	    vkGetPhysicalDeviceProperties(physicalDevices[i], &deviceProperties);
	    std::cout << "Driver Version: " << deviceProperties.driverVersion << std::endl;
	    std::cout << "Device Name: " << deviceProperties.deviceName << std::endl;
	    std::cout << "Device Type: " << deviceProperties.deviceType << std::endl;
	    std::cout << "API Version:  " << 
	        // See note below regarding this:
	        ((deviceProperties.apiVersion>>22)&0x3FF) <<"."<<
	        ((deviceProperties.apiVersion>>12)&0x3FF) <<"."<<
	        ((deviceProperties.apiVersion&0xFFF)) << std::endl;
	    devicesProperties[i] = deviceProperties;
	}

	// enumerate queue families -----------------------------------
	// list of device queues per device
	std::vector<std::vector<VkQueueFamilyProperties>> deviceQueueFamilyProperties(deviceCount);
	for (uint32_t i = 0; i < deviceCount; i++) {
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevices[i], &queueFamilyCount, NULL);
		std::vector<VkQueueFamilyProperties> familyProperties(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevices[i], &queueFamilyCount, &familyProperties[0]);
		std::cout << "Queue Families for device " << devicesProperties[i].deviceName << ": " << queueFamilyCount << std::endl;
	    for (uint32_t j = 0; j < queueFamilyCount; j++) {
	    	std::cout << "\tFAMILY " << j << std::endl;
	        std::cout << "\t- Count of Queues: " << familyProperties[j].queueCount << std::endl;
	        std::cout << "\t- Supported operations on this queue:\n";
	        if (familyProperties[j].queueFlags & VK_QUEUE_GRAPHICS_BIT)
	            std::cout << "\t\t Graphics\n";
	        if (familyProperties[j].queueFlags & VK_QUEUE_COMPUTE_BIT)
	            std::cout << "\t\t Compute\n";
	        if (familyProperties[j].queueFlags & VK_QUEUE_TRANSFER_BIT)
	            std::cout << "\t\t Transfer\n";
	        if (familyProperties[j].queueFlags & VK_QUEUE_SPARSE_BINDING_BIT)
	            std::cout << "\t\t Sparse Binding\n";
	    }
	    deviceQueueFamilyProperties[i] = familyProperties;
	}

	// logical device creation
	VkDeviceCreateInfo deviceInfo;
	deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceInfo.pNext = NULL;
	deviceInfo.flags = 0;

	deviceInfo.enabledLayerCount = 0;
	deviceInfo.ppEnabledLayerNames = NULL;
	deviceInfo.enabledExtensionCount = 0;
	deviceInfo.ppEnabledExtensionNames = NULL;

	deviceInfo.pEnabledFeatures = NULL;

	// initialize queues
	VkDeviceQueueCreateInfo deviceQueueInfo;
	deviceQueueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	deviceQueueInfo.pNext = NULL;
	deviceQueueInfo.flags = 0;
	//first family best family
	deviceQueueInfo.queueFamilyIndex = 0;


	vkDestroyInstance(instance, NULL);
}

void Vulkan::populate_instance_info(VkInstanceCreateInfo& ii){
		//initialize instance creation information structure -----------
	ii.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	ii.pNext = NULL;
	ii.flags = 0;
	ii.pApplicationInfo = NULL;
	ii.enabledLayerCount = 0;
	ii.ppEnabledLayerNames = NULL;
	ii.enabledExtensionCount = 0;
	ii.ppEnabledExtensionNames = NULL;
}

void Vulkan::populate_application_info(VkApplicationInfo& ai){
	ai.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	ai.pNext = NULL;
	ai.pApplicationName = "GFXENG";
	ai.applicationVersion = 0;
	ai.pEngineName = "DDD";
	ai.engineVersion = 0;
	ai.apiVersion = VK_API_VERSION_1_0;
}


void Vulkan::error_check(VkResult error_code, const std::string& error_type){
    /*
    VK_NOT_READY = 1,
    VK_TIMEOUT = 2,
    VK_EVENT_SET = 3,
    VK_EVENT_RESET = 4,
    VK_INCOMPLETE = 5,
    VK_ERROR_OUT_OF_HOST_MEMORY = -1,
    VK_ERROR_OUT_OF_DEVICE_MEMORY = -2,
    VK_ERROR_INITIALIZATION_FAILED = -3,
    VK_ERROR_DEVICE_LOST = -4,
    VK_ERROR_MEMORY_MAP_FAILED = -5,
    VK_ERROR_LAYER_NOT_PRESENT = -6,
    VK_ERROR_EXTENSION_NOT_PRESENT = -7,
    VK_ERROR_FEATURE_NOT_PRESENT = -8,
    VK_ERROR_INCOMPATIBLE_DRIVER = -9,
    VK_ERROR_TOO_MANY_OBJECTS = -10,
    VK_ERROR_FORMAT_NOT_SUPPORTED = -11,
    VK_ERROR_FRAGMENTED_POOL = -12,
*/

	std::cout << "ERROR CHECK <" << error_type << ">: ";
	switch(error_code){
		case VK_ERROR_INCOMPATIBLE_DRIVER: 
			std::cout << "no driver error";
			break;
		case VK_SUCCESS:
			std::cout << "error free!";
			break;
		default:
			std::cout << "some other weird error happened: " << (int)error_code;
	}
	std::cout << std::endl;
}