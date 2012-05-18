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

namespace NLS {
    class MapFile {
    public:
        void Open(string filename);
        void Map(uint32_t base, uint32_t len);
        MapFile() : off(0), data(0), delta(0), d(0) {}
        MapFile(const MapFile& other) : off(0), data(0), delta(0), d(other.d) {}
        void operator=(const MapFile& other);
        void Unmap();
        uint32_t Tell();
        void Seek(uint32_t);
        void Skip(uint32_t);
        template <class T> T Read() {
            return *(T*)ReadBin(sizeof(T));
        }
        int32_t ReadCInt();
        char* ReadEncString();
        char* ReadString();
        char* ReadString(int32_t len);
        char* ReadTypeString();
        wchar_t* ReadWString(int32_t len);
        uint32_t ReadOffset(uint32_t fileStart);
        void* ReadBin(uint32_t size);
    private:
        struct Data;
        Data* d;
        uint32_t off;
        char* data;
        uint32_t delta;
    };
    char* to_cstring(int);
    char* AllocString(size_t len);
}