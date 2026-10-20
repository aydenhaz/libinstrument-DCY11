# 交接文档 — libInstrumentPannelLarge.so 逆向重建工程

日期：2026-10-20
状态：**接口重建完成，符号闭包 441/441，源码通过语法校验，待 ARM 链接与真机验收**

---

## 1. 项目概述

目标：`libInstrumentPannelLarge.so`（车载多屏 HMI「大尺寸仪表」插件，32 位 ARM 原生库）。
任务：反编译并生成**出入接口一致**的源代码，可编译为同名 `.so` 替代原库。

原库指纹：

| 项 | 值 |
|---|---|
| 格式 | ELF32 / ARM / EABI5 / **soft-float (armeabi)** / Thumb / stripped / BIND_NOW |
| 大小 | 452,284 字节 |
| 导出代码符号 | 仅 3 个：`StartupInstrumentPannelLarge`@0x312c5、`ExitInstrumentPannelLarge`@0x313c9、`LogInit`@0x31471（另 1 个弱导出 `BroadcastExtScreen::SwitchRouteMessage::getClassType()` 及一批 RTTI 数据符号）|
| 导入符号 | 441 个（宿主库 399 + NDK 运行时 42）|
| 依赖库 | 9 个宿主 .so + libm/libdl/libc/libc++_shared |

## 2. 三个入口的精确行为（反汇编验证）

### 2.1 `int StartupInstrumentPannelLarge(void *ctx)` @0x312c5

```
1. ALC 自注册：
   void* alc = I_GetAlcPtr();            // 为空则跳过整段
   alc = I_GetAlcPtr();                  // 原实现调用两次
   fn = **(void***)alc;
   block = {0,0,"HMI_PANNELLARGE",0,25,"bool StartupInstrumentPannelLarge(void *)","StartupInstrumentPannelLarge"};
   fn(alc, 8, 0, &block);                // BLX r3 @0x3131a
2. hsl::MultiScreenService* svc = GetInstance();
   if (svc && !svc->IsCreatedLargeScreen()) svc->SetCreatedLargeScreen(true);
3. CGPackageMgr* pm = CGFrameSimulation::getPackageMgr();
   CGString pkg(u"InstrumentPannelLargePackage");
   CGIntent it;
   it.putStringExtra(u"fromPackage", "instrumentPannelLargePackage");
   it.putStringExtra(u"packageType", "com.autonavi.package.GIPLPacket");
   pm->loadPackage(pkg, it, false, false);
4. return 1;                            // MOVS r0,#1
```

### 2.2 `void ExitInstrumentPannelLarge(void *ctx)` @0x313c9

```
1. 同构 ALC 反注册（tag=56，经 lr 间接调用 @0x3141c）
2. CGFrameSimulation::getPackageMgr()->unloadPackage(CGString(u"InstrumentPannelLargePackage"), false);
```

### 2.3 `void LogInit(void)` @0x31471

原函数体为单条 `BX lr` —— 空实现。

### 2.4 辅助事实

- 字符串全部为 PC 相对增量寻址（`LDR rN,[pc,#x]` 取值后再 `add rN,pc`，addend=指令地址+4），字面量已逐字节核对（`.rodata` 偏移：包名 @0x5caae UTF-16、键值 @0x54e49/0x54e66 ASCII、ALC 名 @0x54e02/0x54e2c）。
- init_array 4 项 / fini_array 2 项：注册 `CGInstrumentPannelLargeCruiseFragment` 等 Frame（ClassNode_CGPackage + __cxa_atexit）。

## 3. 依赖分层（实证）

```
libInstrumentPannelLarge.so
 ├── libhsl.so               142 符号  HMI 服务层
 ├── libGFrame.so            131 符号  UI 框架
 ├── libGPlatformInterface.so  38 符号  CGString/CGColor/I_*/GPI::（最底层）
 ├── libCommon.so             41 符号  工具集
 ├── libGbl.so                22 符号  bl::* BizLayer
 ├── libbase_utils.so         14 符号  asl::* RefBase 家族
 ├── libGAdaptorInterface.so   9 符号  GAI_*
 ├── libGSQuirrel.so           2 符号  Squirrel VM
 ├── libGAdaptor.so            0       纯消费方
 └── NDK 运行时                 42 符号  libc/libm/libdl/libc++_shared
```

9 个宿主库全部为 32 位 ARM EABI5 soft-float Thumb，与重建目标 ABI 一致（已核实）。

## 4. 目录结构

