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
    namespace Map {
        array<Layer, 8> Layers;
        vector<Back*> Backgrounds;
        vector<Back*> Foregrounds;
        string CurName;
        Node Next;
        Node Current;
        int Counter = 0;
        void Load() {
            Current = Next;
            Next = Node();
            Sprite::Unload();
            for (Layer& l : Layers) {
                for (Tile* t : l.Tiles) delete t;
                l.Tiles.clear();
                for (Obj* o : l.Objs) delete o;
                l.Objs.clear();
            }
            for (Back* b : Backgrounds) delete b;
            Backgrounds.clear();
            for (Back* b : Foregrounds) delete b;
            Foregrounds.clear();
            for (int i = 0; i < 8; ++i) {
                Layer& l = Layers[i];
                Node ln = Current[i];
                string type = ln["info"]["tS"];
                for (Node tn : ln["tile"]) l.Tiles.push_back(new Tile(tn, type));
                sort(l.Tiles.begin(), l.Tiles.end(), [](Tile* t1, Tile* t2){return t1->z < t2->z;});
                for (Node on : ln["obj"]) l.Objs.push_back(new Obj(on));
                sort(l.Objs.begin(), l.Objs.end(), [](Obj* o1, Obj* o2){return o1->z < o2->z;});
            }
            for (Node bn : Current["back"]) {
                if ((int)bn["front"]) Foregrounds.push_back(new Back(bn));
                else Backgrounds.push_back(new Back(bn));
            }
        }
        vector<string> ToLoad;
        void Init() {
            //Node n1 = Base["Map"]["Map"];
            //for (int i = 0; i < 10; ++i) {
            //    Node n2 = n1["Map"+to_string(i)];
            //    for (Node n3 : n2) {
            //        ToLoad.push_back(n3.Name());
            //    }
            //}
            Load("100000000");
            Load();
        }
        void Loop() {
            //if (ToLoad.empty()) {
            //    Game::Over = true;
            //    return;
            //}
            //string id = ToLoad.back();
            //ToLoad.pop_back();
            //Load(id, "");
            //Counter = ToLoad.size();
            if (Next) Load();
            for (Back* b : Backgrounds) b->Draw();
            for (Layer& l : Layers) {
                for (Obj* o : l.Objs) o->Draw();
                for (Tile* t : l.Tiles) t->Draw();
            }
            for (Back* b : Foregrounds) b->Draw();
        }
        bool Load(string id, string portal) {
            id.insert(0, 9-id.size(), '0');
            string zone = string("Map")+id[0];
            Node n = Base["Map"]["Map"][zone][id];
            if (!n) return false;
            Next = n;
            return true;
        }
    }
}