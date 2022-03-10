# d3d12ProxyEdrDx11_0
Totally naive app using proxy/dll forwarding to try and get Elden Ring running on DX12 hardware with Feature level 11_0 (f.e. Nvidia Kepler GPUs) 

To be honest I just got really pissed that Elden Ring crashes with the famous WSOD (White Screen of Death) on my GPU (NVIDIA Tesla K40 using off-screen rendering),
I never imagined that I'd actually get something working :-)

So this is it:

- Some debugging revealed that in *my* case the white screen of death is caused by failing to properly initialize DX12 (and not bothering to check the return code...)
- To be more specific: The game seems to call D3D12CreateDevice with a min required feature level of D3D_FEATURE_LEVEL_12 - which is greater than the max. supported feature level on NVIDIA Kepler GPUs (11_0)
- After that things go downhill fast (mainly because the game doesn't seem to perform proper error handling)...

So this project tries to do the following in order to *maybe* get elden ring working for your old non-supported D3D_FEATURE_LEVEL_11_0 GPU:
- provide a "fake" d3d12.dll that will be loaded instead of the real d3d12.dll by elden ring (when placed in the elden ring game exe folder)
- the "fake" d3d12.dll just forwards all but one function to the original d3d12 dll (that has to be placed in the same folder and renamed to d3d12_original.dll)
- the "fake" d3d12.dll provides a different implementation of D3D12CreateDevice (the function call that fails), which just calls the original D3D12CreateDevice function with the same parameters but D3D_FEATURE_LEVEL set to 11_0 instead of 12_0
- -> This *usually* leads to successfull D3D12 init and - to my	complete surprise - getting ingame on my Nvidia Kepler GPU
- -> The whole project is very likely not a good example how to do something like this, so use with caution ! I just thought this might be helpful for other people stuck on Nvidia GTX 6xx or 7xx series GPUs, no guarantees or anything !


# How to try this out ?

-Build this project with Visual Studio 2019 (community edition will do) using the x64 target OR just grab the dll file from the github release

-Grab the real d3d12.dll and d3d12core.dll from your windows/system32 folder, place both dlls in the elden ring game folder (where eldenring.exe is located) 
AND RENAME the real d3d12.dll to d3d12_original.dll (the one inside the game folder, NOT the dll in windows/system32)

Note: If you don't have d3d12core.dll in windows/system32 just omit this dll - or update your Windows version (then perform the copy of both dlls again after updating)

-Place the new "fake" d3d12.dll in the elden ring game folder, too (where eldenring.exe is located)

-Disable EasyAnti-Cheat: 
1. Rename start_protected_game.exe to start_protected_game_original.exe 
2. Create a copy of eldenring.exe and rename this copy to start_protected_game.exe
-> Note that only offline play will work, not disabling EasyAnti-Cheat should result in a crash with a "untrusted system files" message (or similar) and might get you banned if otherwise

-Try to launch elden ring, now you should get past the white screen :-) 

# Hints
- The WSOD is just a symptom and apparently has different causes - only try this if you exhausted all other possible causes any your GPU is the likely culprit
- You may need to use a version of elden ring with copy protection removed, this change is likely to trigger anti-cheat and anti-debugging/drm measures, so don't expect this to play nice with the retail version
- Try to start eldenring.exe directly, not via the start_protected_game.exe (EasyAnti-Cheat has to be disabled, so only offline play should work - see instructions above)
- Maybe grab DebugView (https://docs.microsoft.com/en-us/sysinternals/downloads/debugview) and watch the eventlog while launching elden ring: 
You should see some debug messages from the fake dll like this:

[15016] D3D12CreateDevice-LoadLibrary Result:3360000

[15016] D3D12CreateDevice - Proxy function called!

[15016] D3D12CreateDevice - Original function address queried

[15016] D3D12CreateDevice-CallResult:0 
-> this is the return code of D3D12CreateDevice and can be used to check for error messages (see https://docs.microsoft.com/en-us/windows/win32/direct3ddxgi/dxgi-error)

# Notes

1. So for I've received success reports for the following GPUs: 
- Geforce 940mx, 960m, 850m, 850mx, 680, 750ti, 760, 770, 780, 780Ti
- Radeon R9 280x, R7 370

2. According to user reports this works also for Assassin's Creed Valhalla
  - Feel free to give this a shot on other DX12 games that are requiring FEATURE_LEVEL_12_0 or 11_1 if you're stuck with a DX12 card that only supports FEATURE_LEVEL_11_0.
  - If this works for other games please let me know (add a comment to the pinned issues regarding support for other games) and I'll add a note to the README so other players can find this easier.

# Known Issues

- You might see some screen flickering and/or incorrect fog rendering on Nvidia GPUs 
- There have been reports of frequent crashes ingame on AMD R7 GPUs

# Donate

If this helped you please consider donating some beer money (won't be used for Coffee, I promise)

[![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=F2DK2UNHLCFHL)
