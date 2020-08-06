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
    ll s, e, m;
    T v;
    SegmentTree<M> *l = NULL, *r = NULL;

    SegmentTree(ll _s, ll _e){
        s = _s;
        e = _e;
        m = (s+e)/2;
    }

    void updateL(ll ind, T val){
        if (l == NULL){
            l = new SegmentTree<M>(s,m);
        }
        l->update(ind,val);
    }

    void updateR(ll ind, T val){
        if (r == NULL){
            r = new SegmentTree<M>(m+1,e);
        }
        r->update(ind,val);
    }

    T getChild(SegmentTree *t){
        if (t == NULL){return M::id;}
        return t->v;
    }

    T queryChild(SegmentTree *t, ll a, ll b){
        if (t == NULL){return M::id;}
        return t->query(a,b);
    }

    void update(ll ind, T val){
        if (s==e){
            v = val;
            return;
        }
        if (ind<=m){
            updateL(ind,val);
        } else {
            updateR(ind,val);
        }
        v = M::append(getChild(l), getChild(r));
    }

    T query(ll a, ll b){
        if (a>b){
            return M::id;
        }
        if (a==s && b==e){
            return v;
        }
        if (b<=m){
            return queryChild(l,a,b);
        } else if (m<a){
            return queryChild(r,a,b);
        } else {
            return M::append(queryChild(l,a,m),queryChild(r,m+1,b));
        }
    }
};

////////////////////////////////////////////////////////////////
// Solution to https://www.spoj.com/problems/ORDERSET/

ll add(const ll &a, const ll &b){
    return a+b;
}

template<> const ll Monoid<ll, add>::id(0);

///////////////

ll xstart = -1e9-1;
ll xend = 1e9+1;

int main(){
    cin.tie(0);
    ios_base::sync_with_stdio(0);
    ll q;
    cin>>q;
    SegmentTree<Monoid<ll, add>> *root = new SegmentTree<Monoid<ll, add>>(0,xend-xstart);
    for (ll i = 0; i<q; i++){
        char c;
        ll x;
        cin>>c>>x;
        if (c=='I'){
            //insert x llo s
            root->update(x-xstart,1);
        } else if (c=='D'){
            root->update(x-xstart,0);
        } else if (c=='K'){
            //find first index where cnt>=x
            ll l = 0, r = xend-xstart;
            while (l<=r){
                ll mid = (l+r)/2;
                if (root->query(0,mid)>=x){
                    r=mid-1;
                } else {
                    l=mid+1;
                }
            }
            if (l==xend-xstart+1){
                cout<<"invalid\n";
            } else {
                cout<<l+xstart<<'\n';
            }
        } else if (c=='C'){
            cout<<root->query(0,x-xstart-1)<<'\n';
        } else {
            continue;
        }
    }
}