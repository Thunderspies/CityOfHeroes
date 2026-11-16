#########################################################
##
########################################################
FxInfo

Lifespan	1

####################################

Condition
	On 	Time
	Time 	0


	Event
		EName	Offset
		Type	Local
		At	WepR
		BhvrOverride
			PositionOffset		0.0 0.0 4.75
		End
		Lifespan	1
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Dust
		Type	Local
		At	Offset
		Part1	POWERS\EarthControl\StalagtiteDustHit.part
		Part2	POWERS\EarthControl\StalagtiteDustHit2.part
	End
End

########################################################

End