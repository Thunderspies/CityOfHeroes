#############################################################
## Peacebringer - Radient Strike
#############################################################

FxInfo

LifeSpan	30

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local 
		At	Origin
		BHVR	Behaviors/OffsetEyes.bhvr
		ChildFX	POWERS\CosmicPowers\Peacebringer\ActivationEyes\ActivationEyes_Child.fx
		LifeSpan	30
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	SwordFlash2
		Type	Local
		At	wepR
		Part	:PunchFlash.part
		Part	:PunchTendrils.part
		Part	:PunchMistEnergy.part
		Sound peacepunch 80 80 .86
		LifeSpan 15
	End

End

#############################################################

End			