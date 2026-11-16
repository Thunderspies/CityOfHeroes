#############################################################
## IceWeapon.fx
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
		Geom	Tintable_FX_IceWeapon
	End

	Event
		EName 	Offset_Start
		Type	Local
		At	Offset
		AltPiv	2
		Part	:snowfall.part
	End

	Event
		EName 	Offset_End
		Type	Local
		At	Offset
		AltPiv	1
		Part	:icesword2.part
		Part	:GreaterSwordtrail.part
		Part	:Swordtrail2.part
		POther	Offset_Start
	End

	Event
		Type	Local
		At	WepR
		Part	:iceswordFistSumon.part
		SOund	coldswordout 75 30 0.84
		Lifespan 50
	End
End

Condition
	On 	Time
	Time 	5

	Event
		EName 	snowburst
		Type	LOCAL
		At	WepR
		Part	:snowburst.part
		Sound cold_loop 70 70 0.77
	End

	Event
		EName 	snowburstSkatter
		Type	LOCAL
		At	WepR
		Part	:snowburstskatter.part
	End
End

#############################################################

End