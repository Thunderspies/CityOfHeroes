#############################################################
## Cripple_Continuing.fx
#############################################################

FxInfo

#########################################################



Condition
	On 	Time
	Time	0

	Event
		EName 	Prime
		Type	Local
		At	Chest
		Part	:Toxic_Splash_Continuing.part
		ChildFX	:Slime_UpperBody.fx
	End

	Event
		Type	Local
		At	Root
		Sound 	Hamidon_Cripple_Continuing_Loop 100 100 .3
	End


End

#########################################################

End