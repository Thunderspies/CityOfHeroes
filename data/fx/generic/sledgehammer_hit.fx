#########################################################
##	iceweapon
########################################################
FxInfo

Input  
	Inpname	Mystic
End


####################################

Lifespan 60

Condition
	On 	Time
	Time 	40

	Event
		EName 	Mallet
		Type	Local 
		At	Mystic
		Part1	POWERS\EarthControl\StalagtiteDustHit.part
		Part2	POWERS\EarthControl\StalagtiteDustHit2.part
		Part3	POWERS\EarthControl\WeaponRocks05.part
		Part4	POWERS\EarthControl\WeaponRocks05.part
		Sound sledge2 45 45 .55
		Sound sledge3 45 45 .5
		Sound sledge4 45 45 .54
	End


End




End
