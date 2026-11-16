#########################################################
##	chill touch hit
FxInfo

LifeSpan		50

##################################
##Sound
########################################################################









###########################################################################################

Condition
		
	Event	
		Ename   targetchill
		Type	Local
		At	neck
		BHVR	Behaviors/GenericParticleFade.bhvr
		Part	:WindCloudsDownWarm.part
		Part	:WindClouds2DownWarm.part
		LifeSpan	20
	End
		

End



Condition
	On	Time
	Time	15

	Event	
		Ename   targetchill
		Type	Local
		At	chest
		BHVR	Behaviors/GenericParticleFade.bhvr
		Part1	:IceChunks.part
		Part2	:IceChunksLarge.part
		LifeSpan	15
	End

End

Condition
	On	Time
	Time	25

	Event	
		Ename   targetchill
		Type	Local
		At	Root
		BHVR	Behaviors/GenericParticleFade.bhvr
		Part	:WindCloudsLargeWarm.part
		Part	:WindClouds2LargeWarm.part
		LifeSpan	30
	End

End


End				