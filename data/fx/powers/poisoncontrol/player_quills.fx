#############################################################
## Player_Quills.fx
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

#############################################################

Condition
	On 	Time
	Time 	5

	Event
		EName	Spike
		Type	Local
		At	HandL
		Bhvr	Behaviors\PoisonSpikeScale.bhvr
		Geom	Quills_WepL
	End
End

Condition
	On 	Time
	Time 	15

	Event
		EName	GasSpurts
		Type	Local
		At	HandL

		Part1	:MustardGasHitBA.part
		Part2	:MustardGasSpecksHitBA.part
		lifeSpan	16
	End
End

#############################################################

Condition
	On 	Time
	Time 	5

	Event
		EName	Spike2
		Type	Local
		At	HandR

		Bhvr	Behaviors\PoisonSpikeScale.bhvr
		Geom	Quills_WepR
	End
End

Condition
	On 	Time
	Time 	9

	Event
		EName	Spike
		Type	Local
		At	Chest
		Part2	:MustardGasSpecksHitA.part
		Part1	:MustardGasHitA.part
		LifeSpan 7
	End
End

Condition
	On 	Time
	Time 	15

	Event
		Type	Local
		At	HandR

		Part1	:MustardGasHitBA.part
		Part2	:MustardGasSpecksHitBA.part
		lifeSpan 16
	End

End

## BACK ###########################################################

Condition
	On 	Time
	Time 	3
	Event
		Type	Local
		At	waist

		Bhvr	Behaviors\PoisonSpikeScale1.bhvr
		Geom	Quills_Waist
	End
End

Condition
	On 	Time
	Time 	5
	Event
		Type	Local
		At	chest

		Bhvr	Behaviors\PoisonSpikeScale.bhvr
		Geom	Quills_Chest01
	End

	Event
		Type	Local
		At	chest

		Bhvr	Behaviors\PoisonSpikeScaleQuick.bhvr
		Geom	Quills_chest02
	End
End

## LEGS ###########################################################

Condition
	On 	Time
	Time 	2
	Event
		Type	Local
		At	UlegL
		Bhvr	Behaviors\PoisonSpikeScale1.bhvr
		Geom	Quills_UlegL
	End

	Event
		Type	Local
		At	UlegR
		Bhvr	Behaviors\PoisonSpikeScale2.bhvr
		Geom	Quills_UlegR
	End
End

Condition
	On 	Time
	Time 	3
	Event
		Type	Local
		At	LlegR
		Bhvr	Behaviors\PoisonSpikeScale.bhvr
		Geom	Quills_LlegR
	End

	Event
		Type	Local
		At	LlegL
		Bhvr	Behaviors\PoisonSpikeScale.bhvr
		Geom	Quills_LlegL
	End
End

#############################################################

End