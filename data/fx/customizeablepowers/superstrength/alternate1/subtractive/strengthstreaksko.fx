#############################################################
## StrengthStreaksKO.fx
#############################################################

FxInfo
Lifespan 90

######################################################################

Condition
	On 	Time
	Time 	0


	Event
		EName	Streak1
		Type	local
		At	WepR
		Part1	CustomizeablePowers\SuperStrength\Subtractive\StrengthStreak.part

		Sound PunchSSCustom 100.0 100.0 .85
	End

	Event
		EName	Streak1
		Type	local
		At	WepL
		Part1	CustomizeablePowers\SuperStrength\Subtractive\StrengthStreak.part

	End

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
		Part1	CustomizeablePowers\SuperStrength\Subtractive\StrengthBodyGlowsSmall.part

	End

	Event
		EName	Streak1
		Type	local
		At	WepL
		Part1	CustomizeablePowers\SuperStrength\Subtractive\StrengthBodyGlowsSmall.part

	End

End

Condition
	On 	Time
	Time 	40

	Event
		EName	Streak1
		Type	local
		At	WepR

		Part2	CustomizeablePowers\SuperStrength\Subtractive\StrengthPulseKO.part
		Part3	CustomizeablePowers\SuperStrength\Subtractive\StrengthGlowKO.part

	End

	Event
		EName	Streak1
		Type	local
		At	WepL

		Part2	CustomizeablePowers\SuperStrength\Subtractive\StrengthPulseKO.part
		Part3	CustomizeablePowers\SuperStrength\Subtractive\StrengthGlowKO.part
	End
End


Condition
	On 	Time
	Time 	40

	Event
		EName	Streak1
		Type	local
		At	WepR
		Part2	CustomizeablePowers\SuperStrength\Subtractive\StrengthRingKO.part
	End

	Event
		EName	Streak1
		Type	local
		At	WepL

		Part2	CustomizeablePowers\SuperStrength\Subtractive\StrengthRingKO.part
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	Target
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		15
			PhysForcePower		500
			PhysForcePowerJitter	100
		End

		Lifespan	1
	End
End
###################
End
