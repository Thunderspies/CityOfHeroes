#########################################################
##	onfire
##
FxInfo  

Input  
	InpName	hips
End

LifeSpan	200


Condition
	On 	time
	Time 	0
	Repeat  11

	Event 
		EName 	Sparkz
		Type 	Local
		At 	hips
		ChildFX V_COV\PhysicsEnabled\FireBurstOUT.fx
		Lifespan 70
		LifespanJitter 	20
	End
End

Condition
	On 	time
	Time 	0
	Repeat  11

	Event 
		EName 	Sparkz
		Type 	Local
		At 	hips
		ChildFX V_COV\PhysicsEnabled\FireBurstScatter.fx
		Lifespan 70
		LifespanJitter 	20
	End
End


Condition
	On 	Time
	Time 	0

	Event
		EName 	Fire
		Type	Posit
		At	hips
		Part1	\POWERS\FireControl\FireBallTargetSteam.part
		Part2	\POWERS\FireControl\FireBallTargetFlame2.part
		Part3	\POWERS\FireControl\FireBallSpark.part
		Sound FireExplo 100.0 100.0 .8
	End

	Event
		EName 	flash
		Type	Local
		At	hips
		Part1	powers/firecontrol/FireWeaponExplosion.part
		Part2	powers/firecontrol/FireWeaponHitSpark.part
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	hips
		
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		16
			PhysForcePower		600
			PhysForcePowerJitter	100
		End

		Lifespan	1

	End
End

Condition
	On 	Time
	Time 	6
	Event
		EName 	flash
		Type	Destroy 

	End


End

Condition
	On 	Time
	Time 	100

	Event
		EName 	All
		Type	Destroy 
	End


End


End			
