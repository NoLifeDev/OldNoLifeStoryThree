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
        bool Online;
        sf::TcpSocket Socket;
        uint16_t Version;
        string Patch;
        uint8_t Locale;
        char SendIV[4];
        char RecvIV[4];
        char rbuf[0x10000];
        uint16_t rbufr;
        uint16_t rbufs;
        uint16_t rbufp;
        char wbuf[0x10000];
        uint16_t wbufp = 4;
        bool header;
        bool handshake;
        bool login;
        enum class OpCode : uint16_t{
            HandShake = 0x14,
            Login = 0x15,
            Ping = 0x2E,
            LoginScreen = 0x38
        };
        template <typename T>
        T Read() {
            T& v = *(T*)&rbuf[rbufp];
            rbufp += sizeof(T);
            return v;
        }
        template <>
        string Read<string>() {
            uint16_t size = Read<uint16_t>();
            string s((char*)&rbuf[rbufp], size);
            rbufp += size;
            return s;
        }
        template <typename T>
        void Write(T v) {
            memcpy(&wbuf[wbufp], &v, sizeof(v));
            wbufp += sizeof(T);
        }
        template <>
        void Write<string>(string v) {
            Write<uint16_t>(v.size());
            memcpy(&wbuf[wbufp], v.data(), v.size());
            wbufp += v.size();
        }
        template <>
        void Write<OpCode>(OpCode op) {
            Write((uint16_t)op);
            cout << "Sending packet: " << (uint16_t)op << endl;
        }
        void ConnectLogin() {
            Socket.setBlocking(true);
            sf::Socket::Status err = Socket.connect("83.80.148.175", 8484, sf::seconds(1));
            if (err != sf::Socket::Status::Done) {
                Online = false;
                cout << "Failed to connect to login server" << endl;
                return;
            }
            cout << "Connected to login server" << endl;
            Socket.setBlocking(false);
            Online = true;
            handshake = true;
            header = true;
            login = true;
            rbufs = 2;
            rbufr = 0;
            rbufp = 0;
        }
        void Init() {
            ConnectLogin();
        }
        namespace Send {
            void Send() {
                uint16_t a = (Network::SendIV[3]<<8)+Network::SendIV[2];
                a ^= WZ::Version;
                uint16_t b = a^(wbufp-4);
                wbuf[0] = a&0xFF00;
                wbuf[1] = a>>8;
                wbuf[2] = b&0x00FF;
                wbuf[3] = b>>8;
                Encrypt(wbuf+4, wbufp-4);
                Socket.send(wbuf, wbufp);
                wbufp = 4;
            }
            void HandShake() {
                Write(OpCode::HandShake);
                Write(Locale);
                Write(Version);
                Write((uint16_t)stoi(Patch));
                Send();
            }
            void Ping() {
                Write(OpCode::Ping);
                Send();
            }
            void LoginScreen() {
                Write(OpCode::LoginScreen);
                Send();
            }
            void Login(string user, string pass) {
                Write(OpCode::Login);
                Write(user);
                Write(pass);
                Send();
            }
        }
        namespace Receive {
            void Ping() {
                Send::Ping();
                cout << "Ping!" << endl;
            }
        }
        void Loop() {
            if (!Online) return;
            size_t r;
            sf::Socket::Status err = Socket.receive(rbuf+rbufr, rbufs-rbufr, r);
            rbufr += r;
            if (err != sf::Socket::Status::Done && err != sf::Socket::Status::NotReady) {
                Online = false;
                cout << "Disconnected" << endl;
                return;
            }
            if (rbufr < rbufs) return;
            if (header) {
                uint16_t size;
                if (handshake) {
                    size = Read<uint16_t>();
                } else {
                    uint32_t b = Read<uint32_t>();
                    size = (b>>16)^b;
                }
                cout << "Packet size: " << size << endl;
                rbufs = size;
                rbufr = 0;
                rbufp = 0;
                header = false;
                return;
            }
            if (handshake) {
                cout << "Received handshake" << endl;
                Version = Read<uint16_t>();
                cout << "Version: " << Version << endl;
                Patch = Read<string>();
                cout << "Patch: " << Patch << endl;
                uint32_t siv = Read<uint32_t>();
                cout << "SendIV: " << siv << endl;
				uint32_t riv = Read<uint32_t>();
                cout << "RecvIV: " << riv << endl;
                Locale = Read<uint8_t>();
                cout << "Locale: " << (int)Locale << endl;
                memcpy(SendIV, &siv, 4);
				memcpy(RecvIV, &riv, 4);
                handshake = false;
                Send::HandShake();
                Send::LoginScreen();
                Send::Login("Testme", "testme");
            } else {
                Decrypt(rbuf, rbufs);
                uint16_t opcode = Read<uint16_t>();
                cout << "Received packet: " << opcode << endl;
                switch (opcode) {
                case 0x11: Receive::Ping; break;
                }
            }
            rbufs = 4;
            rbufr = 0;
            rbufp = 0;
            header = true;
        }
    }
}