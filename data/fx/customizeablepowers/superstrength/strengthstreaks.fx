#############################################################
## StrengthStreaks.fx
#############################################################

FxInfo

Lifespan 40

#########################################################

Input
	Inpname	Hips
End

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Streak1
		Type	local
		At	UARMR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\SuperStrength\StrengthBodyGlows.part
		PMagnet	LarmR
	End

	Event
		EName	Streak1
		Type	local
		At	UARML
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\SuperStrength\StrengthBodyGlows.part
		PMagnet	LarmL
	End

	Event
		EName	Streak1
		Type	local
		At	LarmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\SuperStrength\StrengthBodyGlows.part
		PMagnet	WepR
	End

	Event
		EName	Streak1
		Type	local
		At	LarmL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\SuperStrength\StrengthBodyGlows.part
		PMagnet	WepL
	End

	Event
		EName	Streak1
		Type	local
		At	WepR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\SuperStrength\StrengthStreak.part
		Part3	CustomizeablePowers\SuperStrength\StrengthGlow.part
		Sound PunchAASSCustom 100.0 100.0 0.85
	End

	Event
		EName	Streak1
		Type	local
		At	WepL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\SuperStrength\StrengthStreak.part
		Part3	CustomizeablePowers\SuperStrength\StrengthGlow.part
	End
End

#############################################################

End