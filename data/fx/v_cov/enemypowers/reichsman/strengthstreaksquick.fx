#########################################################
##	Fireweapon
########################################################
FxInfo

Input
	Inpname	Mystic
End

Input
	Inpname	Chest
End

Input
	Inpname	WepL
End

Input
	Inpname	WepR
End

Input
	InpName	UARMR
End

Input
	InpName	UARML
End

Input
	InpName	LARML
End

Input
	InpName	LARMR
End

Input
	InpName	Col_L
End

Input
	InpName	Col_R
End

######################################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Streak1
		Type	local
		At	UARMR
		Part1	:StrengthBodyGlows.part
		PMagnet	LarmR
	End

	Event
		EName	Streak1
		Type	local
		At	UARML
		Part1	:StrengthBodyGlows.part
		PMagnet	LarmL
	End

	Event
		EName	Streak1
		Type	local
		At	LarmR
		Part1	:StrengthBodyGlows.part
		Part2	:StrengthStreak.part
		PMagnet	WepR
	End

	Event
		EName	Streak1
		Type	local
		At	LarmL
		Part1	:StrengthBodyGlows.part
		Part2	:StrengthStreak.part
		PMagnet	WepL
	End


	Event
		EName	Streak1
		Type	local
		At	WepR
		Part1	:StrengthStreak.part
		Part2	:StrengthGlow.part
		Sound ReichsmanStreaksLight_01 100.0 100.0 .55
		Sound ReichsmanStreaksLight_02 100.0 100.0 .55
		Sound ReichsmanStreaksLight_03 100.0 100.0 .55

	End

	Event
		EName	Streak1
		Type	local
		At	WepL
		Part1	:StrengthStreak.part
		Part2	:StrengthGlow.part
	End
End


Condition
	On 	Time
	Time 	30

	Event
		EName	All
		Type	Destroy

	End


End

End
##################################

