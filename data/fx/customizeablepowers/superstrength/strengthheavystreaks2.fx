#############################################################
## StrengthHeavyStreaks2.fx
#############################################################

FxInfo

Lifespan 40

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	UARMR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	CustomizeablePowers\SuperStrength\StrengthBodyGlows.part
		PMagnet	LarmR
	End

	Event
		Type	local
		At	UARML
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	CustomizeablePowers\SuperStrength\StrengthBodyGlows.part
		PMagnet	LarmL
	End

	Event
		Type	local
		At	LarmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	CustomizeablePowers\SuperStrength\StrengthBodyGlows.part
		PMagnet	WepR
		Sound PunchSSCustom2 100.0 100.0 0.8
	End

	Event
		Type	local
		At	LarmL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	CustomizeablePowers\SuperStrength\StrengthBodyGlows.part
		PMagnet	WepL
	End
End

Condition
	On 	Time
	Time 	10

	Event
		Type	local
		At	WepR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	CustomizeablePowers\SuperStrength\StrengthStreak.part
		Part	CustomizeablePowers\SuperStrength\StrengthGlow.part
	End

	Event
		Type	local
		At	WepL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	CustomizeablePowers\SuperStrength\StrengthStreak.part
		Part	CustomizeablePowers\SuperStrength\StrengthGlow.part
	End
End

#############################################################

End