#############################################################
## IndoctrinateAwakened.fx
#############################################################

FxInfo

#LifeSpan 500

## FX ON PENELOPE ###########################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	FW_AwakenedFeralInsanity_Loop 50 50 .5
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Bondage
		Type	Local
		At	Hips

		Part1	:Awakened_Feral_Insanity_Lightning.part
		Part1	:Awakened_Feral_Insanity_LightningRound.part
		Part1	:Awakened_Feral_Insanity_Glow.part
		Lifespan	0
	End
End



#############################################################

End