#########################################################
##	onfire
##
FxInfo  

Input  
	InpName	hips
End

Input  
	InpName	root
End

LifeSpan	100

Condition
	On 	Time
	Time 	0

	Event
		EName 	flash
		Type	Local
		At	hips
		Part1	powers/firecontrol/FireWeaponExplosion.part
		Part2	powers/firecontrol/FireWeaponHitSpark.part
	End

	Event
		EName 	Fire
		Type	local
		At	hips
		Part1	:FireBallTargetSteam.part
		Part2	:FireBallTargetFlame2.part
		Part3	:FireBallSpark.part	
		Sound FireExplo 100.0 100.0 .8
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	hips
		
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		30
			PhysForcePower		400
			PhysForcePowerJitter	100
		End

		Lifespan	1

	End

	Event
		EName 	Hook
		Type	Local 
		At	root
		Geom	RootToChestAdjustment
		#Sound	Atomicblast 100 30 .9

	End

	Event
		EName 	RadiusFxScale
		Type	start 
		At	Hook
		altpiv	1
		Part2	powers/firecontrol/FireBallHitRing3.part
		Part	powers/firecontrol/FireBallHitRing4.part
		LifeSpan	10
		
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
		ChildFX V_COV\PhysicsEnabled\FireBurstOUT.fx
		Lifespan 70
		LifespanJitter 	20
	End
	
#	Event 
#		HardwareOnly
#		EName 	Sparkz
#		Type 	Local
#		At 	hips
#		ChildFX V_COV\PhysicsEnabled\FireBurstUP.fx
#		Lifespan 70
#		LifespanJitter 	20
#	End
End

Condition
	On 	time
	Time 	0
	Repeat  5

	Event 
		EName 	Sparkz
		Type 	Local
		At 	hips
		ChildFX V_COV\PhysicsEnabled\FireBurstScatter.fx
		Lifespan 70
		LifespanJitter 	20
	End
		
	Event 
		HardwareOnly
		EName 	Sparkz
		Type 	Local
		At 	hips
		ChildFX V_COV\PhysicsEnabled\FireBurstExplode.fx
		Lifespan 70
		LifespanJitter 	20
	End
End


Condition
	On 	Time
	Time 	15
	Event
		EName 	flash
		Type	Destroy 

	End


End

Condition
	On 	Time
	Time 	100
	Event
		EName 	Fire
		Type	Destroy 

	End

	Event
		EName 	all
		Type	Destroy 

	End

End

End			