#########################################################
##	
##

FxInfo
LifeSpan  200


Input  
	InpName	Origin
End

Input  
	InpName	T_Root
End

Input  
	InpName	WepR
End

##################################
Condition
	On 	Time
	Time 	38

	Event
		EName 	weporigin
		Type	local 
		At	WepR
		geom	gernadelauncher
		
	End

	Event
		EName 	Prime
		Type	start 
		At	weporigin
		Altpiv	1
		geom	grenade
		bhvr	behaviors/MorterLobbprojectile2.bhvr
		Part1	:BeanBagDust2.part
		Magnet	T_Root
		LookAt	T_Root

	End

	Event
		EName 	Blast
		Type	Start
		At	weporigin
		Altpiv 1
		Geom	Fx_GunBurst
		Sound Grenade2 120.0 120.0 1.0
		
	End

	Event
		EName 	tracer
		Type	start
		At	weporigin
		Altpiv 1

		Part1	WEAPONFX/Gunfire_ShotgunMuzzflash.part
		Part2	WEAPONFX/Gunfire_Shotgunsmoke.part
		Part3	WEAPONFX/Gunfire_ShotgunMuzzflashSmall.part
	End

End

##################################

Condition
	On 	PrimeHit

	Event
		EName 	Smoke
		Type	Posit
		At	T_Root
		Part1	V_COV\WeaponFx\TearGasExplodeSmoke1.part
		Part1	V_COV\WeaponFx\TearGasExplodeSmoke2.part
		
		Part	POWERS\ItemsAndTempPowers\TearGas01.part
		Part1	POWERS\ItemsAndTempPowers\TearGasFlat.part
		Lifespan 60
	End

	Event
		EName 	Sparks
		Type	Posit
		At	T_Root
		Part	:FireExplosionFlash.part
		Part2	V_COV\WeaponFx\DynamiteExplodeSparks.part
		Sound grexplo2 120.0 120.0 1.0
		Lifespan 5
		
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	T_Root
		
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		20
			PhysForcePower		100
			PhysForcePowerJitter	10
		End

		Lifespan	1

	End
End

##################################

Condition
	On 	Time
	Time	41

	Event
		Ename 	Blast
		Type	Destroy
	End
End

Condition
	On 	Time
	Time	51

	Event
		Ename 	tracer
		Type	Destroy
	End

End

Condition
	On 	PrimeHit
	
	Event
		Ename 	Prime
		Type	Destroy
	End


End


End				