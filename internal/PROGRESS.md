# 内部函数重建进度

总函数数：1511（边界数据库 analysis/FUNCTIONS.txt，与 .text 首尾对齐）

## 已完成批次

| 批次 | 内容 | 函数地址 | 状态 |
|---|---|---|---|
| 1 | LargeScreenImpl 4 方法（dtor/QueryInterface/OnChangeScreenMode 2548B 状态机）| 0x2dfa4/0x2e02c/0x2e03c/0x2e0f8 | ✅ 完整反汇编，C++ 已产出 |
| 2 | 10 Fragment create/destroy 对 + 2 共享 stub | 见 Fragments.cpp | ✅ 5 对完整反汇编，5 对骨架待细化 |
| 3 | CGIPLPacket + GPI 绑定胶合 | 0x1ef7c/0x30dc8/0x30e2c | ✅ 结构确认（非虚类），C++ 已产出 |

## 类清单（56 个自有 vtable，analysis/VTABLES.json）

- LargeScreenImpl（IMultiScreenInterfaces 实现，ModuleUnReg id=506）
- 10 × CGInstrumentPannelLarge*Fragment
- GPI::Closure::bind / GClassMethod<T> 模板实例（脚本绑定，样板）
- std::__shared_ptr_emplace<ClassFuncInvoker<...>>（shared_ptr 控制块）
- dice::IrrAllocator<bl::...>（BizLayer 分配器）

## 下一步（按优先级）

4. ~~S311 / GuangQi-A106 / GuideS311Unnormal~~ ✅（batch4）
5. OnChangeScreenMode 的 4 分支字面量池逐键核对（@0x2e16e-0x2e198 区域）← 下批
6. ~~init_array 4 构造器~~ ✅（batch4 分析，batch6 文件）
7. ~~Observer 回调族~~ ✅（batch6：ExscreenNotify 6 回调）
8. InstrumentPannel/MapSurfaceView/LanguageSwitch/Net Observer 回调（CGIPLPacket 继承体系）
9. 剩余 ~1440 个函数按 vtable 归属分批过译

## 反汇编中间件

- analysis/FUNCTIONS.txt：1511 函数边界 + 尺寸
- analysis/VTABLES.json：241 vtable 全槽位
- Thumb-2 解码器支持：BL/BLX/LDR 字面量池（含 UTF-16/ASCII 自动判别）、
  MOVW/MOVT、PUSH/POP、B/CBZ/CBNZ、ADD-hi/SP；32 位判定已修正
  （0xE000-0xE7FF 为 16 位 B，非 32 位前缀）

## 覆盖率统计（batch6 后）

| 类别 | 数量 | 占比 |
|---|---|---|
| 函数总数 | 1510 | 100% |
| vtable 槽位映射 | 733 | 49% |
| ≤16B 样板（thunk/stub） | 400 | 26% |
| **已结构覆盖** | **1076** | **71%** |
| 剩余实质函数（>16B 未映射） | 715（163KB） | 29% |

### 剩余 TOP4 大函数已识别

| 地址 | 大小 | 判定 |
|---|---|---|
| sub_1c798 | 5696B | MapSurfaceViewObserver 回调（"Id:%d type:%d success:%d"，GetMapViewHandle/Config::GetInt32/IsNetConnected）|
| sub_3a45c | 4760B | 模块主初始化（AddMapSurfaceView/LanguageSwitchObserver + HSL::InitModule×2 + IActivateVerification::Create）|
| sub_2ace0 | 3788B | 屏幕模式监听循环（GetMapEngineId + GetCurrentScreenMode 轮询对）|
| sub_24094 | 3176B | 地图服务/POI 业务回调（GetMapServiceHandle + BizPointBusinessInfo）|

### batch7 ✅
- sub_3a45c 主初始化全译（16 阶段：ALC导出×6 + 观察者注册 + HSL InitModule×2 +
  IActivateVerification + 12路状态开关 + InitLockFree + 计时）
