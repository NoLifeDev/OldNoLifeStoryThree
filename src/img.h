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
    class MP3Property {
    public:
        MP3Property(MapFile& file, Node n, uint32_t off);
        void Play(bool loop);
        void Stop();
    private:
        int32_t len;
        void* data;
        uint32_t offset;
        MapFile file;
        HSTREAM stream;
    };
    class Img {
    public:
        static vector<Img*> All;
        static bool Lazy;
        Img(MapFile file, Node n, uint32_t size, uint32_t offset) : file(file), n(n), size(size), offset(offset) {
            if (!Lazy) All.push_back(this);
            else n.Set(this);
        }
        void Parse();
    private:
        Node n;
        MapFile file;
        uint32_t size;
        uint32_t offset;
        void ExtendedProperty(Node n);
        void SubProperty(Node n);
    };
}