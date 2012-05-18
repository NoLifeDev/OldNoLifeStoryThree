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
    path Path;
    Node Base;
    vector<path> Paths;
    class File {
    public:
        File(Node n);
    private:
        void Directory(Node n);
        uint32_t fileStart;
        MapFile file;
        bool big;
    };
    File::File(Node n) {
        big = false;
        string filename = Path/path(n.Name()+".wz");
        file.Open(filename);
        file.Map(0, 0x100);
        if (strcmp(file.ReadString(4), "PKG1") != 0) die();
        uint64_t fileSize = file.Read<uint64_t>();
        fileStart = file.Read<uint32_t>();
        file.ReadString();
        if (!WZ::Version) {
            EncVersion = file.Read<int16_t>();
            int32_t count = file.ReadCInt();
            uint32_t c = 0;
            for (int k = 0; k < count; ++k) {
                uint8_t type = file.Read<uint8_t>();
                if (type == 3) {
                    file.ReadEncString();
                    file.ReadCInt();
                    file.ReadCInt();
                    file.Read<uint32_t>();
                    continue;
                } else if (type == 4) {
                    file.ReadEncString();
                    file.ReadCInt();
                    file.ReadCInt();
                    c = file.Tell();
                    break;
                } else die();
            }
            if (c == 0) die();
            bool success = false;
            for (WZ::Version = 0; WZ::Version < 512 && !success; ++WZ::Version) {
                string s = to_string((long long)WZ::Version);
                size_t l = s.length();
                VersionHash = 0;
                for (size_t i = 0; i < l; ++i) VersionHash = 32*VersionHash+s[i]+1;
                uint32_t result = 0xFF^(VersionHash>>24)^(VersionHash<<8>>24)^(VersionHash<<16>>24)^(VersionHash<<24>>24);
                if (result == EncVersion) {
                    file.Map(0, 0x100);
                    file.Seek(c);
                    uint32_t off = file.ReadOffset(fileStart);
                    if (off > fileSize) continue;
                    file.Map(off, 0x100);
                    uint8_t a = file.Read<uint8_t>();
                    if(a != 0x73) continue;
                    for (uint8_t j = 0; j < 3 && !success; ++j) {
                        Key = Keys[j];
                        AKey = AKeys[j];
                        WKey = WKeys[j];
                        string ss = file.ReadEncString();
                        if (ss == "Property") success = true;
                        else file.Seek(1);
                    }
                    break;
                }
            }
            if (!success) die();
            file.Map(0, 0x100);
            file.Seek(fileStart+2);
        } else {
            int16_t eversion = file.Read<int16_t>();
            if (eversion != EncVersion) die();
        }
        if (file.ReadCInt() == 0) die();
        file.Seek(fileStart+2);
        Directory(n);
        file.Unmap();
        delete this;
    }
    void File::Directory(Node n) {
        int32_t count = file.ReadCInt();
        if (count == 0) {
            new File(n);
            return;
        }
        vector<Node> dirs;
        n.Reserve(count);
        for (int i = 0; i < count; ++i) {
            char* name;
            uint8_t type = file.Read<uint8_t>();
            if (type == 1) die();
            else if (type == 2) {
                int32_t s = file.Read<int32_t>();
                uint32_t p = file.Tell();
                file.Seek(fileStart+s);
                type = file.Read<uint8_t>();
                name = file.ReadEncString();
                file.Seek(p);
            } else if (type == 3) name = file.ReadEncString();
            else if (type == 4) name = file.ReadEncString();
            else die();
            uint32_t fsize = file.ReadCInt();
            uint32_t checksum = file.ReadCInt();
            uint32_t off = file.ReadOffset(fileStart);
            if (type == 3) dirs.emplace_back(n.g(name, i));
            else if (type == 4) {
                size_t len = strlen(name);
                name[len-4] = '\0';
                if (!big) {
                    big = true;
                    uint32_t p = file.Tell();
                    file.Map(0, off);
                    file.Seek(p);
                }
                new Img(file, n.g(name, i), fsize, off);
            } else die();
        }
        for (Node n : dirs) Directory(n);
    }
    void Load(string name) {
        Base.InitTop(name);
        new File(Base);
    }
    namespace WZ {
        uint16_t Version;
        void Init() {
            GenKeys();
            for (path p : Paths) {
                Path = p;
                if (exists(Path/path("Data.wz"))) {
                    Load("Data");
                    return;
                }
                if (exists(Path/path("Base.wz"))) {
                    Load("Base");
                    return;
                }
            }
            die();
        }
        void AddPath(string path) {
            Paths.push_back(path);
        }
    }
}