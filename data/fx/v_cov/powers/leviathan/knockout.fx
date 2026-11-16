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
		At	WepR
		Part1	:CloudTrail.part
		Sound PunchBB 100.0 100.0 .85
	End

	Event
		EName	Streak1
		Type	local
		At	WepL
		Part1	:CloudTrail.part

	End

	Event
		EName	Streak1
		Type	local
		At	Chest
		Part1	:StrengthBodyGlows.part

	End

	Event
		EName	Streak1
		Type	local
		At	Col_R
		Part1	:StrengthBodyGlows.part
		PMagnet	UarmR
	End

	Event
		EName	Streak1
		Type	local
		At	Col_R
		Part1	:StrengthBodyGlows.part
		PMagnet	UarmL
	End

	Event
		EName	Streak1
		Type	local
		At	UARMR
		Part1	:StrengthBodyGlows.part
		Part2	:StrengthBodyGlows2.part
		PMagnet	LarmR
	End

	Event
		EName	Streak1
		Type	local
		At	UARML
		Part1	:StrengthBodyGlows.part
		Part2	:StrengthBodyGlows2.part
		PMagnet	LarmL
	End

	Event
		EName	Streak1
		Type	local
		At	LarmR
		Part1	:StrengthBodyGlows.part
		Part2	:StrengthBodyGlows2.part
		PMagnet	WepR
	End

	Event
		EName	Streak1
		Type	local
		At	LarmL
		Part1	:StrengthBodyGlows.part
		Part2	:StrengthBodyGlows2.part
		PMagnet	WepL
	End
End

Condition
	On 	Time
	Time 	40

	Event
		EName	Streak1
		Type	local
		At	WepR
		part	:StrengthRingKO.part
		part 	:StrengthRingKO2.part


	End

End

Condition
	On	Time
	Time	50

	Event
		Type	local
		At 	WepR
		Part	:Lightbeams3.part

	End
End



Condition
	On 	Time
	Time 	90

	Event
		EName	All
		Type	Destroy

	End


End

End
##################################

