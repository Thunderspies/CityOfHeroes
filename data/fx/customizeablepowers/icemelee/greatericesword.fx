#############################################################
## GreaterIceSword.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Offset
		Type	Local
		At	WepR
		Bhvr	:IceWeapon.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_GreaterIceWeapon
		SOund	coldswordout1 75 30 1.0
	End

	Event
		EName	Offset_Start
		Type	Local
		At	Offset
		AltPiv	2
		Part	:snowfall.part
		Part	:snowfallWhips.part
		Part	:SwordSparklies.part
		POther	Offset_End
	End

	Event
		EName	Offset_End
		Type	Local
		At	Offset
		AltPiv	1
		Part	:GreatericeswordFrost.part
		Part1	:Greatericesword2.part
		Part5	:GreaterSwordtrail2.part
		POther	Offset_Start
	End

	Event
		Type	Local
		At	WepR
		Part1	:iceswordFistSumon.part
		Sound coldswordout 75 75 0.9
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
		Part1	:snowburst.part
	End

	Event
		Type	Local
		At	WepR
		Part1	:snowburstskatter.part
		Sound cold_loop 70 70 0.77
	End
End

#############################################################

End