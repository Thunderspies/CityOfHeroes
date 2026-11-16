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
		EName 	Poo
		Type	Local
		At	WepR
		Bhvr	behaviors/powers/coldcontrol/IceWeapon.bhvr
		Geom	GreaterIceWeapon
		SOund	coldswordout1 75 30 1.0
	End


	Event
		EName 	Next
		Type	Local
		At	Poo
		 AltPiv 2

		Part2	:Sword_Snowfall.part
		Part	:Sword_Mist_Detached.part
		Part	:Sword_Sparkles.part
		POther	node


	End


	Event
		EName 	node
		Type	Local
		At	Poo
		 AltPiv 1
		Part	:Sword_Frost.part
		Part4	:Sword_Mist.part
		Part5	:Sword_Mist_Trail.part
		POther	Next
		#PMagnet	Next

	End

	Event
		EName 	glowhand
		Type	Local
		At	WepR
	#	Part1	:iceswordFistSumon.part
		Sound coldswordout 75 75 .9

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
	#	Part1	:snowburst.part

	End

	Event
		EName 	snowburstSkatter
		Type	LOCAL
		At	WepR
	#	Part1	:snowburstskatter.part
		Sound cold_loop 70 70 .77
	End


End


End
