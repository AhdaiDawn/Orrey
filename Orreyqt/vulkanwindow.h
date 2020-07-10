#pragma once

#ifndef VULKANWINDOW_H
#define VULKANWINDOW_H

#include <random>
#include <limits>
#include <spdlog/spdlog.h>
#include <QWindow>
#include <QTimer>
#include "Vulkan.h"

class VulkanWindow : public QWindow, private Vulkan
{
	Q_OBJECT

public slots:
	void MainLoop();

signals:
	void updateFPSLcd(int);
	void updateObjectSizeLCD(int);
	void updateAstroidObjectNumLCD(int);

public:
	VulkanWindow();
	~VulkanWindow();

	void mousePressEvent(QMouseEvent*) override;
	void mouseReleaseEvent(QMouseEvent*) override;
	void mouseMoveEvent(QMouseEvent*) override;
	void keyPressEvent(QKeyEvent*) override;
	void keyReleaseEvent(QKeyEvent*) override;
	void wheelEvent(QWheelEvent*) override;
//	void resizeEvent(QResizeEvent* ev) override {
//		spdlog::info("Image extent: {} {}", this->width(), this->height());
//	}

	void startRender();
	void init();
	void cleanUp();
	bool togglePaused();
	void recreateSwapchain();
	bool hideOrbits();
	void updateMouseCamera(float xPos, float yPos, float deltaTime);
	void updateKeyCamera(float deltaTime);
	void addAstroidObjectNum();
	void subAstroidObjectNum();
	int getObjectNum();
	void addObjectSize();
	void subObjectSize();
	int getObjectSize() { return object_scale; }

	int getFps() { return static_cast<int>(1.f / m_frameTime); }

	struct
	{
		glm::vec2 mousePos = glm::vec2();
		float zoom = -50.0f;
		float rotationSpeed = 7.5f;
		float movementSpeed = 1.0f;
		bool viewUpdated = false;

		glm::vec3 rotation = glm::vec3();
		glm::vec3 position = glm::vec3();

		struct
		{
			bool up = false;
			bool down = false;
			bool left = false;
			bool right = false;
			bool accelerate = false;
			bool moderate = false;
		} keys;
	} m_camera;

	float m_speed = 1.0f;

private:

	double xPos = 0, yPos = 0;


	int objects_per_group = 512;  //小行星每组多少个
	int saturn_ring_object_count = 1024*3; //木星环的数量 3000
	int astroid_belt_max_object_count = 512*100; //小行星环个数 250000
	int object_scale = 15; //缩放 5-30 step 5

	bool m_mouse_pressed = false;
	bool m_key_pressed = false;
	bool enable_hide = false;
	bool enable_orbits = true;

	QTimer* m_timer;
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

	struct
	{
		struct
		{
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

	struct
	{
		vko::Buffer uniformBuffer;
		vko::VulkanCommandPool commandPool;
		vk::CommandBuffer cmdBuffer;
		vk::DescriptorSetLayout descriptorSetLayout;
		vk::DescriptorSet descriptorSet;
		vk::PipelineLayout pipelineLayout;
		vk::Pipeline pipeline;
		vk::Semaphore semaphore;
		struct
		{
			float deltaT;
			int32_t objectCount;
			int32_t scale;
			float speed;
		} ubo;
	} m_compute;

	struct CelestialObj
	{
		glm::vec4 position; //xyz Position, w Mass
		glm::vec4 velocity; //xyz Velocity
		glm::vec4 scale;	//xyz Scale w texIndex
		glm::vec4 rotation; //xyz Current rotation on each axis
		glm::vec4 rotationSpeed; //xyz Rotation speed for each axis
		glm::vec4 posOffset; //If w != 0 then object is a moon and this describes the position of the object it is orbiting
		glm::vec4 orbitalTilt; //xyz Angle of tilt for the orbital plane
		glm::vec4 colourTint = glm::vec4(1.0);
	};

	struct
	{
		vko::Buffer m_bufferVertexOrbit;
		std::vector<int> vertices;//记得pop()
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

#endif // VULKANWINDOW_H
