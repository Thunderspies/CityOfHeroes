#########################################################
##	template

FxInfo


#########################################################
####################### AUDIO ###########################
#########################################################

Condition
	On	Time
	Time	0
	

	Event
		Type	Local
		At 	Origin
		Sound Fast 80 80 .8
	End
End

#########################################################
#################### BODY GLOW 1 ########################
#########################################################



Condition
	On	Cycle
	Time	30
	Chance	.7
	

	Event	
		ENAME	HeadGlow
		Type	Local
		At	Head
		part1	:HeadCircles01.part
		part1	:HeadCirclesDark.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 30
	
	End


End

Condition
	On	Time
	Time	60
	

	Event	
		ENAME	HeadGlow
		Type	Local
		At	Head
		part1	:HeadSpikes01.part
		part1	:HeadSpikesDark01.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 30
	
	End


End

Condition
	On	Cycle
	Time	60
	Chance	.7
	

	Event	
		ENAME	HeadGlow
		Type	Local
		At	Head
		part1	:HeadSpikes02.part
		part1	:HeadSpikesDark02.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 25
	
	End


End


End		