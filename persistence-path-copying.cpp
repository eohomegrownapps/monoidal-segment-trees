#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

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
    int time = 0;

    SegmentTree(int _s, int _e, T initv(int) = [](int x){return M::id;}, bool nocreate = false){
        s = _s;
        e = _e;
        m = (s+e)/2;
        if (nocreate){
            return;
        }
        if (s==e){
            v = initv(s);
            return;
        }
        l = new SegmentTree<M>(s, m, initv);
        r = new SegmentTree<M>(m+1, e, initv);
        v = M::append(l->v, r->v);
    }

    SegmentTree<M>* update(int ind, T val, int t){
        SegmentTree<M> *node = new SegmentTree<M>(s,e,NULL,true);
        node->time = t;
        if (s==e){
            node->v = M::append(v,val);
            return node;
        }
        if (ind<=m){
            node->l = l->update(ind,val,t);
            node->r = r;
        } else {
            node->l = l;
            node->r = r->update(ind,val,t);
        }
        node->v = M::append(node->l->v, node->r->v);
        return node;
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
// Solution to https://www.spoj.com/problems/PSEGTREE/

ll add(const ll &a, const ll &b){
    return a+b;
}

template<> const ll Monoid<ll, add>::id(0);

///////////////

int arr[100000];

ll init(int s){
    return arr[s];
}

int main(){
    cin.tie(0);
    ios_base::sync_with_stdio(0);
    int n;
    cin>>n;
    for (int i = 0; i<n; i++){
        cin>>arr[i];
    }
    vector<SegmentTree<Monoid<ll,add>>*> history;
    history.push_back(new SegmentTree<Monoid<ll,add>>(0,n-1,init));
    int q;
    cin>>q;
    for (int i = 0; i<q; i++){
        int a,b,c,d;
        cin>>a>>b>>c>>d;
        if (a==1){
            //take version b
            history.push_back(history[b]->update(c-1,d,history.size()));
        } else {
            cout<<history[b]->query(c-1,d-1)<<'\n';
        }
    }
}