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
		Sound 	FW_AwakenedIndoctrinate_Loop 50 50 .25
	End
End


Condition
	On 	Time
	Time 	0

	Event
		EName 	Bondage
		Type	Local
		At	HandL

		Part1	:IndoctrinateAwakened_LightningHand.part
		Part1	:IndoctrinateAwakened_GlowHand.part
#		Part1	:Shackle_Ring1.part
#		Part1	:Shackle_Ring2.part
		Lifespan	0
	End
End
Condition
	On 	Time
	Time 	0

	Event
		EName 	Bondage
		Type	Local
		At	HandR

		Part1	:IndoctrinateAwakened_LightningHand.part
		Part1	:IndoctrinateAwakened_GlowHand.part
#		Part1	:Shackle_Ring1.part
#		Part1	:Shackle_Ring2.part
		Lifespan	0
	End
End


#############################################################

End