```
InstrumentPannelLarge_rebuilt/
├── HANDOVER.md                    ← 本文件
├── README_接口规格.md              ← 与第 2 节同内容的接口规格
├── include/
│   └── InstrumentPannelLarge.h    ← 三入口头文件 + SwitchRouteMessage
├── src/
│   ├── InstrumentPannelLarge.cpp  ← 三入口完整重建（核心交付）
│   └── module_init.cpp            ← init_array/fini_array 骨架（4+2）
├── stub_include/                  ← 桩头（按 demangle 精确签名）
│   ├── GFrame/CGString.h  CGColor.h  GblAlc.h  CGFrameSimulation.h
│   └── hsl/MultiScreenService.h
├── host_libs/                     ← 9 个宿主库（链接用，勿随源码外发）
│   └── (libGFrame/libhsl/libGbl/libGAdaptor/libGAdaptorInterface/
│        libGPlatformInterface/libCommon/libGSQuirrel/libbase_utils).so
├── Android.mk / CMakeLists.txt    ← 两种构建脚本
├── link_arm.sh                    ← 一键 ARM 链接（NDK standalone toolchain）
├── PROVIDER_MAP.txt               ← 441 符号 → 提供方 映射（最终版）
├── IMPORTED_SYMBOLS.txt           ← 441 导入符号 demangle 全表
└── LINK_CLOSURE_REPORT.txt        ← 链接闭包报告
```

## 5. 构建方法

### 5.1 前置条件

- Android NDK：**r16（最后一个支持 armeabi 的版本）**；或用新 NDK 的
  `armeabi-v7a + -mfloat-abi=soft -mthumb`（宿主库已是该 ABI，可直接参与链接）。
- `host_libs/` 下 9 个宿主库齐全（本包已含）。

### 5.2 一键链接（推荐）

```sh
export NDK_TOOLCHAIN=<你的 standalone toolchain 目录>
./link_arm.sh
```

### 5.3 ndk-build / CMake

`Android.mk` 与 `CMakeLists.txt` 已给出，注意 `APP_ABI` / `ANDROID_ABI` 的选择与
float ABI 一致性。stub_include 仅用于无原厂头文件时编译；有原厂 GFrame/hsl 头文件
时，把 include 路径指向原厂头即可（源码头文件包含名已按原厂风格编写）。

### 5.4 验收清单

1. `readelf -h` → `Machine: ARM`、`Flags: 0x5000200`（EABI5 soft-float）
2. `readelf -d | grep NEEDED` → 9 宿主库 + libc/libm/libdl/libc++_shared
3. `nm -D --defined-only` → 与原库一致：3 入口 + `_ZN18BroadcastExtScreen18SwitchRouteMessage12getClassTypeEv` + RTTI 数据符号
4. `nm -D --undefined-only` → 与原库 441 项集合一致
5. 装入 HMI 进程行为对比：启动注册、大屏标记、包加载/卸载

## 6. 已验证 / 未验证

| 项 | 状态 |
|---|---|
| 3 入口调用序列、参数、返回值 | ✅ 逐指令反汇编验证 |
| 全部字符串字面量 | ✅ 与原二进制逐字节一致 |
| 符号闭包 441/441 | ✅ 对 9 宿主库实证 |
| 源码语法（g++ -std=c++11 -fsyntax-only） | ✅ 通过 |
| ARM 链接产物 | ⏳ 沙箱无 ARM 工具链，需按 §5 执行 |
| 真机/HMI 进程行为 | ⏳ 需目标环境 |
| 内部 ~1500 函数业务逻辑 | ❌ 未重建（超出「出入接口」范围，见 §7）|

## 7. 已知限制与后续工作

1. **功能主体未重建**：`.text` 其余 ~190KB（Fragment/Observer 业务逻辑）未逐函数
   反编译。替代库与原库在**接口层**等价，业务行为等价需按 `.ARM.exidx` 函数边界
   分批补全（建议优先级：Observer 注册/回调 → 包加载相关 → 其余）。
2. **ALC 协议为建模复刻**：`fn(alc, 8, 0, &block)` 的 8/25/56 参数语义属宿主私有，
   已按寄存器/栈行为忠实复刻；如需语义级对齐，用 libGPlatformInterface.so 的
   `I_GetAlcPtr` 实现核对。
3. **init/fini 骨架**：项数与顺序与原库一致，内部 Fragment 工厂表待补全。
4. `libGbl.so` 来自 `libGbl.zip.001`（实为完整单卷 zip，CRC 校验通过）；
   `libGFrame(1).so`/`libGAdaptor(1).so` 经 md5 比对为同版本重复件，未入档。

## 8. 联系人 / 备注

- 全部逆向结论可复现：分析脚本基于 pyelftools 式自解析 + 自写 Thumb-2 解码器，
  关键中间结论（指令地址、字面量地址）已内联在源码注释中。
- 新增内部函数重建任务时，提供原库即可继续按函数边界推进。
