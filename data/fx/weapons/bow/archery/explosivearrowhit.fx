#########################################################
##	Inferno FX
##
FxInfo
	
lifespan	20

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local 
		At	root
		
		BHVR	Behaviors/RootToChestAdjustment.bhvr
		Part	:InfernoSmall.part
		#Part	:InfernoFlat.part
		Sound inferno4 100 100 .9

	End

	Event
		EName 	Primex
		Type	Local 
		At	root
		Bhvr	:CameraShake01.bhvr
		Part	:InfernorockBits.part		
		Part	:InfernoExplosion.part
		Part	:InfernoExplosionFlat.part
	
		Lifespan	15

	End



End
