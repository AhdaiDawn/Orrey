#pragma once
#ifndef RENDERERTHREAD_H
#include <QThread>
class VulkanWindow;
class RendererThread: public QThread
{
public:
	RendererThread(VulkanWindow *vulkanwindow);
	~RendererThread();
	void run()override;

private:
	VulkanWindow* window;

};

#endif // !RENDERERTHREAD_H

