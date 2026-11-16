#########################################################
##	iceweapon
########################################################
FxInfo

Input
	Inpname	WepR
End




Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime8
		Type	Local
		At	WepR
		Bhvr	CustomizeablePowers\IceMelee\IceWeapon.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_FX_IceWeapon__Ice
		#SOund	coldswordout1 75 30 .8
	End


Event
		EName 	Next
		Type	Local
		At	Prime8
		 AltPiv 2

		Part2	:snowfall.part

		POther	Prime


	End



	Event
		EName 	Prime
		Type	Local
		At	Prime8
		 AltPiv 1
		Part1	:icesword2.part
		Part2	:icesword1Trail.part
		Part3	:icesword1.part
		Part4	:GreaterSwordtrail.part
		Part5	:Swordtrail2.part
		POther	Next
		#PMagnet	Next

	End

	Event
		EName 	glowhand
		Type	Local
		At	WepR
		Part1	:iceswordFistSumon.part
		SOund	coldswordout 75 30 .84

	End

End
Condition
	On 	Time
	Time 	50

	Event
		EName 	glowhand
		Type	Destroy


	End

End

Condition
	On 	Time
	Time 	5

	Event
		EName 	snowburst
		Type	LOCAL
		At	WepR
		Part1	:snowburst.part
		Sound cold_loop 70 70 .77

	End

	Event
		EName 	snowburstSkatter
		Type	LOCAL
		At	WepR
		Part1	:snowburstskatter.part
	End


End


End
