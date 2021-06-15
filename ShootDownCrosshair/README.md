# Shoot Down the Center of the Crosshair using LineTraceSingleByChannel

* Unreal Version: **5.0.0**
* Template: **First Person**
* Project Name: **FPUE5**

![Screenshot](img/screenshot-line-trace.png)

**All credit goes to the Shoot Game Tutorial. I just tried to split it out and simplify it so I could understand it better.** 

*this code is ignoring weapon spread*

1. update the HUD.cpp file so the crosshair is in the center of screen. The crosshair texture is 16x16 so I adjusted it by 8 units in each direction on line 28. I think it's purposely off-center in the template to be more aligned with the yellow projectile, but I'm really not sure I could wrong.
2. MyProjectCharacter.h - line 143
3. MyProjectCharacter.cpp- line 14, lines 144-169, and lines 302-321


## Helpful Links
* [Shooter Game](https://www.unrealengine.com/marketplace/en-US/item/fea72c38b29841e592f9ed2007a9d756)
* [Shoot Game Unreal Docs](https://docs.unrealengine.com/4.26/en-US/Resources/SampleGames/ShooterGame/)
* [LineTraceSingleByChannel](https://docs.unrealengine.com/4.26/en-US/API/Runtime/Engine/Engine/UWorld/LineTraceSingleByChannel/)
* [FCollisionQueryParams](https://docs.unrealengine.com/4.26/en-US/API/Runtime/Engine/FCollisionQueryParams/)
* [GetInstigatorController](https://docs.unrealengine.com/4.26/en-US/API/Runtime/Engine/GameFramework/AActor/GetInstigatorController/)
* [APlayerController](https://docs.unrealengine.com/4.26/en-US/API/Runtime/Engine/GameFramework/APlayerController/)
* [AController](https://docs.unrealengine.com/4.26/en-US/API/Runtime/Engine/GameFramework/AController/)
* [Unreal Tournament Source](https://github.com/EpicGames/UnrealTournament/blob/clean-master/UnrealTournament/Source/UnrealTournament/Private/UTWeapon.cpp)
* [Another way to get player controller](https://answers.unrealengine.com/questions/63322/how-to-get-the-player-controller-in-c.html)
* [Instigator](https://docs.unrealengine.com/4.26/en-US/API/Runtime/Engine/Engine/FActorSpawnParameters/Instigator/)
* [GetPlayerViewPoint](https://docs.unrealengine.com/4.26/en-US/API/Runtime/Engine/GameFramework/APlayerController/GetPlayerViewPoint/)
* [Great video on Debug Logging](https://www.tomlooman.com/unreal-engine-4-c-tutorial-effective-debug-logging/)
