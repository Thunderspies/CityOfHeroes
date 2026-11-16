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
		Part1	CustomizeablePowers\SuperStrength\StrengthStreak.part
		Part3	CustomizeablePowers\SuperStrength\StrengthGlow.part
		Sound PunchASSCustom 100.0 100.0 0.85
	End

	Event
		Type	local
		At	WepL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\SuperStrength\StrengthStreak.part
		Part3	CustomizeablePowers\SuperStrength\StrengthGlow.part
	End
End

##################################

End

