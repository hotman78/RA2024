
struct Visualizer{
    int n;
    map<pair<int,int>, int>id;
    vector<pair<int,int>>edges;
    vector<string>color,style,penwidth;
    Visualizer(int n):n(n){
    }
    void add_edge(int s,int t){
        id[{s,t}]=edges.size();
        edges.emplace_back(s, t);
        color.emplace_back("black");
        style.emplace_back("dotted");
        penwidth.emplace_back("1.5");
    }
    string node_name(int i){
        return "E"+to_string(i);
    }
    void reset_color(){
        color=vector<string>(color.size(),"black");
    }
    void reset_style(){
        style=vector<string>(style.size(),"dotted");
        // penwidth=vector<string>(penwidth.size(),"1.5");
    }
    void change_color(int s,int t,string c){
        color[id[{s,t}]]=c;
    }
    void change_style(int s,int t,string sty){
        style[id[{s,t}]]=sty;
    }
    void change_penwidth(int s,int t,string val){
        penwidth[id[{s,t}]]=val;
    }
    string vis(){
        // cerr<<color.size()<<" "<<n<<" "<<edges.size()<<endl;
        string res="";
        res+="digraph G {";
        for(int i=0;i<n;++i){
            res+=node_name(i)+ "[label = "+to_string(i)+", shape = circle];";
        }
        int idx=0;
        for(auto [s, t]: edges){
            res+=node_name(s) +"->"+ node_name(t)+ "[color=\""+color[idx]+"\",style="+style[idx]+", penwidth="+penwidth[idx]+"];";
            ++idx;
        }
        res+="}";
        // cerr<<edges.size()<<endl;
        return res;
    }
    int idx=0;
    void out(){
        string num=to_string(idx++);
        while(num.size()<4)num.insert(num.begin(),'0');
        string path = "animation/out_"+num+".dot";
        // " | dot -Tsvg>"+path
        // cerr<<"/bin/echo -e \""+vis()+"\""<<endl;
        ofstream ofs(path);
        if (!ofs) {
            std::cout << "ファイルが開けませんでした" << std::endl;
            return;
        }
        // cerr<<vis();
        ofs<<vis();
        ofs.close();
    }
};