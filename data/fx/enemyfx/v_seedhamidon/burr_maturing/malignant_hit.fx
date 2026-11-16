#############################################################
## Fault.fx
#############################################################

FxInfo
Lifespan 30

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Root
		Sound 	HamidonSeed_Malignant_Hit_01 100 100 .8
	End

	Event
		Type	Local
		At	Chest
		Part	:Malignant_HitSplatter_Mult.part
		Part	:Malignant_Splatter_Spray.part
		PArt	:Malignant_HitFlash.part
		Lifespan 5
	End
End

#############################################################

End