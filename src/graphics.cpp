//////////////////////////////////////////////////////////////////////////
// Copyright 2012 Peter Atechian (Retep998)                             //
//////////////////////////////////////////////////////////////////////////
// This file is part of NoLifeStory.                                    //
//                                                                      //
// NoLifeStory is free software: you can redistribute it and/or modify  //
// it under the terms of the GNU General Public License as published by //
// the Free Software Foundation, either version 3 of the License, or    //
// (at your option) any later version.                                  //
//                                                                      //
// NoLifeStory is distributed in the hope that it will be useful,       //
// but WITHOUT ANY WARRANTY; without even the implied warranty of       //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        //
// GNU General Public License for more details.                         //
//                                                                      //
// You should have received a copy of the GNU General Public License    //
// along with NoLifeStory.  If not, see <http://www.gnu.org/licenses/>. //
//////////////////////////////////////////////////////////////////////////
#include "Global.h"

namespace NLS {
    namespace Graphics {
        sf::Window* Window;
        void Init() {
#ifdef _DEBUG
            Window = new sf::Window(sf::VideoMode(1440, 900), "NoLifeStory", sf::Style::Close, sf::ContextSettings(0, 0, 0, 2, 1));
#else
            Window = new sf::Window(sf::VideoMode::getFullscreenModes()[0], "NoLifeStory", sf::Style::Close|sf::Style::Fullscreen, sf::ContextSettings(0, 0, 0, 2, 1));
#endif
            Window->setKeyRepeatEnabled(true);
            Window->setMouseCursorVisible(false);
            Window->setVerticalSyncEnabled(true);
            View::Width = Window->getSize().x;
            View::Height = Window->getSize().y;
            HANDLE hIcon = LoadIconW(GetModuleHandleW(NULL), MAKEINTRESOURCEW(IDI_NOLIFESTORY_ICON));
	        HWND hWnd = Window->getSystemHandle();
            SendMessage(hWnd, WM_SETICON, ICON_SMALL2, (LPARAM)hIcon);
            glewExperimental = true;
            GLenum error = glewInit();
            if (error != GLEW_OK) die();
            glClear(GL_COLOR_BUFFER_BIT);
            Window->display();
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(0, View::Width, View::Height, 0, -1, 1);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glEnable(GL_TEXTURE_2D);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            glEnable(GL_VERTEX_ARRAY);
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            Sprite::Init();
        }
        void Loop() {
            glEnable(GL_COLOR_LOGIC_OP);
            glLogicOp(GL_XOR);
            glLoadIdentity();
            glBindTexture(GL_TEXTURE_2D, 0);
            glBegin(GL_LINE_STRIP);
            for (int i = 0; i < View::Width; ++i) {
                glVertex2d(i, View::Height-Time::Deltas[i]*1000);
            }
            glEnd();
            glDisable(GL_COLOR_LOGIC_OP);
            Window->setTitle(string("class NoLifeStory {int fps = "+to_string((int)Time::FPS)+";};"));
            sf::Event e;
            while (Window->pollEvent(e)) {
                switch (e.type) {
                case sf::Event::Closed:
                    Game::Over = true;
                    break;
                case sf::Event::GainedFocus:
                    //unmute
                    break;
                case sf::Event::LostFocus:
                    //Mute
                    break;
                default:
                    break;
                }
            }
            Window->display();
            glColor4f(1, 1, 1, 1);
            glClear(GL_COLOR_BUFFER_BIT);
            View::Update();
        }
    }
}