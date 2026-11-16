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
		EName 	Sword
		Type	Local
		At	WepR
		Bhvr	behaviors/powers/coldcontrol/IceWeapon.bhvr
		Geom	FX_IceWeapon
		#SOund	coldswordout1 75 30 .8
	End


	Event
		EName 	Next
		Type	Local
		At	Sword
		AltPiv 	1
		Part2	powers/coldcontrol/snowfall.part
		POther	Prime
	End



	Event
		EName 	Prime
		Type	Local
		At	Sword
		AltPiv 	2
		Part1	:icesword2.part
		Part2	:icesword1Trail.part
		Part3	:icesword1.part
		Part4	:GreaterSwordtrail.part
		Part5	:Swordtrail2.part
		POther	Next
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
