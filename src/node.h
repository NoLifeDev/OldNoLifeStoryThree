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

namespace NLS {
    class Node {
    public:
        class Data;
        //Public functions
        Node();
        Node(const Node&);
        Node operator= (const Node&);
        Node operator[] (string) const;
        Node operator[] (const char*) const;
        Node operator[] (char*) const;
        Node operator[] (int) const;
        Node operator[] (const Node&) const;
        string Name() const;
        operator bool() const;
        operator string() const;
        operator double() const;
        operator int() const;
        operator Sprite() const;
        //operator Sound() const;
        Node begin() const;
        Node end() const;
        Node& operator++();
        Node& operator--();
        bool operator==(const Node&) const;
        bool operator!=(const Node&) const;
        Node& operator*();
        Node(Data*);
        Node(Data&);
        void InitTop(string);
        void Assign(const Node&);
        void Set(char*);
        void SetUOL(char*);
        void Set(double);
        void Set(int);
        void Set(class Img*);
        void Set(Sprite);
        void Set(class MP3Property*);
        void Resolve();
        void Reserve(int);
        Node g(char*, int n);
    private:
        Data* data;
    };
}
