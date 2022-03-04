# d3d12ProxyEdrDx11_0
Totally naive app using proxy/dll forwarding to try and get Elden Ring running on DX12 hardware with Feature level 11_0 (f.e. Nvidia Kepler GPUs) 

To be honest I just get really pissed that Elden Ring crashes with the famous WSOD (White Screen of Death) on my GPU (NVIDIA Tesla K40 using off-screen rendering),
I never imagined that I'd actually get something working :-)

So this is it:

- Some debugging revealed that in *my* case the white screen of death is caused by failing to properly initialize DX12 (and not bothering to check the return code...)
- To be more specific: The game seems to call D3D12CreateDevice with a min required feature level of D3D_FEATURE_LEVEL_12 - which is greater than the max. featured on NVIDIA Kepler GPUs (11_0)
- After that things go downhill fast (mainly because the game doesn't seem to perform proper error handling)...

So this project tries to do the following in order to *maybe* get elden ring working for your old non-supported D3D_FEATURE_LEVEL_11_0 GPU:
- provide a "fake" d3d12.dll that will be loaded instead of the real d3d12.dll by elden ring (when placed in the elden ring game exe folder)
- the "fake" d3d12.dll just forwards all but one function to the original d3d12 dll (that has to be placed in the same folder and renamed to d3d12_original.dll)
- the "fake" d3d12.dll provides a different implementation of D3D12CreateDevice (the function call that fails), which just calls the original D3D12CreateDevice function with the same parameters but D3D_FEATURE_LEVEL set to 11_0 instead of 12_0
- -> This *usually* leads to successfull D3D12 init and - to my	complete surprise - getting ingame on my Nvidia Kepler GPU
- -> The whole project is very likely not a good example how to do something like this, so use with caution ! I just thought this might be helpful for other people stuck on Nvidia GTX 6xx or 7xx series GPUS, no guarantees or anything !


How to try this out ?

-Build this project with Visual Studio 2019 (community edition will do) using the x64 target (or just grab the debug dll file from the github release)

-Grab the real d3d12.dll and d3d12core.dll from your windows/system32 folder, rename the real d3d12.dll to d3d12_original.dll and place both dlls in the elden ring game folder (where eldenring.exe is located)

-Place the new "fake" d3d12.dll in the elden ring game folder, too (where eldenring.exe is located)

-Try to launch elden ring, now you should get past the white screen :-) 

Hints:
- The WSOD is just a symptom and apparently has different causes - only try this if you exhausted all other possible causes any your GPU is the likely culprit
- You may need to use a version of elden ring with copy protection removed, this change is likely to trigger anti-cheat and anti-debugging/drm measures, so don't expect this to play nice with the retail version
- Try to start eldenring.exe directly, not via the start_protected_game.exe (EasyAnti-Cheat has to be disabled, so only offline play should work)
- Maybe grab DebugView (https://docs.microsoft.com/en-us/sysinternals/downloads/debugview) and watch the eventlog while launching elden ring: 
You should see some debug messages from the fake dll like this:

[15016] D3D12CreateDevice-LoadLibrary Result:3360000

[15016] D3D12CreateDevice - Proxy function called!

[15016] D3D12CreateDevice - Original function address queried

[15016] D3D12CreateDevice-CallResult:0 
-> this is the return code of D3D12CreateDevice and can used to check the for error messages (see https://docs.microsoft.com/en-us/windows/win32/direct3ddxgi/dxgi-error)
