#############################################################
## RepulsionBomb.fx
#############################################################

FxInfo
lifeSpan	200

#########################################################

Input
	Inpname	Hips
End

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	hookup
		Type	start
		At	Origin
		geom	RootToChestAdjustment
		Sound repulsion 100 100 .88
	End

	Event

		Ename	ForceBall
		Type	local
		At	hookup
		Altpiv	1
		Part1	:BubbleSphere8.part
		Bhvr 	behaviors/powers/forcefield/ForceFieldBomb.bhvr
		geom	FX_ForceBubble
		LifeSpan	9.5
	End

End

Condition
	On 	Time
	Time 	6

	Event

		ENAME	BubbleExplosion
		Type	start
		At	hookup
		Altpiv	1

		Part1	:BubbleSphereExplosion.part
		Part2	:BubbleSphereExplosion2.part

	End

End

Condition
	On 	Time
	Time 	3

	Event
		EName	ring
		Type	start
		At	hookup
		Altpiv	1
		geom	ForceRing
		bhvr	behaviors/RepulsionBombRing.bhvr
		LifeSpan		7
	End

End

Condition
	On 	Time
	Time 	15

	Event

		Ename	ForceBall
		Type	local
		At	Origin
		Part1	:BubbleSphere8.part
		Bhvr 	behaviors/powers/forcefield/ForceFieldBombBits.bhvr

		LifeSpan	6
	End
End

End
