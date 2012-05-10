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
    namespace Network {
        sf::TcpSocket LoginSocket;
        void Init() {
            LoginSocket.setBlocking(true);
            //Diamonds 83.80.148.175
            //Rice 96.11.243.156
            sf::Socket::Status err = LoginSocket.connect("96.11.243.156", 8484, sf::seconds(1));
            switch (err) {
            case sf::Socket::Status::Done:
                cout << "Connected" << endl;
                break;
            case sf::Socket::Status::Disconnected:
                cout << "Disconnected" << endl;
                break;
            case sf::Socket::Status::Error:
                cout << "Error" << endl;
                break;
            case sf::Socket::Status::NotReady:
                cout << "Not Ready" << endl;
                break;
            default:
                cout << "Wat" << endl;
                break;
            }
        }
    }
}