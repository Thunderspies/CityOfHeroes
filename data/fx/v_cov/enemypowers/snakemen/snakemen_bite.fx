#########################################################
##	template

FxInfo
LifeSpan	105

Input
	Inpname chest
End

#######################  sounds #########################

Condition
	On 	Time
	Time 	17

	Event
		EName 	Mallet
		Type	start 
		At	T_Chest
		#Sound	SwordStab1 100.0 30.0 .9

	End

End


Condition
	On	Time
	Time	0

	Event	
		EName	RattleSound
		Type 	Local
		At	chest
		Sound Snakebite3b 60.0 60.0 .78
		#Sound	Snakebite1 60.0 30.0 .78
		#Sound	Snakebite2 60.0 30.0 .78
		Sound Snakebite3b 60.0 60.0 .78
		Sound Snakebite4b 60.0 60.0 .69
		Sound Snakebite5b 60.0 60.0 .77
	End

End

########################### Ripped Particles (This is now in "SnakeMen_BiteHit.fx" - steve ######################
###
###Condition
###	On	Time
###	Time	0
###
###	Event
###		EName	DripEmit
###		Type 	Local
###		At	Head
###
###		Geom	LocalPivotz
###	End
###
###End
###
###Condition
###	On	Time
###	Time	29
###
###	Event
###		EName	Drip
###		Type 	Local
###		At	DripEmit
###		 Altpiv 1
###
###		part1	:SM_Splatter.part
###		part1	:SM_Splatter2.part
###		Lifespan 4
###	End
###End
###
###Condition
###	On	Time
###	Time	31
###
###	Event
###		EName	Drip
###		Type 	Local
###		At	DripEmit
###		 Altpiv 1
###
###		part1	:SM_Splatter3.part
###		Lifespan 3
###	End
###End


End

	

	