#############################################################
## HeadSonicScreech.fx
#############################################################

FxInfo

LifeSpan 100

#############################################################

Condition
	On 	Time
	Time 	11

	Event
		Type	local
		At	head
		Sound sonicscreech 80 80 .99
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	rigging
		Type	local
		At	head
		Geom	PsiTele02
	End

	Event
		EName 	MussleFlashRigging
		Type	local
		At	head
		Geom	PsionicRigging
	End
End

Condition
	On 	Time
	Time 	20

	Event
		Type	start
		At	MussleFlashRigging
		altpiv	3
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:MussleFlashRingCoreScreech.part
		Part	:MussleFlashRingScreech.part
		LifeSpan 20
	End
End

#############################################################

End