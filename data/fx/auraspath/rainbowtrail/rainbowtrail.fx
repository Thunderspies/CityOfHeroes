#############################################################
## FairyDust_Body.fx
#############################################################

FxInfo
Flags	DontSuppress

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Head
		Part	:Rainbow_Red.part
	End
	Event
		Type	Local
		At	Chest
		Part	:Rainbow_Yellow.part
		Part	:Rainbow_StreakFlares.part
	End
	Event
		Type	Local
		At	Hips
		Part	:Rainbow_Green.part
	End
	Event
		Type	Local
		At	Hips
		BhvrOverride
			PositionOffset	0 -1 0
		End
		Part	:Rainbow_Blue.part
	End
	Event
		Type	Local
		At	Hips
		BhvrOverride
			PositionOffset	0 -2 0
		End
		Part	:Rainbow_Violet.part
	End
	Event
		Type	Local
		At	Hips
		BhvrOverride
			PositionOffset	0 -0.5 0
		End
		Part	:Rainbow_Idle.part
		Pother	Chest
	End

End



#########################################################

End