#include <bits/stdc++.h>
using namespace std;

static inline void trim(string &s){
    size_t a=0; while(a<s.size() && isspace((unsigned char)s[a])) ++a;
    size_t b=s.size(); while(b>a && isspace((unsigned char)s[b-1])) --b;
    s.assign(s.begin()+a, s.begin()+b);
}

static vector<long long> parse_ints_from_line(const string &line){
    vector<long long> nums; long long sign=1, val=0; bool innum=false, neg=false;
    for(size_t i=0;i<line.size();++i){
        char c=line[i];
        if(c=='-' && !innum){ neg=true; innum=true; sign=-1; val=0; continue; }
        if(isdigit((unsigned char)c)){
            if(!innum){ innum=true; sign= neg? -1: 1; neg=false; val=0; }
            val = val*10 + (c - '0');
        } else {
            if(innum){ nums.push_back(sign*val); innum=false; neg=false; sign=1; val=0; }
        }
    }
    if(innum){ nums.push_back(sign*val); }
    return nums;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Read whole stdin into string
    std::ostringstream oss; oss<<cin.rdbuf();
    string s = oss.str();
    if(s.empty()) return 0;

    vector<long long> roots;
    long long cnt = -1;

    bool inBracket=false; bool hadBracket=false;
    long long sign=1, val=0; bool innum=false, neg=false;
    long long sign2=1, val2=0; bool innum2=false, neg2=false;
    for(size_t i=0;i<s.size();++i){
        char c=s[i];
        if(c=='[') { inBracket=true; hadBracket=true; // reset per-context number state
            if(innum2){ cnt = sign2*val2; innum2=false; neg2=false; sign2=1; val2=0; }
            continue; }
        if(c==']') { if(innum){ roots.push_back(sign*val); innum=false; neg=false; sign=1; val=0; }
            inBracket=false; continue; }
        if(inBracket){
            if(c=='-' && !innum){ neg=true; innum=true; sign=-1; val=0; continue; }
            if(isdigit((unsigned char)c)){
                if(!innum){ innum=true; sign= neg? -1: 1; neg=false; val=0; }
                val = val*10 + (c - '0');
            } else {
                if(innum){ roots.push_back(sign*val); innum=false; neg=false; sign=1; val=0; }
            }
        }else{
            if(c=='-' && !innum2){ neg2=true; innum2=true; sign2=-1; val2=0; continue; }
            if(isdigit((unsigned char)c)){
                if(!innum2){ innum2=true; sign2= neg2? -1: 1; neg2=false; val2=0; }
                val2 = val2*10 + (c - '0');
            } else {
                if(innum2){ cnt = sign2*val2; innum2=false; neg2=false; sign2=1; val2=0; }
            }
        }
    }
    if(innum){ roots.push_back(sign*val); }
    if(innum2){ cnt = sign2*val2; }

    // Fallbacks if no bracket or failed to separate
    if(roots.empty()){
        // Try line-based: first non-empty line = roots, second = cnt
        vector<string> lines; {
            string tmp; string cur; for(char c: s){ if(c=='\n' || c=='\r'){ if(!cur.empty()) lines.push_back(cur); cur.clear(); } else cur.push_back(c);} if(!cur.empty()) lines.push_back(cur);
        }
        if(lines.size()>=2){
            vector<long long> a = parse_ints_from_line(lines[0]);
            vector<long long> b = parse_ints_from_line(lines[1]);
            if(!a.empty() && !b.empty()){ roots = a; cnt = b.back(); }
        }
        if(roots.empty()){
            // As last resort, split all numbers: last is cnt, others roots
            vector<long long> all = parse_ints_from_line(s);
            if(all.size()>=2){ cnt = all.back(); all.pop_back(); roots = all; }
        }
    }else{
        // If we got roots from brackets but no cnt parsed, try to parse last number outside brackets failed -> try alternative
        if(cnt == -1){
            vector<long long> all = parse_ints_from_line(s);
            if(all.size() == roots.size()+1){ cnt = all.back(); }
        }
    }

    if(roots.empty() || cnt < 1 || (size_t)cnt > roots.size()){
        // Nothing to do; output nothing
        return 0;
    }

    size_t n = roots.size();
    size_t idx = n - (size_t)cnt; // 0-indexed position for kth largest in ascending order
    nth_element(roots.begin(), roots.begin()+idx, roots.end());
    long long ans = roots[idx];
    cout << ans << '\n';
    return 0;
}
