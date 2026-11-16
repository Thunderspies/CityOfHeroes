#########################################################
##	chill touch hit
FxInfo

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
		Part	:WindCloudsDownCont.part
		Part	:WindClouds2DownCont.part
		#LifeSpan	20
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
		Part1	:IceChunksCont.part
		Part2	:IceChunksLargeCont.part
		#LifeSpan	15
	End

End


End				