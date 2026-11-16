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
		Sound Fast2 70 70 .7
	End
End



#########################################################
#################### BODY GLOW 1 ########################
#########################################################



Condition
	On	cycle
	Time	5
	
	
	Event
		EName 	Prime2
		Type	Posit 
		At	HIPS
		Bhvr 	:SkillinShield.bhvr
		
		Geom	WispShield
		LifeSpan	15		
	End

End

Condition
	On	Time
	Time	5
	
	Event	
		ENAME	HeadGlow
		Type	Local
		At	Hair
		part	:Skillin.part
		part	:SkillinEnergy.part

		Bhvr	:Offset.bhvr
		
	End
	

	Event	
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		part	:Skillin.part
		part	:SkillinEnergy.part

		Bhvr	:Offset.bhvr
	End

	Event	
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part	:Skillin.part
		part	:SkillinEnergy.part

		Bhvr	:Offset.bhvr
		
	End


End

Condition
	On	Cycle
	Time	55
	Chance	.8

	Event	
		ENAME	HeadGlow
		Type	Local
		At	Head
#		part1	:SkillinHeadSpikes01.part		
		part1	:SkillinHeadSpikes02.part

		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 5
	
	End


End

Condition
	On	Cycle
	Time	45
	Chance	.8

	Event	
		ENAME	HeadGlow
		Type	Local
		At	Head
		part1	:SkillinHeadSpikes01.part		
#		part1	:SkillinHeadSpikes02.part

		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 5
	
	End


End

End		