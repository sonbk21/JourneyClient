/////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright � 2015 Daniel Allendorf                                        //
//                                                                          //
// This program is free software: you can redistribute it and/or modify     //
// it under the terms of the GNU Affero General Public License as           //
// published by the Free Software Foundation, either version 3 of the       //
// License, or (at your option) any later version.                          //
//                                                                          //
// This program is distributed in the hope that it will be useful,          //
// but WITHOUT ANY WARRANTY; without even the implied warranty of           //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            //
// GNU Affero General Public License for more details.                      //
//                                                                          //
// You should have received a copy of the GNU Affero General Public License //
// along with this program.  If not, see <http://www.gnu.org/licenses/>.    //
//////////////////////////////////////////////////////////////////////////////
#pragma once
#include "Journey.h"
#ifdef JOURNEY_USE_OPENGL
#include "WindowGLFW.h"
#include "glm.hpp"
#include "UI.h"
#include <iostream>

namespace IO
{
	WindowGLFW::WindowGLFW()
	{
		glwnd = nullptr;
	}

	WindowGLFW::~WindowGLFW()
	{
		if (glwnd) glfwDestroyWindow(glwnd);
		glfwTerminate();
	}

	void error_callback(int error, const char* description)
	{
		std::cout << "Error no.:" << error << " : " << description << std::endl;
	}

	static void key_callback(GLFWwindow* window, int key, int, int action, int)
	{
		Keyboard* keyboard = reinterpret_cast<Keyboard*>(glfwGetWindowUserPointer(window));
		if (keyboard)
		{
			keyboard->sendinput(action != GLFW_RELEASE, key);
		}
	}

	bool WindowGLFW::init()
	{
		fullscreen = false;

		if (glfwInit())
		{
			glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
			context = glfwCreateWindow(1, 1, "", nullptr, nullptr);
			glfwMakeContextCurrent(context);
			glfwWindowHint(GLFW_VISIBLE, GL_TRUE);
			glfwSetErrorCallback(error_callback);
			return initwindow();
		}
		else
		{
			return false;
		}
	}

	bool WindowGLFW::initwindow()
	{
		if (glwnd) glfwDestroyWindow(glwnd);
		glwnd = glfwCreateWindow(800, 600, "Journey", fullscreen ? glfwGetPrimaryMonitor() : nullptr, context);
		if (glwnd)
		{
			glfwMakeContextCurrent(glwnd);
			glfwSwapInterval(0);
			glfwSetInputMode(glwnd, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
			glfwSetInputMode(glwnd, GLFW_STICKY_KEYS, 1);
			glfwSetKeyCallback(glwnd, key_callback);
			glfwSetWindowUserPointer(glwnd, &UI::getkeyboard());

			glLoadIdentity();
			glOrtho(0.0, 800, 590, -10, -1.0, 1.0);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_TEXTURE_2D);
			return true;
		}
		else
		{
			return false;
		}
	}

	void WindowGLFW::update()
	{
		double cursorx;
		double cursory;
		glfwGetCursorPos(glwnd, &cursorx, &cursory);
		int32_t state = glfwGetMouseButton(glwnd, GLFW_MOUSE_BUTTON_LEFT);

		Cursor::Mousestate mst = (state == GLFW_PRESS) ? Cursor::MST_CLICKING : Cursor::MST_IDLE;
		UI::sendmouse(mst, vector2d<int16_t>(static_cast<int16_t>(cursorx), static_cast<int16_t>(cursory)));

		int32_t tabstate = glfwGetKey(glwnd, GLFW_KEY_TAB);
		if (tabstate == GLFW_PRESS)
		{
			fullscreen = !fullscreen;
			initwindow();
		}
	}

	void WindowGLFW::begin() const
	{
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);
		//glEnable(GL_TEXTURE_2D);
	}

	void WindowGLFW::end() const
	{
		//glDisable(GL_TEXTURE_2D);
		glfwPollEvents();
		glfwSwapBuffers(glwnd);
	}
}
#endif