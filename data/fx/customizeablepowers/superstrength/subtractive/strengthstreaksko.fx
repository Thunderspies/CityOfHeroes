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
		Part1	:StrengthStreak.part

		Sound PunchSSCustom 100.0 100.0 .85
	End

	Event
		EName	Streak1
		Type	local
		At	WepL
		Part1	:StrengthStreak.part

	End

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
	End

	Event
		EName	Streak1
		Type	local
		At	LarmL
		Part1	:StrengthBodyGlows.part
		PMagnet	WepL
	End

	Event
		EName	Streak1
		Type	local
		At	WepR
		Part1	:StrengthBodyGlowsSmall.part

	End

	Event
		EName	Streak1
		Type	local
		At	WepL
		Part1	:StrengthBodyGlowsSmall.part

	End

End

Condition
	On 	Time
	Time 	40

	Event
		EName	Streak1
		Type	local
		At	WepR

		Part2	:StrengthPulseKO.part
		Part3	:StrengthGlowKO.part

	End

	Event
		EName	Streak1
		Type	local
		At	WepL

		Part2	:StrengthPulseKO.part
		Part3	:StrengthGlowKO.part
	End
End


Condition
	On 	Time
	Time 	50

	Event
		EName	Streak1
		Type	local
		At	WepR
		Part2	:StrengthRingKO.part
	End

	Event
		EName	Streak1
		Type	local
		At	WepL

		Part2	:StrengthRingKO.part
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
