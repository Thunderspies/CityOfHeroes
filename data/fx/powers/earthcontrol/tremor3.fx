#########################################################
##	Inferno FX
##
FxInfo
	
lifespan	190


Condition

	On Time 
	Time 0
	
	Event
		Type	Local 
		At	hips
		Sound tremor 100.0 100.0 .8
	End

	Event
		EName   RadiusFxScale
		Type	local
		At	Mystic  ##chest
		Bhvr	powers/firecontrol/InfernoA.bhvr
	End

End

Condition

	On 	Time
	Time 	63

	Event
		EName 	Prime
		Type	Local 
		At	Mystic  ##chest
		
		Part1	:Inferno.part
		Part2	:Infernodark.part
		LifeSpan	15
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	Mystic
		
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		40
			PhysForcePower		250
			PhysForcePowerJitter	100
		End

		Lifespan	1
	End
End

Condition
	On 	Time
	Time 	68
	Repeat	30

	Event
		Type	Local 
		At	Mystic
		
		ChildFX		V_COV\PhysicsEnabled\RockDebrisGoldExplode.fx

		Lifespan	100
		
	End

End

Condition
	On 	Time
	Time 	68
	Repeat	19

	Event
		Type	Local 
		At	Mystic
		
		ChildFX		V_COV\PhysicsEnabled\RockDebrisGoldScatter.fx

		Lifespan	100
		
	End

End

Condition
	On 	Time
	Time 	68

#	Event
#		EName 	Primex
#		Type	Local 
#		At	Mystic  ##chest
#		Bhvr	Behaviors/CameraShake05Subtle.bhvr
#		Part1	:InfernorockBits.part
#		Part2	:InfernorockBits2.part
#	End
#


	Event
		EName 	Prime1
		Type	Local 
		At	RadiusFxScale
		bhvr    behaviors/powers/firecontrol/Infernobubble4.bhvr
		Part1	:InfernoYellow2.part
		Part2	:InfernoYellowFlame01.part
		Part	:InfernoYellow2dark.part
		Part	:InfernoYellowFlame01dark.part
		LifeSpan	35
	End		

	Event
		EName 	Prime
		Type	Local 
		At	Mystic  ##chest
		Part	:TremorLightRays.part
		LifeSpan	10
	End
End

Condition
	On 	Time
	Time 	68

	Event
		EName	ring
		Type	start
		At	RadiusFxScale
		geom	EarthRing
		bhvr	behaviors/BlastRingScale3.bhvr
		LifeSpan	10
	End

	
	
End



End
