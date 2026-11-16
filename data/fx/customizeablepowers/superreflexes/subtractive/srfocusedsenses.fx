#########################################################
##	template

FxInfo
#Lifespan 200


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
	On	Time
	Time	30


	Event
		ENAME	HeadGlow
		Type	Local
		At	Head
		part1	:SRFSHeadCircles01.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 60

	End


End

Condition
	On	Time
	Time	60


	Event
		ENAME	HeadGlow
		Type	Local
		At	Head
		part1	:SRFSHeadSpikes01.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 30

	End


End

Condition
	On	Cycle
	Time	120


	Event
		ENAME	HeadGlow
		Type	Local
		At	Head
		part1	:SRFSHeadSpikes02.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 5

	End


End


End