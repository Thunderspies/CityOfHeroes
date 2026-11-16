#########################################################
##	chill touch hit
FxInfo

LifeSpan		30

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
		Part	:WindCloudsDown.part
		Part	:WindClouds2Down.part
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
		Part	:WindCloudsLarge.part
		Part	:WindClouds2Large.part
		LifeSpan	30
	End

End

Condition
	On	Time
	Time	0

	Event	
		Type	Local
		At	Root
		BHVR	:EndSplat.BHVR
		splat	generic_Ring.tga
		LifeSpan	40
	End
End

Condition
	On	Time
	Time	10

	Event	
		Type	Local
		At	Root
		BHVR	:EndSplat2.BHVR
		splat	generic_Ring.tga
		LifeSpan	20
	End
End

End				