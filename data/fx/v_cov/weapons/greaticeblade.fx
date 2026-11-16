#########################################################
##	iceweapon
########################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Poo
		Type	Local
		At	WepR
		Bhvr	behaviors/powers/coldcontrol/IceWeapon.bhvr
		Geom	GreatIceBlade
		SOund	coldswordout1 75 30 1.0
	End


	Event
		EName 	Next
		Type	Local
		At	Poo
		 AltPiv 2

		Part2	POWERS\ColdControl\snowfall.part
		Part	POWERS\ColdControl\snowfallWhips.part
		Part	POWERS\ColdControl\SwordSparklies.part
		POther	node


	End

	Event
		EName 	node
		Type	Local
		At	Poo
		 AltPiv 1
		Part	POWERS\ColdControl\GreatericeswordFrost.part
		Part1	POWERS\ColdControl\Greatericesword2.part
		Part2	POWERS\ColdControl\Greatericesword1Trail.part
		Part3	POWERS\ColdControl\Greatericesword1.part
		Part4	POWERS\ColdControl\GreaterSwordtrail.part
		Part5	POWERS\ColdControl\GreaterSwordtrail2.part
		POther	Next
		PMagnet	Next

	End

	Event
		EName 	node2
		Type	Local
		At	Poo
		 AltPiv 3
		Part1	POWERS\ColdControl\icesword2.part
		Part2	POWERS\ColdControl\icesword1Trail.part
		Part3	POWERS\ColdControl\icesword1.part
		Part4	POWERS\ColdControl\GreaterSwordtrail.part
		Part5	POWERS\ColdControl\Swordtrail2.part
		POther	Next2
		PMagnet	Next2

	End

	Event
		EName 	Next2
		Type	Local
		At	Poo
		 AltPiv 4

		Part2	POWERS\ColdControl\snowfall.part
		Part	POWERS\ColdControl\snowfallWhips.part
		Part	POWERS\ColdControl\SwordSparklies.part
		POther	node2
	End

	Event
		EName 	glowhand
		Type	Local
		At	WepR
		Part1	POWERS\ColdControl\iceswordFistSumon.part
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
		Part1	POWERS\ColdControl\snowburst.part

	End

	Event
		EName 	snowburstSkatter
		Type	LOCAL
		At	WepR
		Part1	POWERS\ColdControl\snowburstskatter.part
		Sound cold_loop 70 70 .77
	End

	Event
		EName 	snow1
		Type	LOCAL
		At	Poo
		  AltPiv 4
		Part1	POWERS\ColdControl\snowburst.part
		Part2	POWERS\ColdControl\snowburstskatter.part
		Part2	POWERS\ColdControl\snowburstskatter.part
	End

	Event
		EName 	snow2
		Type	LOCAL
		At	Poo
		  AltPiv 1
		Part1	POWERS\ColdControl\snowburst.part
		Part2	POWERS\ColdControl\snowburstskatter.part
		Part2	POWERS\ColdControl\snowburstskatter.part
	End


	Event
		EName 	snow3
		Type	LOCAL
		At	Poo
		  AltPiv 3
		Part1	POWERS\ColdControl\snowburst.part
		#Part2	POWERS\ColdControl\snowburstskatter.part
	End

	Event
		EName 	snow4
		Type	LOCAL
		At	Poo
		  AltPiv 2
		Part1	POWERS\ColdControl\snowburst.part
		#Part2	POWERS\ColdControl\snowburstskatter.part
	End


End


End
