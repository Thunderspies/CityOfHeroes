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
		Sound GasArrowHit 70 70 .66
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