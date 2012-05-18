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
    class Node::Data {
    public:
        union vt {
            double dreal;
            int ireal;
            char* string;
            Img* img;
			Sprite png;
            Sound mp3;
        } value;
        Data* parent;
        char* name;
        Data* children;
        uint16_t num;
        enum Type : uint16_t {
            none,
            dreal,
            ireal,
            string,
            uol,
            png,
            mp3,
            img
        } type;
    private:
        Data(const Node::Data&);
    };
    Node::Node() : data(nullptr) {}
    Node::Node(Data* data) : data(data) {}
    Node::Node(Data& data) : data(&data) {}
    Node::Node(const Node& other) : data(other.data) {}
    Node Node::operator= (const Node& other) {
        return data = other.data;
    }
    Node Node::operator[] (const char* key) const {
        if (!data) return Node();
        if (strcmp(key, "..") == 0) return data->parent;
        if (strcmp(key, ".") == 0) return *this;
        if (data->type == Data::img) {
            data->type = Data::none;
            data->value.img->Parse();
        }
        if (!data->children) return Node();
        for (int i = 0; i < data->num; ++i) {
            if (strcmp(data->children[i].name, key) == 0) return data->children[i];
        }
        return Node();
    }
    Node Node::operator[] (string key) const {
        return operator[](key.c_str());
    }
    Node Node::operator[] (char* key) const {
        return operator[]((const char*)key);
    }
    Node Node::operator[] (int key) const {
        return operator[](to_string((long long)key).c_str());
    }
    Node Node::operator[] (const Node& key) const {
        return operator[]((string)key);
    }
    Node Node::g(char* key, int n) {
        data->children[n].name = key;
        data->children[n].parent = this->data;
        return data->children[n];
    }
    string Node::Name() const {
        if (!data) return string();
        return data->name;
    }
    void Node::InitTop(string s) {
        data = (Data*)calloc(1, sizeof(Data));
        data->name = AllocString(s.length()+1);
        strcpy(data->name, s.data());
        data->parent = this->data;
    }
    void Node::Assign(const Node& other) {
        data = other.data;
    }
    Node::operator bool() const {
        return !!data;
    }
    Node::operator string() const {
        if (!data) return string();
        switch (data->type) {
        case Data::ireal:
            return to_string((long long)data->value.ireal);
        case Data::dreal:
            return to_string((long double)data->value.dreal);
        case Data::string:
            return data->value.string; 
        default:
            return string();
        }
    }
    Node::operator double() const {
        if (!data) return 0;
        if (data->type == Data::ireal) return data->value.ireal;
        else if (data->type == Data::dreal) return data->value.dreal;
        else return 0;
    }
    Node::operator int() const {
        if (!data) return 0;
        if (data->type == Data::ireal) return data->value.ireal;
        else if (data->type == Data::dreal) return (int)data->value.dreal;
        else return 0;
    }
    Node::operator Sprite() const {
        if (!data) return Sprite::Blank();
        if (data->type == Data::png) return data->value.png;
        else return Sprite::Blank();
    }
    Node::operator Sound() const {
        if (!data) return Sound::Blank();
        if (data->type == Data::mp3) return data->value.mp3;
        else return Sound::Blank();
    }
    void Node::Set(char* v) {
        if (!v) v = AllocString(1);
        data->value.string = v;
        data->type = Data::string;
    }
    void Node::SetUOL(char* v) {
        data->value.string = v;
        data->type = Data::uol;
    }
    void Node::Set(double v) {
        data->value.dreal = v;
        data->type = Data::dreal;
    }
    void Node::Set(int v) {
        data->value.ireal = v;
        data->type = Data::ireal;
    }
    void Node::Set(Sprite v) {
		data->value.png = v;
		data->type = Data::png;
    }
    void Node::Set(Sound v) {
        data->value.mp3 = v;
        data->type = Data::mp3;
    }
    void Node::Set(Img* img) {
        data->value.img = img;
        data->type = Data::img;
    }
    void Node::Resolve() {
        if (!data) return;
        if (data->type == Data::uol) {
            char* s = data->value.string;
            static char* parts[10];
            int n = 1;
            char* it = s;
            parts[0] = s;
            while (*it != '\0') {
                if (*it == '/') {
                    *it = '\0';
                    parts[n] = it+1;
                    ++n;
                }
                ++it;
            }
            Node nn = data->parent;
            for (int i = 0; i < n; ++i) {
                if (!nn) break;
                nn = nn[parts[i]];
            }
            if (nn) *data = *nn.data;
        } else {
			for (Node n = begin(); n != end(); ++n) {
                Node nn = n;
                nn.Resolve();
            }
        }
    }
    void Node::Reserve(int n) {
        static Data* d = nullptr;
        static int remain = 0;
        if (remain < n) {
            d = (Data*)calloc(0x10000, sizeof(Data));
            remain = 0x10000;
        }
        data->children = d;
        remain -= n;
        d += n;
        data->num = n;
    }
    Node Node::begin() const {
        if (!data) return nullptr;
        if (data->type == Data::img) {
            data->type = Data::none;
            data->value.img->Parse();
        }
        return data->children;
    }
    Node Node::end() const {
        if (!data) return nullptr;
        if (data->type == Data::img) {
            data->type = Data::none;
            data->value.img->Parse();
        }
        return data->children + data->num;
    }
    Node& Node::operator++() {
        ++data;
        return *this;
    }
    Node& Node::operator--() {
        --data;
        return *this;
    }
    bool Node::operator==(const Node& other) const {
        return data == other.data;
    }
    bool Node::operator!=(const Node& other) const {
        return data != other.data;
    }
    Node& Node::operator*() {
        return *this;
    }
}