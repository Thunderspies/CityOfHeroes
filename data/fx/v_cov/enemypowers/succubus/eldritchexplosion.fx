#########################################################
##	Inferno FX
##
FxInfo
	
lifespan	200



#############################################################################

Condition
	On 	Time
	Time 	0


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
		bhvr    behaviors/powers/firecontrol/Infernobubble.bhvr
		Part1	:EldritchHitRing1.part
		Part2	:EldritchHitRing2.part

		LifeSpan	15
		
	End
	
	Event
		Ename	ExplosiveForce
		Type	Start
		At	RadiusFxScale
		
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		20
			PhysForcePower		500
			PhysForcePowerJitter	50
		End

		Lifespan	1

	End

End


Condition
	On 	time
	Time 	0
	Repeat  31

	Event 
		EName 	Sparkz
		Type 	Start
		At 	RadiusFxScale
		ChildFX :HellBurst.fx
		Lifespan 75
		LifespanJitter 	15
	End
End


Condition
	On 	Time
	Time 	3

	Event
		EName 	attach
		Type	Local 
		At	Root
		Geom	RoottoChestAdjustment
		Sound EldritchExplo 80 80 .8
		
	End

	Event
		EName 	Prime
		Type	Local 
		At	attach
		altpiv	1
		Part1	:EldritchSparks.part

		Lifespan	5
				
	End

End

Condition
	On 	Time
	Time 	5

	Event
		EName 	Prime2
		Type	Local 
		At	attach
		altpiv	1
		
		Part2	:BaleFireRing.part

		Lifespan	10
		
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
