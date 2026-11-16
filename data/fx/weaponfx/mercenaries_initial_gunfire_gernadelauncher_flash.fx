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
		geom	Mastermind_Military_Rifle
		
	End

	Event
		EName 	Prime
		Type	start 
		At	weporigin
		Altpiv	1
		geom	grenade
		bhvr	behaviors/Mercenaries_MorterLobbprojectile2.bhvr
		Part1	:BeanBagDust2.part
		Magnet	T_Root
		LookAt	T_Root

	End

	Event
		EName 	Blast
		Type	Start
		At	weporigin
		Altpiv 1
#		Geom	Fx_GunBurst
#		BHVR	WEAPONs\WeaponRotate.bhvr
		Sound Mercenary_Grenade 120.0 120.0 1.0
		
	End

	Event
		EName 	tracer
		Type	start
		At	weporigin
		Altpiv 1

		BHVR	WEAPONs\WeaponRotate.bhvr
		Part1	WEAPONFX/SpecOpps_ShotgunMuzzflash.part
		Part2	WEAPONFX/SpecOpps_Shotgunsmoke.part
		Part3	WEAPONFX/SpecOpps_ShotgunMuzzflashSmall.part
	End

End

##################################

Condition
	On 	PrimeHit
	
	Event
		Type	Start
		At	T_Root
		BHVR	Behaviors/CameraShake01Subtle.bhvr
	End

	Event
		EName 	Fire
		Type	Posit
		At	T_Root
		Part1	:FireExplosion.part
		Part2	powers/firecontrol/FireWeaponHitSpark.part
		Part	:ExplosiveFlashBang.part
		Lifespan 2
	End

	Event
		EName 	Sparks
		Type	Posit
		At	T_Root
		Part1	:GernadeLauncherSparks.part
		#Part2	:GernadeLauncherSpark.part
		Sound grexplo2 120.0 120.0 1.0
		Lifespan 5
		
	End

	Event
		EName 	Sparks
		Type	Posit
		At	T_Root
		Part	:FireExplosionFlash.part
		#Part2	:GernadeLauncherSpark.part
		Sound grexplo2 120.0 120.0 1.0
		Lifespan 5
		
	End
		Event
		Type	start
		At	T_Root

		Part1	POWERS\ItemsAndTempPowers\BlindingLightFlash.part
		Part5	POWERS\ItemsAndTempPowers\FlashStreaks.part
		Sound flashgrenade2 80 80 1.0
		LifeSpan	70
	End
	
	Event
		Type	start
		At	T_Root

		Part4	POWERS\ItemsAndTempPowers\BlindingRingFlash.part
		LifeSpan	80
	End

	Event
		Type	start
		At	T_Root

		
		Part2	POWERS\ItemsAndTempPowers\BlindingLightRays.part
		
		LifeSpan	20
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