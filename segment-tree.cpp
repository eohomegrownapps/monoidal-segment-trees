#include <bits/stdc++.h>
using namespace std;

// Monoid struct taken from https://gist.github.com/evincarofautumn/2b5f004ca81e33c62ff0
template<typename T, T append_(T const&, T const&)>
struct Monoid{
    //a semigroup with an identity element ('id')
    typedef T type;
    static T append(T const& a, T const& b){
        return append_(a, b);
    }
    static const T id;
};

template<typename M>
struct SegmentTree {
    using T = typename M::type;
    int s, e, m;
    T v;
    SegmentTree<M> *l, *r;

    SegmentTree(int _s, int _e, T initv(int) = [](int x){return M::id;}){
        s = _s;
        e = _e;
        if (s==e){
            v = initv(s);
            return;
        }
        m = (s+e)/2;
        l = new SegmentTree<M>(s, m, initv);
        r = new SegmentTree<M>(m+1, e, initv);
        v = M::append(l->v, r->v);
    }

    void update(int ind, T val){
        if (s==e){
            v = val;
            return;
        }
        if (ind<=m){
            l->update(ind,val);
        } else {
            r->update(ind,val);
        }
        v = M::append(l->v, r->v);
    }

    T query(int a, int b){
        if (a>b){
            return M::id;
        }
        if (a==s && b==e){
            return v;
        }
        if (b<=m){
            return l->query(a,b);
        } else if (m<a){
            return r->query(a,b);
        } else {
            return M::append(l->query(a,m),r->query(m+1,b));
        }
    }
};

////////////////////////////////////////////////////////////////
// Solution to https://www.spoj.com/problems/GSS5/

struct Data{
    int tot = 0;
    int maxl = 0;
    int maxr = 0;
    int maxtot = 0;
};

Data combine(const Data &a, const Data &b){
    Data ans;
    ans.tot = a.tot + b.tot;
    ans.maxl = max(a.maxl, a.tot+b.maxl);
    ans.maxr = max(b.maxr, a.maxr+b.tot);
    ans.maxtot = max(max(a.maxtot,b.maxtot),a.maxr+b.maxl);
    return ans;
}

constexpr Data zero;
template<> const Data Monoid<Data, combine>::id(zero);

///////////////

int arr[100000];

Data init(int s){
    Data v;
    v.tot=arr[s];
    v.maxl=arr[s];
    v.maxr=arr[s];
    v.maxtot=arr[s];
    return v;
}

int main(){
    cin.tie(0);
    ios_base::sync_with_stdio(0);
    int t;
    cin>>t;
    for (int tc = 0; tc<t; tc++){
        int n;
        cin>>n;
        for (int i = 0; i<n; i++){
            cin>>arr[i];
        }
        SegmentTree<Monoid<Data, combine>> *root = new SegmentTree<Monoid<Data, combine>>(0,n-1,init);
        int m;
        cin>>m;
        for (int i = 0; i<m; i++){
            int a,b,c,d;
            cin>>a>>b>>c>>d;
            a--;b--;c--;d--;
            b = min(b,d);
            c = max(c,a);
            int ans = -1e9;
            if (b<c){
                ans = root->query(a,b).maxr+root->query(b+1,c-1).tot+root->query(c,d).maxl;
            } else {
                ans = max(ans, root->query(a,c-1).maxr+root->query(c,d).maxl);
                ans = max(ans, root->query(a,b).maxr+root->query(b+1,d).maxl);
                ans = max(ans, root->query(c,b).maxtot);
            }
            cout<<ans<<'\n';
        }
    }
}