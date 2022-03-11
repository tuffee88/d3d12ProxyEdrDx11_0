# d3d12ProxyEdrDx11_0
Project using proxy function and dll forwarding to try and get Elden Ring running on *DX12* hardware with Feature level 11_0  

To be honest I just got really pissed that Elden Ring crashes with the famous WSOD (White Screen of Death) on my GPU,
I never imagined that I'd actually get something working :-)

So this is it:

- Some debugging revealed that in *my* case the white screen of death is caused by failing to properly initialize DX12 
- To be more specific: The game seems to call D3D12CreateDevice with a min required feature level of D3D_FEATURE_LEVEL_12 - which is greater than the max. supported feature level on my GPU (11_0)
- After that things go downhill fast (mainly because the game doesn't seem to perform proper error handling)...

This project tries to do the following in order to *maybe* get Elden Ring working for your old non-supported D3D_FEATURE_LEVEL_11_0/ 11_1 GPUs:
- provide a "fake" d3d12.dll that will be loaded instead of the real d3d12.dll by Elden Ring 
- the "fake" d3d12.dll just forwards all but one function to the original d3d12.dll (renamed to d3d12_original.dll)
- the "fake" d3d12.dll provides a different implementation of the failing function call D3D12CreateDevice, which just calls the original D3D12CreateDevice function with the same parameters but D3D_FEATURE_LEVEL set to 11_0 instead of 12_0
  - This *usually* leads to successfull D3D12 init and - getting ingame on various GPUs


# How to try this out ?

1. Build this project with Visual Studio 2019 using the x64 target **OR** just grab the dll file from the Github release

2. Grab the real d3d12.dll and d3d12core.dll from your windows/system32 folder, place both dlls in the Elden Ring game folder (where eldenring.exe is located) 

3. RENAME the real d3d12.dll to d3d12_original.dll (the one inside the game folder, NOT the dll in windows/system32)
   - Note: If you don't have d3d12core.dll in windows/system32 just omit this dll - or update your Windows version (then perform the copy of both dlls again after updating)

4. Place the new "fake" d3d12.dll in the Elden Ring game folder, too (where eldenring.exe is located)

5. Disable EasyAnti-Cheat: 
   - Rename start_protected_game.exe to start_protected_game_original.exe 
   - Create a copy of eldenring.exe and rename this copy to start_protected_game.exe
     - Note that only offline play will work, not disabling EasyAnti-Cheat should result in a crash with a "untrusted system files" message (or similar) and might get you banned otherwise

6. Try to launch Elden Ring, now you should get past the white screen :-) 

# Hints
1. The WSOD is just a symptom and apparently has different causes - only try this if you exhausted all other possible causes any your GPU is the likely culprit
2. You'll need to disable EasyAnti-Cheat & Online-Play won't work, see instructions above
3. If you're having problems or are just interested how this works I suggest to use DebugView (https://docs.microsoft.com/en-us/sysinternals/downloads/debugview) and watch the eventlog while launching Elden Ring, you should see some debug messages from the proxy dll like this:

   D3D12CreateDevice - LoadLibrary Result: xxxx
   
   D3D12CreateDevice - Proxy function called !
   
   D3D12CreateDevice - Original function address queried
   
   D3D12CreateDevice - CallResult: xxx

   -> this is the return code of D3D12CreateDevice and can be used to check for error messages 
(see https://docs.microsoft.com/en-us/windows/win32/direct3ddxgi/dxgi-error)

4. If you're experiencing issues after passing the WSOD try running this game via Vulkan using vkd3d-proton (especially on AMD cards):
   - You can get Windows builds at: https://www.nexusmods.com/eldenring/mods/12
   - Replace d3d12_original.dll with the d3d12.dll from vkd3d-proton and try launching the game

# Notes
1. So far I've received success reports for the following GPUs: 
- Geforce: 940mx, 960m, 850m, 850mx, 680, 750ti, 760, 770, 780, 780Ti, Tesla K40*
- Radeon: R9 280x, R7 370

2. According to user reports this works *for some cards* with Assassin's Creed Valhalla and Death Stranding, too
  - Feel free to give this a shot on other DX12 games that are requiring FEATURE_LEVEL_12_0 or 11_1 if you're stuck with a DX12 card that only supports FEATURE_LEVEL_11_0.
  - If this works for other games please let me know (add a comment to the pinned issues regarding support for other games) and I'll add a note to the README so other players can find this easier.
  - Beware that you might experience crashes or graphics glitches - in that sorry, but that's not something that can be fixed in the scope of this project. You may try to run this using Vulkan vkd3d-proton, which *might* fix the issues but usually has a performance penalty (see hint above).

3. The whole project is very likely not a good example how to do something like this, so use with caution ! 

I just thought this might be helpful for other people stuck on Nvidia GTX 6xx or 7xx series GPUs, no guarantees or anything !

# Known Issues
- You might see some screen flickering and/or incorrect fog rendering on Nvidia GPUs 
- There have been reports of frequent crashes ingame on AMD R7 GPUs

# Donate
If this helped you please consider donating some beer money (won't be used for Coffee, I promise)

[![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=F2DK2UNHLCFHL)
