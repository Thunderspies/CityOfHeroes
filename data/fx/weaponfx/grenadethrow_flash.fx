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
	On	Time
	Time	10

	Event
		Type	Local	
		At	Origin
		Sound grtoss1 60 60 .7
	End
End



Condition
	On 	Time
	Time 	10

	Event
		EName 	Prime
		Type	local 
		At	wepR
		geom	grenade
		bhvr	behaviors/grenadeFade.bhvr
		lifespan	22
	End

End

Condition
	On 	Time
	Time 	32

	Event
		EName 	Prime
		Type	start 
		At	wepR
		geom	grenade
		bhvr	behaviors/MorterLobbprojectile2.bhvr
		Part1	:BeanBagDust2.part
		Magnet	T_Root
		LookAt	T_Root

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
	Time	34

	Event
		Ename 	Blast
		Type	Destroy
	End
End

Condition
	On 	Time
	Time	43

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