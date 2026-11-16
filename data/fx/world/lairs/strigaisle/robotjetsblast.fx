#########################################################
##	Inferno FX
##
FxInfo
	
lifespan	900

##########################################################################################################
##Final Blast
######################################################################################3

Condition
	On 	Time
	Time 	27

	Event
		EName 	Prime
		Type	Local 
		At	hips
		
		
		Part	:Inferno.part
		LifeSpan	10
		Sound inferno4 100 100 .9

	End

End

Condition
	On 	Time
	Time 	31

	Event
		EName 	Primex
		Type	Local 
		At	hips
		Bhvr	Behaviors/CameraShake01.bhvr
		Part	:InfernorockBits.part		
		Part	:InfernoExplosion.part
		LifeSpan	40
	End



	Event
		EName   RadiusFxScale
		Type	local
		At	Root
		Bhvr	:Inferno.bhvr
	End


	Event
		EName 	Prime1
		Type	Local 
		At	RadiusFxScale
		bhvr    :Infernobubble.bhvr
		Part	:InfernoYellow2.part
		Part	:InfernoYellowFlame01.part
		
	End		

End

Condition
	On 	Time
	Time 	0
	Event
		EName 	Prime8
		Type	Local 
		At	RadiusFxScale
		Part   :Infernoflash.part
		
	End
End


End
