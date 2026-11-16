#############################################################
## StrengthStreaks.fx
#############################################################

FxInfo
Lifespan 50

######################################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Streak1
		Type	local
		At	Chest
		Lifespan 40
	End

	Event
		EName	Streak1
		Type	local
		At	Col_R
		PMagnet	UarmR
		Lifespan 40
	End

	Event
		EName	Streak1
		Type	local
		At	Col_R
		PMagnet	UarmL
		Lifespan 40
	End

	Event
		EName	Streak1
		Type	local
		At	UARMR
		Part1	:StrengthBodyGlows.part
		PMagnet	LarmR
		Lifespan 40
	End

	Event
		EName	Streak1
		Type	local
		At	UARML
		Part1	:StrengthBodyGlows.part
		PMagnet	LarmL
		Lifespan 40
	End

	Event
		EName	Streak1
		Type	local
		At	LarmR
		Part1	:StrengthBodyGlows.part
		PMagnet	WepR
		Lifespan 40
	End

	Event
		EName	Streak1
		Type	local
		At	LarmL
		Part1	:StrengthBodyGlows.part
		PMagnet	WepL
		Lifespan 40
	End


	Event
		EName	Streak1
		Type	local
		At	WepR
		Part1	:StrengthStreak.part
		Part3	:StrengthGlow.part
		Sound PunchA 100.0 100.0 .85
		Lifespan 40
	End

	Event
		EName	Streak1
		Type	local
		At	WepL
		Part1	:StrengthStreak.part
		Part3	:StrengthGlow.part
		Lifespan 40
	End
End

##################################

End

