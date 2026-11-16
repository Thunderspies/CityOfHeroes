#########################################################
##	Fireweapon
########################################################
FxInfo
Lifespan 40

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
		PMagnet	WepR
		Sound ReichsmanStreaksMed_01 100.0 100.0 .55
		Sound ReichsmanStreaksMed_02 100.0 100.0 .55
		Sound ReichsmanStreaksMed_03 100.0 100.0 .55

	End

	Event
		EName	Streak1
		Type	local
		At	LarmL
		Part1	:StrengthBodyGlows.part
		PMagnet	WepL
	End

End

Condition
	On 	Time
	Time 	10

	Event
		EName	Streak1
		Type	local
		At	WepR
		Part1	:StrengthStreak.part
		Part3	:StrengthGlow.part
	End

	Event
		EName	Streak1
		Type	local
		At	WepL
		Part1	:StrengthStreak.part
		Part3	:StrengthGlow.part
	End
End

##################################

