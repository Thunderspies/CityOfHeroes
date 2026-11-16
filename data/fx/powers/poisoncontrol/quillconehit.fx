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
		Lifespan 200
	End
End

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Chest
		Part1	:MustardGasHitA.part
		Part2	:MustardGasSpecksHitA.part
		Part3	:MustardGasBubblesA.part
		LifeSpan 30
	End

	Event
		Type	Local
		At	UArmL
		Part2	:MustardGasSpecksHitA.part
		Part3	:MustardGasBubblesA.part
		LifeSpan 25
	End

	Event
		Type	Local
		At	UArmR
		Part2	:MustardGasSpecksHitA.part
		Part3	:MustardGasBubblesA.part
		LifeSpan 25
	End

	Event
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
		BhvrOverride
			Scale		0.25 0.25 0.25
		End
		Geom	quill01
	End

	Event
		Type	Create
		Inherit	ALL
		Update	Position Rotation
		At	chest
		Bhvr	Behaviors\QuillProjectile4.bhvr
		BhvrOverride
			Scale		0.25 0.25 0.25
		End
		Geom	quill03
	End

	Event
		Type	Create
		Inherit	ALL
		Update	Position Rotation
		At	chest
		Bhvr	Behaviors\QuillProjectile4.bhvr
		BhvrOverride
			Scale		0.25 0.25 0.25
		End
		Geom	quill07
	End

	Event
		Type	Create
		Inherit	ALL
		Update	Position Rotation
		At	chest
		Bhvr	Behaviors\QuillProjectile4.bhvr
		Geom	quill06
		BhvrOverride
			Scale		0.25 0.25 0.25
		End
		Sound poisonhit2 50 50 .7
	End
End

#############################################################

Condition
	On 	Time
	Time 	0
	Repeat	6

	Event
		Type	Create
		Inherit	Position Scale
		Update	Position
		At	chest
		ChildFX	V_COV\PhysicsEnabled\PoisonSplash.fx
		Lifespan 75
	End

	Event
		HardwareOnly
		Type	Create
		Inherit	Position Scale
		Update	Position
		At	chest
		ChildFX	V_COV\PhysicsEnabled\PoisonSplash.fx
		Lifespan 75
	End
End

#############################################################

End