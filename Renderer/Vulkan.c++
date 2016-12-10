#include "Vulkan.h"
#include "../Window/Linux/XCB.h"

Vulkan::Vulkan(){
	init();
}

void Vulkan::init(){
	#ifndef VK_USE_PLATFORM_XCB_KHR
	std::cout << "!!!!!!!!!!!!!!!!!!!!!!PLATFORM NOT DEFINED" << std::endl;
	#endif
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
	result = vkEnumeratePhysicalDevices(instance, &deviceCount, physicalDevices.data());
	
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
	int32_t queue_fam = 0;
	deviceQueueInfo.queueFamilyIndex = queue_fam;


	// create test queue
	float queuePriorities[] = { 1.0f };
	deviceQueueInfo.queueCount = 1;
	deviceQueueInfo.pQueuePriorities = queuePriorities;
	// set device
	deviceInfo.queueCreateInfoCount = 1;
	deviceInfo.pQueueCreateInfos = &deviceQueueInfo;

	VkDevice logical_device;
	result = vkCreateDevice(physicalDevices[0], &deviceInfo, NULL, &logical_device);
	error_check(result, "Logical Device Creation");

	// create command buffer -- 04 ------------------------------------------------
	VkCommandPool command_pool;
	VkCommandPoolCreateInfo command_pool_info = {};
	command_pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	command_pool_info.pNext = NULL;
	command_pool_info.queueFamilyIndex = queue_fam;
	command_pool_info.flags = 0;
	error_check(vkCreateCommandPool(logical_device, &command_pool_info, NULL, &command_pool),
		"Command Pool Creation");

	VkCommandBufferAllocateInfo command_buffer_info = {};
	VkCommandBuffer command_buffer;
	command_buffer_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	command_buffer_info.pNext = NULL;
	command_buffer_info.commandPool = command_pool;
	command_buffer_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	command_buffer_info.commandBufferCount = 1;
	error_check(vkAllocateCommandBuffers(logical_device, &command_buffer_info, &command_buffer),
		"Command BUffer Creation");

	// END 04 ---------------------------------------------------------------------



	// initialize surfaces
	PFN_vkGetPhysicalDeviceSurfaceFormatsKHR vkGetPhysicalDeviceSurfaceFormatsKHR = NULL;

	// somewhere in initialization code
	vkGetPhysicalDeviceSurfaceFormatsKHR = (PFN_vkGetPhysicalDeviceSurfaceFormatsKHR)vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceSurfaceFormatsKHR");
	if (!vkGetPhysicalDeviceSurfaceFormatsKHR) {
	    std::cout << "ERROR IN FUNCTION POINTER ACQUISITION\n";
	}

	XCB* xcb_window = new XCB(800,600);

	VkSurfaceKHR surface;
	#if defined(_WIN32)
	VkWin32SurfaceCreateInfoKHR surfaceCreateInfo;
	surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	surfaceCreateInfo.hinstance = (HINSTANCE)platformHandle; // provided by the platform code
	surfaceCreateInfo.hwnd = (HWND)platformWindow;           // provided by the platform code
	VkResult result = vkCreateWin32SurfaceKHR(instance, &surfaceCreateInfo, NULL, &surface);
	#elif defined(__ANDROID__)
	VkAndroidSurfaceCreateInfoKHR surfaceCreateInfo;
	surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR;
	surfaceCreateInfo.window = window;                       // provided by the platform code
	VkResult result = vkCreateAndroidSurfaceKHR(instance, &surfaceCreateInfo, NULL, &surface);
	#else
	VkXcbSurfaceCreateInfoKHR surfaceCreateInfo;
	surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
	surfaceCreateInfo.pNext = NULL;
	surfaceCreateInfo.connection = xcb_window->get_connection();
	surfaceCreateInfo.window = xcb_window->get_window();

	// ----- dammit what is happening 
	std::cout << "Surface Creation Sanity Check: " << std::endl;

	std::cout << "\tconnection:\t" << surfaceCreateInfo.connection << std::endl;
	std::cout << "\tget_connection()\t" << xcb_window->get_connection() << std::endl;
	std::cout << "\twindow:\t" << surfaceCreateInfo.window << std::endl;

	// --------------------------------


	VkResult result = vkCreateXcbSurfaceKHR(instance, &surfaceCreateInfo, NULL, &surface);
	error_check(result, "XCB surface creation");
	#endif

	VkPhysicalDevice preferredDevice = physicalDevices[0];
	std::cout << "Preferred Physical Device: " << preferredDevice << std::endl;

	// determine if physical device supports surfaced
	VkBool32 is_supported;
	error_check( vkGetPhysicalDeviceSurfaceSupportKHR(preferredDevice, 0, surface, &is_supported) , 
		"Physical device + surface support check"
	);

	if (is_supported == VK_TRUE)
		std::cout << "Physical device supports surface" << std::endl;
	else
		std::cout << "Physical device does not support surface" << std::endl;

	// store properties of physical device surface
	VkSurfaceCapabilitiesKHR surfaceCapabilities;
/*

THIS IS WHERE IT HITS THE FAN :(

*/


	result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevices[0], surface, &surfaceCapabilities);
	error_check(	result, "Getting device + surface capabilities");


	std::cout << "is this error not ruinous?" <<std::endl; // it is :(

	// swapchain construct
	deviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

	VkSwapchainCreateInfoKHR swapchain_create_info = {};
	swapchain_create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapchain_create_info.pNext = NULL;
	swapchain_create_info.surface = surface;


	swapchain_create_info.imageFormat = VK_FORMAT_B8G8R8A8_UNORM; // this could be problematic

	// minimum number of images for swapchain
//	uint32_t swapchain_image_count = 

	vkDestroyInstance(instance, NULL);
}

void Vulkan::populate_instance_info(VkInstanceCreateInfo& ii){
	// enumerate extensions
	enabledExtensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
	#if defined(_WIN32)
		enabledExtensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
	#else
		enabledExtensions.push_back(VK_KHR_XCB_SURFACE_EXTENSION_NAME);
	#endif


	//initialize instance creation information structure -----------
	ii.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	ii.pNext = NULL;
	ii.flags = 0;
	ii.pApplicationInfo = NULL;
	ii.enabledLayerCount = 0;
	ii.ppEnabledLayerNames = NULL;
	ii.enabledExtensionCount = enabledExtensions.size();
	ii.ppEnabledExtensionNames = enabledExtensions.data();
	std::cout << "initialized instance with " << ii.enabledExtensionCount << " extensions: " <<std::endl;
	for( int i=0; i < ii.enabledExtensionCount; ++i)
		std::cout << "\t" << ii.ppEnabledExtensionNames[i] << std::endl;

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
			exit(1);
	}
	std::cout << std::endl;
}