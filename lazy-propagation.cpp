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

/*
requirements for lazy propagation segment tree

- M is monoid (associative)
- L is monoid over the same type as M
- L is distributive over M

e.g. M is max, L is addition

max(a+c,b+c) = max(a,b)+c
M(L(a,c),L(b,c)) = L(M(a,b),c)

Lazy values applied to x as follows:
L(L(x,a),b)
*/

template<typename M, typename L>
struct SegmentTree {
    static_assert(is_same<typename M::type,typename L::type>::value, "M and L must have same type");
    using T = typename M::type;
    int s, e, m;
    T v;
    T lazyv = L::id;
    SegmentTree<M,L> *l, *r;

    SegmentTree(int _s, int _e, T initv(int) = [](int x){return M::id;}){
        s = _s;
        e = _e;
        if (s==e){
            v = initv(s);
            return;
        }
        m = (s+e)/2;
        l = new SegmentTree<M,L>(s, m, initv);
        r = new SegmentTree<M,L>(m+1, e, initv);
        v = M::append(l->v, r->v);
    }

    T value(){
        // update current value, propagate lazy value to children
        // call whenever you access children
        v = L::append(v, lazyv);
        if (s!=e){
            l->lazyv = L::append(l->lazyv, lazyv);
            r->lazyv = L::append(r->lazyv, lazyv);
        }
        lazyv = L::id;
        return v;
    }

    void update(int a, int b, T val){
        if (s==a&&e==b){
            lazyv = L::append(lazyv,val);
            return;
        }
        value();
        if (b<=m){
            l->update(a,b,val);
        } else if (m<a){
            r->update(a,b,val);
        } else {
            l->update(a,m,val);
            r->update(m+1,b,val);
        }
        v = M::append(l->value(), r->value());
    }

    T query(int a, int b){
        if (a>b){
            return M::id;
        }
        if (a==s && b==e){
            return value();
        }
        value();
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
// Solution to https://dunjudge.me/analysis/problems/1444/

ll _max(const ll &a, const ll &b){
    return max(a,b);
}

template<> const ll Monoid<ll, _max>::id(0);

ll add(const ll &a, const ll &b){
    return a+b;
}

template<> const ll Monoid<ll, add>::id(0);

///////////////

ll arr[100000];

ll initv(int x){
    return arr[x];
}

int main(){
    cin.tie(0);
    ios_base::sync_with_stdio(0);
    int n;
    cin>>n;
    for (int i = 0; i<n; i++){
        cin>>arr[i];
    }
    int m;
    cin>>m;
    SegmentTree<Monoid<ll,_max>,Monoid<ll,add>> *root = new SegmentTree<Monoid<ll,_max>,Monoid<ll,add>>(0,n-1,initv);
    for (int i = 0; i<m; i++){
        int t;
        cin>>t;
        if (t==0){
            int x,y;
            cin>>x>>y;
            cout<<root->query(x,y)<<'\n';
        } else {
            int a,b;
            ll c;
            cin>>a>>b>>c;
            root->update(a,b,c);
        }
    }
}