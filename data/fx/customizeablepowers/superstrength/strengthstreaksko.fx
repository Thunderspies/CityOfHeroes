#############################################################
## StrengthStreaksKO.fx
#############################################################

FxInfo

Lifespan 90

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	WepR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\SuperStrength\StrengthStreak.part
		Sound PunchSSCustom 100.0 100.0 0.85
	End

	Event
		Type	local
		At	WepL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\SuperStrength\StrengthStreak.part
	End

	Event
		Type	local
		At	UARMR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\SuperStrength\StrengthBodyGlows.part
		PMagnet	LarmR
	End

	Event
		Type	local
		At	UARML
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\SuperStrength\StrengthBodyGlows.part
		PMagnet	LarmL
	End

	Event
		Type	local
		At	LarmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\SuperStrength\StrengthBodyGlows.part
		PMagnet	WepR
	End

	Event
		Type	local
		At	LarmL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\SuperStrength\StrengthBodyGlows.part
		PMagnet	WepL
	End

	Event
		Type	local
		At	WepR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\SuperStrength\StrengthBodyGlowsSmall.part
	End

	Event
		Type	local
		At	WepL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\SuperStrength\StrengthBodyGlowsSmall.part
	End
End

#############################################################

Condition
	On 	Time
	Time 	40

	Event
		EName	Streak1
		Type	local
		At	WepR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part2	CustomizeablePowers\SuperStrength\StrengthPulseKO.part
		Part3	CustomizeablePowers\SuperStrength\StrengthGlowKO.part
	End

	Event
		EName	Streak1
		Type	local
		At	WepL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part2	CustomizeablePowers\SuperStrength\StrengthPulseKO.part
		Part3	CustomizeablePowers\SuperStrength\StrengthGlowKO.part
	End
End

Condition
	On 	Time
	Time 	50

	Event
		EName	Streak1
		Type	local
		At	WepR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part2	CustomizeablePowers\SuperStrength\StrengthRingKO.part
	End

	Event
		EName	Streak1
		Type	local
		At	WepL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part2	CustomizeablePowers\SuperStrength\StrengthRingKO.part
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
		Lifespan 1
	End
End

#############################################################

End