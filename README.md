# libInstrumentPannelLarge.so 逆向重建工程

> 32 位 ARM 车载多屏 HMI「大尺寸仪表」插件的完整逆向重建。
> 出入接口一致 · 符号闭包 441/441 · unity build 零错误

---

## 一、这是什么

目标二进制 `libInstrumentPannelLarge.so`（452,284 字节，ELF32/ARM/EABI5/soft-float/Thumb/stripped）
的源代码重建。原始库只有 3 个导出 C 函数（`Startup`/`Exit`/`LogInit`），内部约 1510 个函数、
19 个业务类全部通过自研 Thumb-2 反汇编管线分析并分层重建。

**重建完成度总览**

| 层级 | 数量 | 说明 |
|---|---|---|
| 行为级 C++ | ~207 函数 | 逐行对应反汇编，含全部 72 个具名函数 |
| 指令级语句展开 | 646 函数 | `expanded/` 8 分类文件（解码调用序列+字符串+分支）|
| 样板模板族 | ~250 函数 | runOnUIThread 统一模板 + ALC_WRAP/CFG_GETTER 宏族 |
| vtable 布局规格 | 217 个表 | `analysis/VTABLE_LAYOUT.md` + `Fragments_aligned.h` |
| 符号闭包 | 441/441 | 9 宿主库逐一实证（`PROVIDER_MAP.txt`）|
| 函数分析 | 1510/1510 | 100%，零遗漏 |

## 二、目录结构

```
├── README.md                    ← 本文件
├── HANDOVER.md                  ← 交接文档（8 章：接口规格/依赖分层/构建/验收）
├── include/                     ← 3 入口公共头文件
├── src/                         ← 接口层（3 入口 + init/fini 骨架）
├── internal/                    ← 内部函数重建（Batch 1–22）
│   ├── AlcExport.h              ← 共享 ALC 8 字块导出辅助
│   ├── PROGRESS.md              ← 批次进度与覆盖率总账
│   ├── NS_InstrumentPannelLarge/  (LargeScreenImpl / CGIPLPacket / Fragments / doStartFragment...)
│   └── NS_MultiScreenBase/        (Observer 族 / Fragment 生命周期 / config+ALC 宏族...)
├── expanded/                    ← 646 函数指令级语句展开（8 分类）
├── stub_include/                ← 40+ 宿主桩头（按 demangle 精确签名；有原厂头可整体替换）
├── host_libs/                   ← 9 个宿主库（完整包含；符号闭包实证依据）
├── analysis/                    ← 逆向分析数据库
│   ├── FUNCTIONS.txt            ← 1511 函数边界（.ARM.exidx 解码）
│   ├── VTABLES.json             ← 241 个 vtable 全槽位
│   ├── VTABLE_LAYOUT.md         ← vtable 布局对齐规格（替换原 so 的关键文档）
│   ├── CALLGRAPH.pkl / FUNCTION_DB.txt  ← 705 函数调用图 + 121KB 可读表
│   ├── PROVIDER_MAP.txt         ← 441 符号 → 提供方映射
│   └── IMPORTED_SYMBOLS.txt     ← 441 导入符号 demangle 全表
├── Android.mk / CMakeLists.txt  ← 两种构建脚本
├── link_arm.sh                  ← 一键 ARM 链接（23 源文件全量列表）
├── IMPORTED_SYMBOLS.txt / PROVIDER_MAP.txt / LINK_CLOSURE_REPORT.txt
└── libInstrumentPannelLarge_src.tar.gz / _full.tar.gz  ← 分发归档
```

## 三、快速开始

### 构建（需要 Android NDK）

```sh
# 方式 1：一键链接（推荐）
export NDK_TOOLCHAIN=<你的 NDK standalone toolchain 目录>
./link_arm.sh

# 方式 2：ndk-build
ndk-build APP_ABI=armeabi NDK_PROJECT_PATH=. APP_BUILD_SCRIPT=Android.mk

# 方式 3：CMake
cmake -B build -DCMAKE_TOOLCHAIN_FILE=$NDK/build/cmake/android.toolchain.cmake \
      -DANDROID_ABI=armeabi -DANDROID_PLATFORM=android-21
cmake --build build
```

> ABI 注意：原库为 armeabi（EABI5 soft-float）。NDK ≤ r16 直接用 `armeabi`；
> 新 NDK 用 `armeabi-v7a + -mfloat-abi=soft -mthumb`（9 个宿主库已是该 ABI）。

### 验收（HANDOVER.md §5.4 五步）

```sh
readelf -h libInstrumentPannelLarge.so              # Machine: ARM, Flags: 0x5000200
readelf -d libInstrumentPannelLarge.so | grep NEEDED # 9 宿主库 + libc/libm/libdl/libc++_shared
nm -D --defined-only libInstrumentPannelLarge.so     # 3 入口 + getClassType + RTTI
nm -D --undefined-only libInstrumentPannelLarge.so   # 441 项与原库集合一致
```

## 四、三个入口的精确行为（反汇编验证）

```c
int  StartupInstrumentPannelLarge(void *ctx);  // ALC 自注册(HMI_PANNELLARGE) → 标记大屏已创建
                                                // → loadPackage(u"InstrumentPannelLargePackage",
                                                //     intent{fromPackage, packageType})
void ExitInstrumentPannelLarge(void *ctx);      // ALC 反注册 → unloadPackage
void LogInit(void);                             // 原实现为空（BX lr）
```

## 五、已知限制（如实说明）

1. **ARM 链接未在本沙箱执行**（无 NDK 工具链）——unity build 零错误仅保证源码级正确，
   真机链接是最后一步。
2. **vtable 布局**：`Fragments_aligned.h` 按规格声明虚函数顺序，但编译产物与原二进制的
   逐槽一致需链接后用 `readelf -x .data.rel.ro` 抽查确认。
3. **ALC 协议参数语义**（8/25/56）为建模复刻——寄存器/栈行为忠实，语义级对齐需宿主头文件。
4. **样板族**（~250 个 runOnUIThread 实例等）为统一模板语义简化。

## 六、继续深化的路径

- `expanded/` 646 函数已有语句级轨迹，可按 Batch20–22 模式继续编译化
- `analysis/FUNCTION_DB.txt`（121KB）是逐函数翻译底稿
- 有原厂 GFrame/hsl 头文件时，`stub_include/` 整体替换即可

---
*生成：2026-10-20 · 自研 Thumb-2 反汇编管线 + PLT/GOT 符号解析 + .ARM.exidx 边界解码*
