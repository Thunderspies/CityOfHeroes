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
		Bhvr	CustomizeablePowers\IcyAssault\IceWeapon.bhvr
		Geom	GreaterIceWeapon
		SOund	coldswordout1 75 30 1.0
	End


	Event
		EName 	Next
		Type	Local
		At	Poo
		 AltPiv 2

		Part2	CustomizeablePowers\IcyAssault\snowfall.part
		Part	CustomizeablePowers\IcyAssault\snowfallWhips.part
		Part	CustomizeablePowers\IcyAssault\SwordSparklies.part
		POther	node


	End


	Event
		EName 	node
		Type	Local
		At	Poo
		 AltPiv 1
		Part	CustomizeablePowers\IcyAssault\GreatericeswordFrost.part
		Part1	CustomizeablePowers\IcyAssault\Greatericesword2.part
		Part2	CustomizeablePowers\IcyAssault\Greatericesword1Trail.part
		Part3	CustomizeablePowers\IcyAssault\Greatericesword1.part
		Part4	CustomizeablePowers\IcyAssault\GreaterSwordtrail.part
		Part5	CustomizeablePowers\IcyAssault\GreaterSwordtrail2.part
		POther	Next
		#PMagnet	Next

	End

	Event
		EName 	glowhand
		Type	Local
		At	WepR
		Part1	CustomizeablePowers\IcyAssault\iceswordFistSumon.part
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
		Part1	CustomizeablePowers\IcyAssault\snowburst.part

	End

	Event
		EName 	snowburstSkatter
		Type	LOCAL
		At	WepR
		Part1	CustomizeablePowers\IcyAssault\snowburstskatter.part
		Sound cold_loop 70 70 .77
	End


End


End
