#############################################################
## Judgment_Ion.fx
#############################################################

FxInfo
LifeSpan	90

#############################################################

Input
	Inpname	Hips
End

#############################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	FW_CapturedTalonsOfVengance_Static_01 50 50 .5
		Sound 	FW_CapturedTalonsOfVengance_Static_02 50 50 .5
		Sound 	FW_CapturedTalonsOfVengance_Static_03 50 50 .5
	End
End


Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Chest
		BhvrOverride
			PositionOffset	0.0 0 0.0
		End

		Part	:talons_sparks_Burst.part

		Lifespan	30
	End
End


End