#############################################################
## SaltPileHit.fx
#############################################################

FxInfo
ClampMaxScale 5.0 5.0 5.0

#############################################################

Input
	Inpname	Hips
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName   saltburst
		Type	Create
		Inherit	ALL
		Update	Position
		At	Root
		Part	:SwirlingSalt.part
		Part	:SwirlingSaltFlat.part
		Part	:SwirlingSaltFlat2.part
	End

	Event
		Ename	Crystal
		Type	Create
		Inherit	ALL
		Update	Position
		At	Root
		Bhvr	behaviors\SaltScale.bhvr
		Geom	GEO_SaltPile
	End

	Event
		EName 	Stonesickle05
		Type	Create
		Inherit	ALL
		Update	Position
		At	root
		Bhvr	behaviors\SaltcrackScale.bhvr
		geom	Crack01
		Sound explohit 80 80 0.75
	End

	Event
		Ename	Saltpile
		Type	Create
		Inherit	ALL
		Update	Position
		At	Root
		Bhvr	behaviors\SaltScale.bhvr
		Geom	GEO_SaltCrystals
		Sound glow5_loop 50 50 0.25

	End
End

Condition
	On 	Cycle
	Time 	15
	Chance	0.1

	Event
		Type	Create
		Inherit	ALL
		Update	Position
		At	SaltPile
		Altpiv	2
		Part1	:SaltShineCore.part
		Part2	:SaltShineRings.part
		lifeSpan 5
	End
End

Condition
	On 	Cycle
	Time 	15
	Chance	0.1

	Event

		Type	Create
		Inherit	ALL
		Update	Position
		At	SaltPile
		Altpiv	3
		Part1	:SaltShineCore.part
		Part2	:SaltShineRings.part
		lifeSpan 5
	End
End

Condition
	On 	Cycle
	Time 	15
	Chance	0.1

	Event
		Type	Create
		Inherit	ALL
		Update	Position
		At	SaltPile
		Altpiv	4
		Part1	:SaltShineCore.part
		Part2	:SaltShineRings.part
		lifeSpan 5
	End
End

Condition
	On 	Cycle
	Time 	15
	Chance	0.1

	Event
		Type	Create
		Inherit	ALL
		Update	Position
		At	SaltPile
		Altpiv	5
		Part1	:SaltShineCore.part
		Part2	:SaltShineRings.part
		lifeSpan 5
	End
End

Condition
	On 	Cycle
	Time 	15
	Chance	0.1

	Event
		Type	Create
		Inherit	ALL
		Update	Position
		At	SaltPile
		Altpiv	6
		Part1	:SaltShineCore.part
		Part2	:SaltShineRings.part
		lifeSpan 5
	End
End

Condition
	On 	Cycle
	Time 	15
	Chance	0.1

	Event
		Type	Create
		Inherit	ALL
		Update	Position
		At	SaltPile
		Altpiv	7
		Part1	:SaltShineCore.part
		Part2	:SaltShineRings.part
		lifeSpan 5
	End
End

Condition
	On 	Cycle
	Time 	15
	Chance	0.1

	Event
		Type	Create
		Inherit	ALL
		Update	Position
		At	SaltPile
		Altpiv	8
		Part1	:SaltShineCore.part
		Part2	:SaltShineRings.part
		lifeSpan 5
	End
End

Condition
	On 	Cycle
	Time 	15
	Chance	0.1

	Event

		Type	Create
		Inherit	ALL
		Update	Position
		At	SaltPile
		Altpiv	9
		Part1	:SaltShineCore.part
		Part2	:SaltShineRings.part
		lifeSpan 5
	End
End

Condition
	On 	Cycle
	Time 	15
	Chance	0.1

	Event
		Type	Create
		Inherit	ALL
		Update	Position
		At	SaltPile
		Altpiv	10
		Part1	:SaltShineCore.part
		Part2	:SaltShineRings.part
		lifeSpan 5
	End
End

Condition
	On 	Cycle
	Time 	15
	Chance	0.1

	Event
		Type	Create
		Inherit	ALL
		Update	Position
		At	SaltPile
		Altpiv	11
		Part1	:SaltShineCore.part
		Part2	:SaltShineRings.part
		lifeSpan 5
	End
End

Condition
	On 	Cycle
	Time 	15
	Chance	0.1

	Event
		Type	Create
		Inherit	ALL
		Update	Position
		At	SaltPile
		Altpiv	12
		Part1	:SaltShineCore.part
		Part2	:SaltShineRings.part
		lifeSpan 5
	End
End

Condition
	On 	Cycle
	Time 	15
	Chance	0.1

	Event
		Type	Create
		Inherit	ALL
		Update	Position
		At	SaltPile
		Altpiv	13
		Part1	:SaltShineCore.part
		Part2	:SaltShineRings.part
		lifeSpan 5
	End
End

Condition
	On 	Cycle
	Time 	15
	Chance	0.1

	Event
		Type	Create
		Inherit	ALL
		Update	Position
		At	SaltPile
		Altpiv	14
		Part1	:SaltShineCore.part
		Part2	:SaltShineRings.part
		lifeSpan 5
	End
End

Condition
	On 	Cycle
	Time 	15
	Chance	0.1

	Event
		Type	Create
		Inherit	ALL
		Update	Position
		At	SaltPile
		Altpiv	1
		Part1	:SaltShineCore.part
		Part2	:SaltShineRings.part
		lifeSpan 5
	End
End

Condition
	On	Time
	Time	3

	Event
		Type	Create
		Inherit	ALL
		Update	Position
		At	Root
		Part2	:FossilCrack2a.part
	End
End

Condition
	On	Time
	Time	6

	Event
		Type	Create
		Inherit	ALL
		Update	Position
		At	Root
		Part2	:FossilCrack2a.part
	End
End

Condition
	On 	Death
	Repeat	25

	Event
		Type	Create
		Inherit	ALL
		Update	Position
		At	Root
		ChildFX		V_COV\PhysicsEnabled\SaltScatter.fx
		Lifespan	75
		LifespanJitter	15
	End

	Event
		HardwareOnly
		Type	Create
		Inherit	ALL
		Update	Position
		At	Root
		ChildFX		V_COV\PhysicsEnabled\SaltScatter.fx
		Lifespan	75
		LifespanJitter	15
	End
End

#############################################################

Condition
	On 	DEATH

	Event
		EName	All
		Type	Destroy
	End

	Event
		Ename	BlockofIceExplosion1
		Type	Create
		Inherit	ALL
		Update	Position
		At	Root
		Part1	:SaltBreakoutCrystal01.part
		Part2	:SaltBreakoutCrystal02.part
		Part	:SaltDust2.part
		Part	:SaltDust.part
		Part	:SaltDustWhite.part
		Part3	:SnowBreakout.part
		Sound SaltExplo 80 80 0.75
		lifespan 40
	End
End

#############################################################

End