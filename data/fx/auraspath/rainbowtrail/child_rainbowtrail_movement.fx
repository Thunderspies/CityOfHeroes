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
		Part	:Rainbow_Red_MOVEMENT.part
	End

	Event
		Type	Local
		At	Chest
		Part	:Rainbow_Yellow_MOVEMENT.part
	End

	Event
		Type	Local
		At	Hips
		Part	:Rainbow_Green_MOVEMENT.part
	End

	Event
		Type	Local
		At	Hips
		BhvrOverride
			PositionOffset	0 -1 0
		End
		Part	:Rainbow_Blue_MOVEMENT.part
	End

	Event
		Type	Local
		At	Hips
		BhvrOverride
			PositionOffset	0 -2 0
		End
		Part	:Rainbow_Violet_MOVEMENT.part
	End
End



#########################################################

End