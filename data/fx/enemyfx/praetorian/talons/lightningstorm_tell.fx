#############################################################
## Judgment_Ion.fx
#############################################################

FxInfo
#LifeSpan	80

#############################################################

Input
	Inpname	Hips
End

#############################################################

Condition
	On	Time
	Time	0


	Event
		Type	Local
		At	Root
		Sound 	ToV_LightningStorm_Pet_Loop 100 100 .6
	End

	Event
		Type	Local
		At	Root
		Part	:Ring_Electric_Outer.part
		Part	:Ring_Electric_Outer_Highlight.part
	#	Lifespan 10
	End
End

#############################################################

End