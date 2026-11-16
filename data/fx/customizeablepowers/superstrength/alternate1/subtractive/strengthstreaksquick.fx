#############################################################
## StrengthStreaksQuick.fx
#############################################################


FxInfo
Lifespan 30

######################################################################

Condition
	On 	Time
	Time 	0


	Event
		EName	Streak1
		Type	local
		At	UARMR
		Part1	CustomizeablePowers\SuperStrength\Subtractive\StrengthBodyGlows.part
		PMagnet	LarmR
	End

	Event
		EName	Streak1
		Type	local
		At	UARML
		Part1	CustomizeablePowers\SuperStrength\Subtractive\StrengthBodyGlows.part
		PMagnet	LarmL
	End

	Event
		EName	Streak1
		Type	local
		At	LarmR
		Part1	CustomizeablePowers\SuperStrength\Subtractive\StrengthBodyGlows.part
		PMagnet	WepR
	End

	Event
		EName	Streak1
		Type	local
		At	LarmL
		Part1	CustomizeablePowers\SuperStrength\Subtractive\StrengthBodyGlows.part
		PMagnet	WepL
	End


	Event
		EName	Streak1
		Type	local
		At	WepR
		Part1	CustomizeablePowers\SuperStrength\Subtractive\StrengthStreak.part
		Part3	CustomizeablePowers\SuperStrength\Subtractive\StrengthGlow.part
		Sound PunchASSCustom 100.0 100.0 .85
	End

	Event
		EName	Streak1
		Type	local
		At	WepL
		Part1	CustomizeablePowers\SuperStrength\Subtractive\StrengthStreak.part
		Part3	CustomizeablePowers\SuperStrength\Subtractive\StrengthGlow.part
	End
End

##################################

End

