#pragma once

#ifndef ORREYVK_H
#define ORREYVK_H
#include <random>
#include <limits>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
#include "Vulkan.h"

class VulkanWindow;

class OrreyVk : Vulkan {
public:
	void Run(VulkanWindow* vulkanwindow);
//	void InitWindow();
//	void InitQWindow();
	void Init(VulkanWindow* vulkanwindow);
	void MainLoop();
	void Cleanup();
	void UpdateCamera(float xPos, float yPos, float deltaTime);
	
	struct {
		glm::vec2 mousePos = glm::vec2();
		float zoom = -20.0f;
		float rotationSpeed = 7.5f;
		float movementSpeed = 1.0f;
		bool viewUpdated = false;

		glm::vec3 rotation = glm::vec3();
		glm::vec3 position = glm::vec3();

		struct {
			bool up = false;
			bool down = false;
			bool left = false;
			bool right = false;
		} keys;
	} m_camera;

	float m_speed = 1.0f;
	OrreyVk() {};

private:
//	GLFWwindow * m_window;
//	MainWindow* q_window;
//	VulkanWindow* v_window;
	SolidSphere m_sphere;
	SolidSphere m_skySphere;

	float m_frameTime = 1.0f;
	float m_totalRunTime = 0.0f;
	float m_seconds = 1.0f;

	struct PipelineInfo
	{
		vk::PipelineLayout layout;
		vk::Pipeline pipeline;
	};

	struct {
		struct {
			glm::mat4 projection;
			glm::mat4 model;
			glm::mat4 view;
		} ubo;

		vko::Buffer uniformBuffer;
		vk::DescriptorSetLayout descriptorSetLayout;
		vk::DescriptorSet descriptorSet;
		vk::DescriptorSet skySphereDescriptorSet;
		PipelineInfo pipelinePlanets;
		PipelineInfo pipelineOrbits;
		PipelineInfo pipelineSkySphere;
		vk::Semaphore semaphore;
	} m_graphics;

	struct {
		vko::Buffer uniformBuffer;
		vko::VulkanCommandPool commandPool;
		vk::CommandBuffer cmdBuffer;
		vk::DescriptorSetLayout descriptorSetLayout;
		vk::DescriptorSet descriptorSet;
		vk::PipelineLayout pipelineLayout;
		vk::Pipeline pipeline;
		vk::Semaphore semaphore;
		struct {
			float deltaT;
			int32_t objectCount;
			int32_t scale;
			float speed;
		} ubo;
	} m_compute;

	struct CelestialObj {
		glm::vec4 position; //xyz Position, w Mass
		glm::vec4 velocity; //xyz Velocity
		glm::vec4 scale;	//xyz Scale w texIndex
		glm::vec4 rotation; //xyz Current rotation on each axis
		glm::vec4 rotationSpeed; //xyz Rotation speed for each axis
		glm::vec4 posOffset; //If w != 0 then object is a moon and this describes the position of the object it is orbiting
		glm::vec4 orbitalTilt; //xyz Angle of tilt for the orbital plane
		glm::vec4 colourTint = glm::vec4(1.0);
	};

	struct {
		vko::Buffer m_bufferVertexOrbit;
		std::vector<int> vertices;
		std::vector<int> offsets;
	} m_orbitVertexInfo;

	vko::Buffer m_bufferVertex;
	vko::Buffer m_bufferIndex;
	vko::Buffer m_bufferInstance;
	vko::Image m_textureArrayPlanets;
	vko::Image m_textureStarfield;
	vk::QueryPool m_queryPool;
	std::vector<uint64_t> m_queryResults;
	
	void CreateCommandBuffers();
	void CreateDescriptorPool();
	void CreateDescriptorSetLayout();
	void CreateDescriptorSet();
	void CreateGraphicsPipelineLayout();
	void CreateGraphicsPipeline();

	void RenderFrame();

	void PrepareInstance();
	void UpdateCameraUniformBuffer();
	void UpdateComputeUniformBuffer();
	void PrepareCompute();
	void CreateComputeCommandBuffer();

	std::vector<glm::vec2> CalculateOrbitPoints(glm::vec4 pos, glm::vec4 vel, double G, float timestep);

	float RandomRange(float min, float max) { return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min))); }

	double GetTimeQueryResult(uint32_t timeStampValidBits);

};

#endif // ORREYVK_H