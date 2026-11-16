#########################################################
##	template

FxInfo
LifeSpan	105

Input
	Inpname chest
End


###################################################################################
####  On Victim FX ##################################################################
###################################################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Burst
		Type	Local 
		At	Waist

		bhvr	behaviors\FX_SnakeMen_HitOffset.bhvr
#		Part1	:SM_BiteHit.part
#		Part2	:SM_BiteHit1.part
#		Part3	:SM_BiteHit2.part
#		Part4	:SM_BiteRaysSmall.part
		
	End

End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Burst
		Type	Local 
		At	Waist

		bhvr	behaviors\FX_SnakeMen_HitOffset.bhvr
		Part4	:SM_PoisonSpark2.part
		Lifespan 5
	
	End

End


Condition
	On	Time
	Time	0

	Event	
		Type 	Local
		At	chest
		Sound snakehit3 60.0 60.0 .8
		Sound snakehit4b 60.0 60.0 .8
		Sound snakehit5 60.0 60.0 .8
		Sound snakehit3 60.0 60.0 .8

	End

End

###################################################################################
####  On Caster FX ##################################################################
###################################################################################

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
		At	T_chest
		Sound Snakebite3b 60.0 60.0 .78
		#Sound	Snakebite1 60.0 30.0 .78
		#Sound	Snakebite2 60.0 30.0 .78
		Sound Snakebite3b 60.0 60.0 .78
		Sound Snakebite4b 60.0 60.0 .69
		Sound Snakebite5b 60.0 60.0 .77
	End

End

########################### Ripped Particles ######################

Condition
	On	Time
	Time	0

	Event
		EName	DripEmit
		Type 	Local
		At	T_Head

		Geom	LocalPivotz
	End

End

Condition
	On	Time
	Time	29

	Event
		EName	Drip
		Type 	Local
		At	DripEmit
		 Altpiv 1

		part1	:SM_Splatter.part
		part1	:SM_Splatter2.part
		Lifespan 4
	End
End

Condition
	On	Time
	Time	30

	Event
		EName	Drip
		Type 	Local
		At	DripEmit
		 Altpiv 1

		part1	:SM_Splatter3.part
		Lifespan 3
	End
End



End

	

	