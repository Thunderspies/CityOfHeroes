#############################################################
## StoneFist_Activation.fx
#############################################################

FxInfo

#############################################################

Condition
	On 		TriggerBits
	TriggerBits	MALE
	DoMany		1

	Event
		EName	StoneFist_Male
		Type	Local
		At	LARMR
		Flags	OneAtATime
		While	MALE
		Bhvr	CustomizeablePowers\StoneMelee\Lava\MagmaHands.bhvr
		geom	Male_MagmaHandR
	End

	Event
		EName	StoneFist_AddGlow_Male
		Type	Local
		At	LARMR
		Flags	OneAtATime
		While	MALE
		Bhvr	CustomizeablePowers\StoneMelee\Lava\MagmaHands.bhvr
		geom	Male_MagmaHandR_AddGlow
	End
End


Condition
	On 		TriggerBits
	TriggerBits	FEMALE
	DoMany		1

	Event
		EName	StoneFist_Fem
		Type	Local
		At	LARMR
		Flags	OneAtATime
		While	FEMALE
		Bhvr	CustomizeablePowers\StoneMelee\Lava\MagmaHands.bhvr
		geom	Fem_MagmaHandR
	End

	Event
		EName	StoneFist_AddGlow_Fem
		Type	Local
		At	LARMR
		Flags	OneAtATime
		While	FEMALE
		Bhvr	CustomizeablePowers\StoneMelee\Lava\MagmaHands.bhvr
		geom	Fem_MagmaHandR_AddGlow
	End
End

Condition
	On 		TriggerBits
	TriggerBits	HUGE
	DoMany		1

	Event
		EName	StoneFist_Huge
		Type	Local
		At	LARMR
		Flags	OneAtATime
		While	HUGE
		Bhvr	CustomizeablePowers\StoneMelee\Lava\MagmaHands.bhvr
		geom	Huge_MagmaHandR
	End

	Event
		EName	StoneFist_AddGlow_Male
		Type	Local
		At	LARMR
		Flags	OneAtATime
		While	HUGE
		Bhvr	CustomizeablePowers\StoneMelee\Lava\MagmaHands.bhvr
		geom	Huge_MagmaHandR_AddGlow
	End
End

#############################################################

End