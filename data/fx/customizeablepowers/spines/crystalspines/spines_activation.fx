#############################################################
## Spines_Activation.fx
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 		TriggerBits
	TriggerBits	WEPSPAWN
	DoMany	1

	Event
		EName	DrawSound
		Type	Local
		At	WepR
		Flags	OneAtATime
		Sound Quills1 70 70 0.7
	End
End

## HANDS ###########################################################

Condition
	On 	Time
	Time 	9

	Event
		EName	Spike
		Type	Local
		At	Chest
		Part	CustomizeablePowers\Spines\Spines\MustardGasSpecksHitA.part
		Part	CustomizeablePowers\Spines\Spines\MustardGasHitA.part
		LifeSpan 7
	End
End

Condition
	On 	Time
	Time 	15

	Event
		EName	GasSpurts
		Type	Local
		At	HandL
		Part	CustomizeablePowers\Spines\Spines\MustardGasHitBA.part
		Part	CustomizeablePowers\Spines\Spines\MustardGasSpecksHitBA.part
		Lifespan 16
	End
End

Condition
	On 	Time
	Time 	15

	Event
		Type	Local
		At	HandR
		Part	CustomizeablePowers\Spines\Spines\MustardGasHitBA.part
		Part	CustomizeablePowers\Spines\Spines\MustardGasSpecksHitBA.part
		lifeSpan 16
	End
End

## BODY ###########################################################

Condition
	On 		TriggerBits
	TriggerBits	MALE
	DoMany		1

	Event
		EName	BodySpines_Male
		Type	Local
		Flags	OneAtATime
		At	Root
		While	MALE
		ChildFX	:Child_MaleSpines.fx
	End
End

Condition
	On 		TriggerBits
	TriggerBits	FEMALE
	DoMany		1

	Event
		EName	BodySpines_Female
		Type	Local
		At	Root
		Flags	OneAtATime
		While	FEMALE
		ChildFX	:Child_FemSpines.fx
	End
End

Condition
	On 		TriggerBits
	TriggerBits	HUGE
	DoMany		1

	Event
		EName	BodySpines_Huge
		Type	Local
		At	Root
		Flags	OneAtATime
		While	HUGE
		ChildFX	:Child_HugeSpines.fx
	End
End

#############################################################

End