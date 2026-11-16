#########################################################
##	
##

FxInfo
LifeSpan  200

Input  
	InpName	Origin
End

Input  
	InpName	Target
End

Input  
	InpName	T_Chest
End

Input  
	InpName	Mystic
End

Input  
	InpName	WepR
End

Input  
	InpName	WepL
End

##################################
Condition
	On 	Time
	Time 	27


	Event
		EName 	Prime
		Type	start
		At	mystic
		#Geom	FX_MachineGunBurst
		Bhvr	Behaviors/Missleprojectile.bhvr
		Geom	_5th_missile
		Part1	AnimatedCharacterParts/5thmissile_smoke.part
		Part2	AnimatedCharacterParts/5thmissile_smoke2.part
		Part3	AnimatedCharacterParts/5thmissile_fire.part
		#Part1	:MachineGunFlash.part
		#Part2	:MachineGunGlow.part
		Magnet	Target
		LookAt	Target
	End

	
	Event
		EName 	Blast1
		Type	start
		At	WepR
		Geom	Fx_GunBurst
		Part1	WEAPONFX/Gunfire_ShotgunMuzzflash.part
		Part2	WEAPONFX/Gunfire_Shotgunsmoke.part
		Part3	WEAPONFX/Gunfire_ShotgunMuzzflashSmall.part
		Sound rocketlaunch 100.0 100.0 .7
	End

	Event
		EName 	KickBackSmoke
		Type	start
		At	WepR
		Part1	:RocketLauncherSmoke.part
		Part2	WEAPONFX/Gunfire_Shotgunsmoke.part
		Part4	WEAPONFX/Gunfire_MissleShell.Part
	End

End


##################################

Condition
	On 	PrimeHit

	Event
		EName 	Fire
		Type	Posit
		At	T_Chest
		Part1	:FireExplosion.part
		#Part2	powers/firecontrol/FireWeaponHitSpark.part
		Lifespan 8
	End

	Event
		EName 	Sparks
		Type	Posit
		At	T_Chest
		Part1	:GernadeLauncherSparks.part
		#Part2	:GernadeLauncherSpark.part
		Sound grexplo2 120.0 120.0 1.0
		Lifespan 5
		
	End

	Event
		EName 	Sparks
		Type	Posit
		At	T_Chest
		Part	:FireExplosionFlash.part
		#Part2	:GernadeLauncherSpark.part
		Lifespan 5
		
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	T_Chest
		
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		20
			PhysForcePower		300
			PhysForcePowerJitter	25
		End

		Lifespan	1

	End

	Event
		Ename	Prime
		Type	Destroy
	End

End


Condition
	On 	Time
	Time	30

	Event
		Ename 	Blast1
		Type	Destroy
	End

	
End

Condition
	On 	Time
	Time	34

	Event
		Ename 	KickBackSmoke
		Type	Destroy
	End

End



End				