#########################################################
##	Template

FxInfo
LifeSpan	1000

Condition
	On 	Time
	Time 	0

	Event
		EName	Offset
		Type	Local
		At	origin

		BhvrOverride
			PyrRotate	-15 -55 0
			PositionOffset	50 0 -10
		End

		Part	:RuneEnergy.part
		Part	:RuneTendrils.part
		Part	:RuneSparklies.part
	End

End

Condition
	On 	Time
	Time 	0

	Event
		EName	Offset
		Type	Local
		At	origin

		BhvrOverride
			PyrRotate	-15 -55 0
			PositionOffset	50 0 -10
		End

		Part	:TendrilEnergy.part
		Part	:LeviathanEmber.part
	End

End

######## GEOMETRY

#	Event
#		EName	Offset
#		Type	Local
#		At	root
#		Geom	_Rune_Destructible_Under_01
#	End
#
#	Event
#		EName	Offset
#		Type	Local
#		At	root
#		Geom	_Rune_Destructible_01
#	End

End
