#########################################################
###  JackHammer Dust
########################################################
FxInfo

Lifespan	100

####################################

Condition
	On 	Time
	Time 	0


	Event
		EName	Prop
		Type	Local
		At	Mystic
		Geom	shovel_offset
		Lifespan	2

		End

End




Condition
	On 	Time
	Time 	0

	Event
		EName 	Dust
		Type	Local 
		At	Mystic
		Altpiv  1
		Part1	POWERS\EarthControl\StalagtiteDustHit.part
		Part2	POWERS\EarthControl\StalagtiteDustHit2.part
	End


End


	

End
