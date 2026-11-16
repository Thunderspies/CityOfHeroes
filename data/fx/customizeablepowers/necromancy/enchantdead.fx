#############################################################
## EnchantDead.fx
#############################################################


FxInfo
LifeSpan	200


############################################################

Condition
	On	Time
	Time	0

	Event

		ENAME	Base
		Type	local
		At	WepR
		Sound darkwindup2 60 60 .3
		Part1	CustomizeablePowers\Necromancy\DarkHand.part
		Part3	CustomizeablePowers\Necromancy\DarkHandsEmber.part
		LifeSpan	50

	End

	Event

		ENAME	Base
		Type	local
		At	WepL

		Part1	CustomizeablePowers\Necromancy\DarkHand.part
		Part3	CustomizeablePowers\Necromancy\DarkHandsEmber.part
		LifeSpan	50
	End

End

Condition
	On 	Time
	Time 	20

	Event
		EName 	core
		Type	local
		At	T_Root
		geom	centerdummy
		bhvr	CustomizeablePowers\Necromancy\Smoke_spinFast.bhvr

		lifeSpan	260

	End

	Event
		EName 	Orb
		Type	Local
		At	core
		altpiv	1
		part	CustomizeablePowers\Necromancy\DarkessChillMistTrail.part
		part	CustomizeablePowers\Necromancy\DarkessChillMistTrail2.part
		Sound miasma_loop 100 100 .5

		lifeSpan	80

	End

	Event
		EName 	Orb
		Type	Local
		At	T_Root
		part	CustomizeablePowers\Necromancy\DarkStreaks.part

		lifeSpan	80

	End

	Event

		ENAME	Base
		Type	start
		At	T_Root

		Bhvr	CustomizeablePowers\Necromancy\DarkScale3.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_FX_DarkWave
		Sound miasmaburst 100 100 .6

		lifeSpan	110
	End

	Event

		ENAME	BaseTrail
		Type	start
		At	T_Root

		Part1	CustomizeablePowers\Necromancy\CreateDarknessTrail.part
		Part2	CustomizeablePowers\Necromancy\CreateDarknessTrail2.part
		Sound miasma_loop 100 100 .5

		lifeSpan	80

	End

	Event
		EName	darkskirt
		Type	Start
		At	T_Root

		Part	CustomizeablePowers\Necromancy\CreateDarknessPuddleFlat.part
		Part	CustomizeablePowers\Necromancy\CreateDarknessPuddle2Flat.part
		Part	CustomizeablePowers\Necromancy\CreateDarknessPuddle.part
		Part	CustomizeablePowers\Necromancy\CreateDarknessPuddle2.part

		lifeSpan	80
	End

End

#############################################################################################################################

End