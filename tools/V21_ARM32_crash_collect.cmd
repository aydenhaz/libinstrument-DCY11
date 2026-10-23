@echo off
setlocal EnableExtensions
set "PKG=com.autonavi.amapautp.yh"
set "OUT=%~dp0V21_ARM32_CRASH_%RANDOM%"

where adb.exe >nul 2>nul
if errorlevel 1 (
  echo adb.exe was not found in PATH.
  exit /b 1
)
set "DEVICE_STATE="
for /f "delims=" %%S in ('adb get-state 2^>nul') do set "DEVICE_STATE=%%S"
if /I not "%DEVICE_STATE%"=="device" (
  echo Connect one authorized Android device and try again.
  adb devices
  exit /b 1
)
mkdir "%OUT%" || exit /b 1
echo Capturing a clean log and launching %PKG%...
adb logcat -b all -c >"%OUT%\clear.txt" 2>&1
adb shell am force-stop %PKG% >"%OUT%\force_stop.txt" 2>&1
adb shell monkey -p %PKG% -c android.intent.category.LAUNCHER 1 >"%OUT%\launch.txt" 2>&1
echo Waiting 60 seconds for startup, display initialization, and crash...
timeout /t 60 /nobreak >nul

adb logcat -b all -d -v threadtime >"%OUT%\logcat.txt" 2>&1
adb shell dumpsys dropbox --print data_app_crash >"%OUT%\java_crashes.txt" 2>&1
adb shell dumpsys dropbox --print data_app_native_crash >"%OUT%\native_crashes.txt" 2>&1
adb shell getprop ro.product.cpu.abilist >"%OUT%\abi.txt" 2>&1
adb shell pm path %PKG% >"%OUT%\package_path.txt" 2>&1
findstr /i /c:"InstrumentPannelLarge" /c:"AndroidRuntime" /c:"FATAL EXCEPTION" /c:"linker" /c:"SIGSEGV" /c:"SIGABRT" /c:"GFrame" /c:"amap_so" /c:"signum" "%OUT%\logcat.txt" >"%OUT%\focus.txt"
powershell -NoProfile -Command "Compress-Archive -LiteralPath $env:OUT -DestinationPath ($env:OUT + '.zip') -Force"
if errorlevel 1 (
  echo ZIP creation failed. Upload the folder: "%OUT%"
  exit /b 1
)
echo Upload this ZIP: "%OUT%.zip"
exit /b 0
