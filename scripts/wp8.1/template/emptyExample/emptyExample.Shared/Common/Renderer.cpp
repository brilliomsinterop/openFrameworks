﻿#include "pch.h"
#include "Renderer.h"
#include "ofAppRunner.h"
#include "ofAppWinRTWindow.h"

#include "..\Common\AngleHelper.h"


using namespace DirectX;
using namespace Windows::Foundation;
using namespace Windows::UI::Core;

extern int ofmain();

namespace AngleApp
{


// Loads vertex and pixel shaders from files and instantiates the cube geometry.
Renderer::Renderer(const std::shared_ptr<AngleApp::DeviceResources>& deviceResources) :
	m_loadingComplete(false),
	m_deviceResources(deviceResources)
{
    CreateDeviceDependentResources();
}

// Initializes view parameters when the window size changes.
void Renderer::CreateWindowSizeDependentResources()
{
    if (m_loadingComplete)
    {
        Size outputSize = m_deviceResources->GetOutputSize();
        Size logicalSize = m_deviceResources->GetLogicalSize();
        ofAppWinRTWindow* window = reinterpret_cast<ofAppWinRTWindow*>(ofGetWindowPtr());
        window->OnWindowSizeChanged(logicalSize.Width, logicalSize.Height);
    }
}

// Called once per frame, rotates the cube and calculates the model and view matrices.
void Renderer::Update(AngleApp::StepTimer const& timer)
{

}

void Renderer::OnPointerPressed(PointerEventArgs^ e)
{
    ofAppWinRTWindow* window = reinterpret_cast<ofAppWinRTWindow*>(ofGetWindowPtr());
    window->OnPointerPressed(e);
}

void Renderer::OnPointerMoved(PointerEventArgs^ e)
{
    ofAppWinRTWindow* window = reinterpret_cast<ofAppWinRTWindow*>(ofGetWindowPtr());
    window->OnPointerMoved(e);
}

void Renderer::OnPointerReleased(PointerEventArgs^ e)
{
    ofAppWinRTWindow* window = reinterpret_cast<ofAppWinRTWindow*>(ofGetWindowPtr());
    window->OnPointerReleased(e);
}

void Renderer::AddPointerEvent(PointerEventType type, PointerEventArgs^ args)
{
    std::lock_guard<std::mutex> guard(mMutex);
    std::shared_ptr<PointerEvent> e(new PointerEvent(type, args));
    mInputEvents.push(e);
}

void Renderer::ProcessEvents()
{
    std::lock_guard<std::mutex> guard(mMutex);

    while (!mInputEvents.empty())
    {
        InputEvent* e = mInputEvents.front().get();
        e->execute(this);
        mInputEvents.pop();
    }
}


// Renders one frame using the vertex and pixel shaders.
void Renderer::Render()
{
	if (m_loadingComplete)
	{
        m_deviceResources->aquireContext();

        ProcessEvents();
#if 0
        glClearColor(1.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
#else
        ofAppWinRTWindow* window = reinterpret_cast<ofAppWinRTWindow*>(ofGetWindowPtr());
        window->RunOnce();
#endif
        m_deviceResources->Present();

        m_deviceResources->releaseContext();
    }
}

void Renderer::CreateDeviceDependentResources()
{
    if (!m_loadingComplete)
    {
        m_deviceResources->aquireContext();
        ofmain();
        m_loadingComplete = true;
        m_deviceResources->releaseContext();
    }
}

void Renderer::ReleaseDeviceDependentResources()
{
	m_loadingComplete = false;
}

}