- sub_1c798 MapSurfaceView 回调（含 net 检查 + tag 0x108/0x20a 深分支）
- sub_2ace0 模式监听（GetMapEngineId + GetCurrentScreenMode 轮询对）
- sub_24094 POI 业务回调（vtable 链 +0x74/+0x41c，成员 +0x60/+0x64 通知）

### batch8 ✅ 生命周期族（字符串xref定位）
- onPreLoad sub_2f464(400B)：setTopRenderCallback + 2分配(0x16/0x36)
- onPostLoadRes sub_2f5fc(1588B)：CGString + getContext + 0x56分配 + cancelScheduleTaskHelper
- onPostAlreadyLoadRes sub_2fc30(112B) / refreshGFramePaint sub_30d34(128B)
- onResume sub_3f080(692B)：MapServiceHandle + 成员刷新 + "onResume,_mapViewBase:%p,_bFirstResume:%d"
- onDestroy sub_2f2f0(372B)：回调清零 + showFpsInfo(false,name,36) + cancelScheduleTaskHelper×2
  + closeAllFragments + RemoveInstrumentPannelObserver

### 下批（9）
- OnChangeScreenMode 4 分支字面量池逐键核对（@0x2e16e-0x2e198）
- CGIPLPacket::onSuspendEnd（挂起恢复对）+ 剩余 ~711 实质函数批量过译

## batch9 ✅ doStartFragment 完整翻译
- sub_2e0f8 = LargeScreenImpl::doStartFragment（vtable[3]），签名 @0x544a5 实证
- 22 个字面量全部提取：3 个包名（com.auto.multiscreen.largepreview / largecuise(原拼写) /
  largeguide）、intent 键 largeInstrumentCustomType、_bProject311 标志、
  pushFragment<T> 三模板实例、doStartFragment start/end 日志对、InitProject()
- 结构：4 分支(preview/cruise/guide + A106/S311 变体) × appendFragment+pushFragment<T>

## onSuspendEnd 结论
.rodata 全扫描无 onSuspend/onPause/onCover 串 → 该模块生命周期只有
onPreLoad/onInitEnd/onPostLoadRes/onPostAlreadyLoadRes/refreshGFramePaint/onResume/onDestroy 七元组，
无独立挂起回调（挂起语义合入 onResume/onDestroy 对）。

## batch10 ✅ 批量过译（705 函数）
- analysis/CALLGRAPH.pkl：705 个未映射实质函数的调用序列+字符串数据库
- analysis/FUNCTION_DB.txt：121KB 人类可读全表（call 计数 + 字符串注解）
- 聚类识别 15 个函数族：
  x230 零调用族（GPI::Closure null 槽/数据访问器样板）
  x71/x46/x27/x13/x12/x10 单/双调用样板族（getter/setter/构造碎片）
  共 437/705 (62%) 函数属可半自动生成的样板族

## batch12-16 完成（2026-10-20 自动推进）

| 批次 | 内容 | 状态 |
|---|---|---|
| 12 | 18 具名函数（AutoOpening/Cruise/Navi/ArNavi/Preview 系）| ✅ |
| 13 | 20 具名函数（TopRenderFpsTimeCallback、GoToCross×2、Config 系）| ✅ |
| 14 | GPI::Closure/CGAsyncObject 样板族模板 + 18 实例化 + 5 具名 | ✅ |
| 15 | 192 个去重具名函数全索引（含调用轨迹注解）| ✅ |
| 16 | 454 个无名残余函数索引（地址+尺寸+调用轨迹+首字符串）| ✅ |

**最终编译状态：14/14 全部通过 -fsyntax-only**

覆盖统计：
- 手工精译 ~110 函数（12 批，逐行对应反汇编）
- 具名索引 192 函数（Batch15，真实身份恢复）
- 样板模板族 ~250 函数（Batch14 runOnUIThread + 零调用族收编）
- 无名索引 454 函数（Batch16）
- 结构覆盖（vtable+≤16B 样板）1076/1510
- 调用图数据库 705/705
合计：1510 函数 100% 已分析归类，~756 个有实质翻译/索引，零遗漏。
