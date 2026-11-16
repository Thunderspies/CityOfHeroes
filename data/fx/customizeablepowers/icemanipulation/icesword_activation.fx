#############################################################
## IceSword_Activation.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Weapon
		Type	Local
		At	WepR
		Bhvr	CustomizeablePowers\IceManipulation\IceWeapon.bhvr
		BhvrOverride
			TintGeom	1
			Alpha		192
		End
		Geom	Tintable_FX_IceWeapon
	End

	Event
		EName 	Next
		Type	Local
		At	Weapon
		AltPiv	2
		Part2	CustomizeablePowers\IceManipulation\snowfall.part
		POther	Prime
	End

	Event
		EName 	Prime
		Type	Local
		At	Weapon
		AltPiv	1
		Part1	CustomizeablePowers\IceManipulation\icesword2.part
		Part4	CustomizeablePowers\IceManipulation\GreaterSwordtrail.part
		Part5	CustomizeablePowers\IceManipulation\Swordtrail2.part
		POther	Next
	End

	Event
		Type	Local
		At	WepR
		Part1	CustomizeablePowers\IceManipulation\iceswordFistSumon.part
		SOund	coldswordout 75 30 .84
		Lifespan 50
	End
End

#############################################################

Condition
	On 	Time
	Time 	5

	Event
		Type	Local
		At	WepR
		Part1	CustomizeablePowers\IceManipulation\snowburst.part
		Sound cold_loop 70 70 .77
	End

	Event
		Type	Local
		At	WepR
		Part1	CustomizeablePowers\IceManipulation\snowburstskatter.part
	End
End

#############################################################

End