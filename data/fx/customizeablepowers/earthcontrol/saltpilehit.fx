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
		EName 	Cracks
		Type	Create
		Inherit	ALL
		Update	Position
		At	root
		Bhvr	behaviors\SaltcrackScale.bhvr
		geom	Crack01
		Sound explohit 80 80 0.75
	End

	Event
		Ename	SaltPile
		Type	Create
		Inherit	ALL
		Update	Position
		At	Root
		Bhvr	behaviors\SaltScale.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_GEO_SaltPile
	End

	Event
		Ename	Crystals
		Type	Create
		Inherit	ALL
		Update	Position
		At	Root
		Bhvr	behaviors\SaltScale.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_GEO_SaltCrystals
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
		At	Crystals
		Altpiv	2
		Part1	POWERS\EarthControl\SaltShineCore.part
		Part2	POWERS\EarthControl\SaltShineRings.part
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
		At	Crystals
		Altpiv	3
		Part1	POWERS\EarthControl\SaltShineCore.part
		Part2	POWERS\EarthControl\SaltShineRings.part
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
		At	Crystals
		Altpiv	4
		Part1	POWERS\EarthControl\SaltShineCore.part
		Part2	POWERS\EarthControl\SaltShineRings.part
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
		At	Crystals
		Altpiv	5
		Part1	POWERS\EarthControl\SaltShineCore.part
		Part2	POWERS\EarthControl\SaltShineRings.part
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
		At	Crystals
		Altpiv	6
		Part1	POWERS\EarthControl\SaltShineCore.part
		Part2	POWERS\EarthControl\SaltShineRings.part
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
		At	Crystals
		Altpiv	7
		Part1	POWERS\EarthControl\SaltShineCore.part
		Part2	POWERS\EarthControl\SaltShineRings.part
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
		At	Crystals
		Altpiv	8
		Part1	POWERS\EarthControl\SaltShineCore.part
		Part2	POWERS\EarthControl\SaltShineRings.part
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
		At	Crystals
		Altpiv	9
		Part1	POWERS\EarthControl\SaltShineCore.part
		Part2	POWERS\EarthControl\SaltShineRings.part
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
		At	Crystals
		Altpiv	10
		Part1	POWERS\EarthControl\SaltShineCore.part
		Part2	POWERS\EarthControl\SaltShineRings.part
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
		At	Crystals
		Altpiv	11
		Part1	POWERS\EarthControl\SaltShineCore.part
		Part2	POWERS\EarthControl\SaltShineRings.part
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
		At	Crystals
		Altpiv	12
		Part1	POWERS\EarthControl\SaltShineCore.part
		Part2	POWERS\EarthControl\SaltShineRings.part
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
		At	Crystals
		Altpiv	13
		Part1	POWERS\EarthControl\SaltShineCore.part
		Part2	POWERS\EarthControl\SaltShineRings.part
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
		At	Crystals
		Altpiv	14
		Part1	POWERS\EarthControl\SaltShineCore.part
		Part2	POWERS\EarthControl\SaltShineRings.part
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
		At	Crystals
		Altpiv	1
		Part1	POWERS\EarthControl\SaltShineCore.part
		Part2	POWERS\EarthControl\SaltShineRings.part
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
		ChildFX		:SaltScatter.fx
		Lifespan	75
		LifespanJitter	15
	End

	Event
		HardwareOnly
		Type	Create
		Inherit	ALL
		Update	Position
		At	Root
		ChildFX		:SaltScatter.fx
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
		EName	Cracks
		Type	Destroy
	End

	Event
		EName	SaltPile
		Type	Destroy
	End

	Event
		EName	Crystals
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