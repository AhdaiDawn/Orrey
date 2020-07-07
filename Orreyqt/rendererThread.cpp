#include "rendererThread.h"
#include "vulkanwindow.h"
#include <qdebug.h>
RendererThread::RendererThread(VulkanWindow* vulkanwindow)
{
	window = vulkanwindow;
}

RendererThread::~RendererThread()
{
}

void RendererThread::run() {
	while (1) {
		qDebug() << "thread";
		window->MainLoop();
	}
}
