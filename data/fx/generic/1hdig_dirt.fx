#########################################################
##	
########################################################
FxInfo

Lifespan	32

######### Right hand dirt cloud  ###########################

Condition
	On 	Time
	Time 	16

	Event
		EName 	Dust
		Type	Local 
		At	HandR
		Part1	POWERS\EarthControl\StalagtiteDustHit.part
		Part2	POWERS\EarthControl\StalagtiteDustHit2.part
		# SoundNoRepeat 3
		Sound Hand_Dig_01 50 50 .1
		Sound Hand_Dig_02 50 50 .1
		Sound Hand_Dig_03 50 50 .1
		Sound Hand_Dig_04 50 50 .1
		Sound Hand_Dig_05 50 50 .1
		Sound Hand_Dig_06 50 50 .1
		Sound Hand_Dig_07 50 50 .1
		LifeSpan 19
	End


End

######### Right hand dirt flying clump  ###########################

Condition
	On 	Time
	Time 	22

	Event
		EName 	Dust
		Type	Local 
		At	WepR
		Part1	POWERS\EarthControl\StalagtiteDustHit.part
		Part2	POWERS\EarthControl\StalagtiteDustHit2.part
		LifeSpan 19
	End


End

	

End
