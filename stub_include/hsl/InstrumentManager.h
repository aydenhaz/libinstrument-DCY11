#pragma once
namespace hsl {
class InstrumentManager {
public:
    static bool GetFirstCameraImageFlag();
    static void GetInstrumentScreenSize(unsigned&, unsigned&);
    static void GetInstrumentMapSize(unsigned&, unsigned&, unsigned&, unsigned&);
    static void SetInstrumentScreenSize(unsigned, unsigned);
    static void SetInstrumentMapSize(unsigned, unsigned, unsigned, unsigned);
    static int GetInstrumentViewType();
    static void SetInstrumentViewType(int);
};
}
