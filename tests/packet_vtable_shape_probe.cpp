// Freestanding 32-bit Itanium C++ ABI model, never linked into the Android SO.
// Names and the opaque bytes are probes, not recovered GFrame declarations.
struct Async {
    virtual ~Async() {}
    virtual int executor() { return 0; }
    virtual int uiThread() { return 0; }
    char state[0x3c];
};
struct ObjMem { virtual ~ObjMem() {} };
struct Pkg : Async, ObjMem {
    virtual int supports() { return 0; }
    virtual int active() { return 1; }
    virtual void initEnd() {}
    virtual void slot7() {}
    virtual void slot8() {}
    virtual void destroy() {}
    virtual void preload() {}
    virtual void slot11() {}
    virtual void slot12() {}
    virtual void slot13() {}
    virtual void postLoad() {}
    virtual void postAlready() {}
    char hostState[0x7c];
};
struct FragmentPkg : Pkg {};
struct MapObs {
    virtual void mapStatus() {}
    virtual ~MapObs() {}
};
struct BizObs {
    virtual ~BizObs() {}
    virtual bool onBusiness() { return true; }
};
struct Cover : MapObs, BizObs {
    bool onBusiness() override { return true; }
    char coverState[0xa0];
};
struct Packet : FragmentPkg, Cover {
    ~Packet() override {}
    void initEnd() override {}
    void destroy() override {}
    void preload() override {}
    void postLoad() override {}
    void postAlready() override {}
    char proxy[4];
};
static_assert(sizeof(Async) == 0x40, "host async prefix");
static_assert(sizeof(ObjMem) == 4, "host secondary base");
static_assert(sizeof(Pkg) == 0xc0, "host package extent");
static_assert(sizeof(Cover) == 0xa8, "cover extent");
static_assert(sizeof(Packet) == 0x16c, "Packet extent");

Packet packetShapeProbe;
