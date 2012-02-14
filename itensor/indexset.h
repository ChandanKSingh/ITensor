#ifndef __ITENSOR_INDEXSET_H
#define __ITENSOR_INDEXSET_H
#include "index.h"
#include "permutation.h"

//
// IndexSet
//
class IndexSet
    {
    public:

    IndexSet();

    IndexSet(const Index& i1);

    IndexSet(const Index& i1, const Index& i2);

    template <class Iterable>
    IndexSet(const Iterable& ii, int size, int& alloc_size, int offset = 0);

    int
    r() const { return r_; }

    int
    rn() const { return rn_; }

    const Index&
    index(int j) const { return GET(index_,j); }

    int
    m(int j) const { return GET(index_,j).m(); }

    typedef boost::array<Index,NMAX+1>::const_iterator 
    index_it;

    //Can be used for iteration over Indices in a Foreach loop
    //e.g. Foreach(const Index& I, t.index() ) { ... }
    const std::pair<index_it,index_it> 
    index() const  
        { return std::make_pair(index_.begin()+1,index_.begin()+r_+1); }

    void
    setUniqueReal();

    Real
    uniqueReal() const { return ur_; }

    Index 
    findtype(IndexType t) const;

    bool 
    findtype(IndexType t, Index& I) const;

    int 
    findindex(const Index& I) const;

    int 
    findindexn(const Index& I) const;

    int 
    findindex1(const Index& I) const;

    bool 
    has_common_index(const IndexSet& other) const;
    
    bool 
    hasindex(const Index& I) const;

    bool 
    hasindexn(const Index& I) const;

    bool 
    hasindex1(const Index& I) const;

    bool
    hasAllIndex(const boost::array<Index,NMAX+1>& I, int nind) const;

    bool 
    notin(const Index& I) const { return !hasindex(I); }

    void 
    addindex1(const std::vector<Index>& indices);

    void 
    addindex1(const Index& I);

    //Removes the jth index as found by findindex
    void 
    removeindex1(int j);

    void 
    removeindex1(const Index& I) 
        { removeindex1(findindex1(I)); }

    void 
    mapindex(const Index& i1, const Index& i2);

    void
    getperm(const IndexSet& other, Permutation& P) const;

    //
    // Primelevel Methods
    //

    void 
    noprime(PrimeType p = primeBoth);

    void 
    doprime(PrimeType pt, int inc = 1);

    void 
    primeall() { doprime(primeBoth,1); }

    void 
    primesite() { doprime(primeSite,1); }

    void 
    primelink() { doprime(primeLink,1); }

    void 
    mapprime(int plevold, int plevnew, PrimeType pt = primeBoth);

    void 
    mapprimeind(const Index& I, int plevold, int plevnew, 
                PrimeType pt = primeBoth);

    void 
    primeind(const Index& I, int inc = 1)
        { mapindex(I,primed(I,inc)); }

    void 
    primeind(const Index& I, const Index& J);

    void 
    noprimeind(const Index& I) { mapindex(I,I.deprimed()); }

    //
    // Other Methods
    //

    void
    swap(IndexSet& other);

    void
    read(std::istream& s);

    void
    write(std::ostream& s) const;

    friend std::ostream&
    operator<<(std::ostream& s, const IndexSet& is);

    //////////
    //
    // Data Members
    //

    boost::array<Index,NMAX+1> index_;

    int rn_,
        r_;

    Real ur_;

    //
    /////////

    };

template <class Iterable>
IndexSet::
IndexSet(const Iterable& ii, int size, int& alloc_size, int offset)
    :
    r_(size)
    { 
    sortIndices(ii,size,rn_,alloc_size,index_,offset);
    setUniqueReal();
    }


template<class Iterable>
void
sortIndices(const Iterable& I, int ninds, int& rn_, int& alloc_size, 
            boost::array<Index,NMAX+1>& index_, int offset = 0)
    {
    assert(ninds <= NMAX);

    rn_ = 0;
    alloc_size = 1;

    int r1_ = 0;
    boost::array<const Index*,NMAX+1> index1_;

    for(int n = offset; n < ninds+offset; ++n)
        {
        const Index& i = I[n];
        DO_IF_DEBUG(if(i == Index::Null()) Error("Null Index in sortIndices");)
        if(i.m()==1) 
            { index1_[++r1_] = &i; }
        else         
            { 
            index_[++rn_] = i; 
            alloc_size *= i.m(); 
            }
        }
    for(int l = 1; l <= r1_; ++l) 
        index_[rn_+l] = *(index1_[l]);
    }

#endif
