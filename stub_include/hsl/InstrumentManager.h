#pragma once
namespace hsl {
enum InstrumentType : int {};
class InstrumentManager {
public:
    static bool GetFirstCameraImageFlag();
    static void GetInstrumentScreenSize(unsigned&, unsigned&);
    static void GetInstrumentMapSize(unsigned&, unsigned&, unsigned&, unsigned&);
    static void SetInstrumentScreenSize(unsigned, unsigned);
    static void SetInstrumentMapSize(unsigned, unsigned, unsigned, unsigned);
    static int GetInstrumentViewType();
    static void SetInstrumentViewType(InstrumentType);
    static void SetInstrumentViewType(int value) { SetInstrumentViewType(static_cast<InstrumentType>(value)); }
};
}
