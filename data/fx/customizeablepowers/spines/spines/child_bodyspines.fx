#########################################################
## Spines - Body Spines Child FX
#########################################################

FxInfo

Flags InheritAlpha

## BODY SPINES #######################################################

Condition
	On 	Time
	Time 	2

	Event
		EName	Spike02b
		Type	Local
		At	UlegL
		Bhvr	Behaviors\PoisonSpikeScale1.bhvr
		Geom	Quills_UlegL
	End

	Event
		EName	Spike02b
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
		At	waist
		Bhvr	Behaviors\PoisonSpikeScale1.bhvr
		Geom	Quills_Waist
	End

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

Condition
	On 	Time
	Time 	5

	Event
		Type	Local
		At	HandL
		Bhvr	Behaviors\PoisonSpikeScale.bhvr
		Geom	Quills_WepL
	End

	Event
		Type	Local
		At	HandR
		Bhvr	Behaviors\PoisonSpikeScale.bhvr
		Geom	Quills_WepR
	End

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

#############################################################

End