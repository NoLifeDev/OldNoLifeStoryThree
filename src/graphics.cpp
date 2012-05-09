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
            Window = new sf::Window(sf::VideoMode(800, 600), "NoLifeStory", sf::Style::Close, sf::ContextSettings(0, 0, 0, 2, 1));
            Window->setKeyRepeatEnabled(true);
            Window->setMouseCursorVisible(false);
            Window->setVerticalSyncEnabled(false);
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
            glOrtho(0, 800, 600, 0, -1, 1);
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
            {
                static mt19937 engine;
                uniform_real_distribution<float> dist;
                double d = Time::Total*2.08;
                double df = floor(d);
                double dif = d-df;
                dif = 6*pow(dif, 7)-7*pow(dif, 6)+1;
                engine.seed(df);
                float r = dist(engine)*(dif);
                float g = dist(engine)*(dif);
                float b = dist(engine)*(dif);
                engine.seed(df+1);
                r += dist(engine)*(1-dif);
                g += dist(engine)*(1-dif);
                b += dist(engine)*(1-dif);
                glClearColor(r, g, b, 1);
            }
            glClear(GL_COLOR_BUFFER_BIT);
            glColor4f(1, 1, 1, 1);
            Sprite s = Sprite::All.back();
            Sprite::All.pop_back();
            s.Draw(400, 300);
            if (Sprite::All.empty()) Game::Over = true;
            Window->display();
        }
    }
}