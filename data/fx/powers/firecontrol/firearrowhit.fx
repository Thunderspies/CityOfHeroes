#############################################################
## HEADER
#############################################################

FxInfo

ClampMinScale 0.875 0.875 0.875
ClampMaxScale 1.125 1.125 1.125

LifeSpan 100

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	arrow2
		Type	local
		At	Chest
		BHVR	Weapons\Bow\Archery\ArrowHit.bhvr
		Geom	NormalArrowHit
		Lifespan 44
	End

	Event
		EName 	Fire
		Type	Posit
		At	hips
		Sound molotovhit1 80 80 .86
		Sound fireexplo 80 80 .72
		Sound firezap 80 80 .76
		Sound molotovhit1 80 80 .86
	End

	Event
		EName 	spark
		Type	Local
		At	hips
		Part1	:InfernoExplosion.part
		Part1	:FireBallTargetSteam.part
		Part3	:FireBallSpark.part
		LifeSpan 30
	End

	Event
		Type	Local
		At	chest
		Part1	:FireArrowFire.part
		Part2	:FireArrow1.part
		Lifespan 7
	End

	Event
		Type	Local
		At	chest
		Part1	:InfernoExplosion.part
		Lifespan 3
	End
End

Condition
	On 	time
	Time 	0
	Repeat  3

	Event
		EName 	Sparkz
		Type 	Local
		At 	Chest
		ChildFX V_COV\PhysicsEnabled\FireBurstOUT.fx
		Lifespan 70
		LifespanJitter 	20
	End

	Event
		HardwareOnly
		EName 	Sparkz
		Type 	Local
		At 	Chest
		ChildFX V_COV\PhysicsEnabled\FireBurstOUT.fx
		Lifespan 70
		LifespanJitter 	20
	End
End

Condition
	On 	time
	Time 	0
	Repeat  5

	Event
		EName 	Sparkz
		Type 	Local
		At 	Chest
		ChildFX V_COV\PhysicsEnabled\FireBurstScatter.fx
		Lifespan 70
		LifespanJitter 	20
	End
End

#############################################################

End