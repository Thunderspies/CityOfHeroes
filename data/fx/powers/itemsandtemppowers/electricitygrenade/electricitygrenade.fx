#########################################################
##	forceBubble
########################################################
FxInfo

LifeSpan	10

#################################################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	lightning
		Type	local
		At	root
		Part	:ChainLightningArchs3.part
		Part	:ChainLightningArchs2.part
		Part	:ChainLightningArchs1.part
		Part	:ChainLightningArchs.part
		Part	:ChainLightningArchsSpinner.part
		Part	:ChainLightningRing2.part
		Part	:ChainLightningArchsInverted.part
		
		Sound ChainLight2_loop 100 100 .8
	
	End

	
End

End