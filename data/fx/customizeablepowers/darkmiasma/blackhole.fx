#############################################################
## HEADER
#############################################################

FxInfo

ClampMaxScale 1.0 1.0 1.0

LifeSpan 180

#############################################################

Condition
	On	Time
	Time	0

	Event

		ENAME	Base
		Type	start
		At	T_root

		Part3	CustomizeablePowers\DarkMiasma\BlackHoleRadiusRing.Part
		Part5	CustomizeablePowers\DarkMiasma\BlackHoleRadiusRingThin.Part
		Sound Blackhole4 80 80 .75
	End
End

Condition
	On	Time
	Time	15

	Event
		ENAME	BaseTrail
		Type	start
		At	T_root

		Part3	CustomizeablePowers\DarkMiasma\BlackHolePuddle.part
		Part4	CustomizeablePowers\DarkMiasma\BlackHolePuddle2.part
		Part5	CustomizeablePowers\DarkMiasma\BlackHoleTendrill.Part
	End
End

Condition
	On	Time
	Time	20

	Event
		ENAME	Base1
		Type	local
		At	WepR
		Part1	CustomizeablePowers\DarkMiasma\DarkHand.part
		Part3	CustomizeablePowers\DarkMiasma\DarkHandsEmber.part
		Lifespan 90
	End

	Event

		ENAME	Base2
		Type	local
		At	WepL
		Part1	CustomizeablePowers\DarkMiasma\DarkHand.part
		Part3	CustomizeablePowers\DarkMiasma\DarkHandsEmber.part
		Lifespan 90
	End
End

#############################################################

End