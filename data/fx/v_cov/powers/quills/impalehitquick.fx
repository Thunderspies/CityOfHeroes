#############################################################
## HEADER
#############################################################

FxInfo

ClampMinScale 0.75 0.75 0.75
ClampMaxScale 1.25 1.25 1.25

LifeSpan 50

#########################################################

Input
	Inpname	Hips
End

#########################################################

Condition
	On 	Time
	Time 	5

	Event
		Type	Local
		At	chest
		Sound Darthit1 70 70 .9
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		ENAME	CrossHairNode
		Type	Local
		At	Chest
		Part1	Powers\MartialArts\MAHitStar01.part
		Part2	Powers\MartialArts\MAHitStar02.part
		Part3	Powers\MartialArts\MAHitStar03.part
		Part4	Powers\MartialArts\MAHitGlow01.part
		Part5	Powers\MartialArts\MAHitRays01.part
		Sound poisonhit2 50 50 .7
		Lifespan 200
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Gas
		Type	Local
		At	chest
		Part1	:MustardGasHitA.part
		Part2	:MustardGasSpecksHitA.part
		Part3	:MustardGasBubblesA.part
		LifeSpan 30
	End

	Event
		EName	Gas1
		Type	Local
		At	UArmL
		Part2	:MustardGasSpecksHitA.part
		Part3	:MustardGasBubblesA.part
		LifeSpan 25
	End

	Event
		EName	Gas2
		Type	Local
		At	UArmR
		Part2	:MustardGasSpecksHitA.part
		Part3	:MustardGasBubblesA.part
		LifeSpan 25
	End

	Event
		EName	HeadConfusionClouds
		Type	Local
		At	Head
		Part1	:MustardGasHeadA.part
		Part2	:MustardGasSpecksHeadA.part
		Part3	:MustardGasBubblesHeadA.part
		LifeSpan 25
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Create
		Inherit	ALL
		Update	Position Rotation
		At	chest
		Bhvr	Behaviors\QuillProjectile4.bhvr
		Geom	Spine01
	End

	Event
		Type	Create
		Inherit	ALL
		Update	Position Rotation
		At	chest
		Bhvr	Behaviors\QuillProjectile4.bhvr
		Geom	Spine03
	End

	Event
		Type	Create
		Inherit	ALL
		Update	Position Rotation
		At	chest
		Bhvr	Behaviors\QuillProjectile4.bhvr
		Geom	Spine07
	End

	Event
		Type	Create
		Inherit	ALL
		Update	Position Rotation
		At	chest
		Bhvr	Behaviors\QuillProjectile4.bhvr
		Geom	Spine06
	End
End

#############################################################

Condition
	On 	Time
	Time 	0
	Repeat	11

	Event
		EName	GasSpurts
		Type	Local
		At	chest

		ChildFX	V_COV\PhysicsEnabled\PoisonDarkSplash.fx
		Lifespan	75
	End

	Event
		HardwareOnly
		EName	GasSpurts
		Type	Local
		At	chest
		ChildFX	V_COV\PhysicsEnabled\PoisonDarkSplash.fx
		Lifespan	75
	End
End

#############################################################

Condition
	On 	Time
	Time 	0
	Repeat	4

	Event

		EName	GasHurts
		Type	Create
		Inherit	ALL
		Update	Position Rotation
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\PoisonQuillDarkScatter.fx
		Lifespan 75
	End

	Event
		HardwareOnly
		EName	GasHurts
		Type	Create
		Inherit	ALL
		Update	Position Rotation
		At	chest
		ChildFX	V_COV\PhysicsEnabled\PoisonQuillDarkScatter.fx
		Lifespan 75
	End
End

#############################################################

End