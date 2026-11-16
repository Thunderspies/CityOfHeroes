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
		Ename	Fem_CrystalHandR
		Type	Local
		At	LARMR
		Flags	OneAtATime
		While	MALE
		Bhvr	CustomizeablePowers\StoneMelee\Crystal\CrystalHands.bhvr
		geom	Male_CrystalHandR
	End

	Event
		Ename	Fem_CrystalHandL
		Type	Local
		At	LARML
		Flags	OneAtATime
		While	MALE
		Bhvr	CustomizeablePowers\StoneMelee\Crystal\CrystalHands.bhvr
		geom	Male_CrystalHandL
	End
End

Condition
	On 		TriggerBits
	TriggerBits	FEMALE
	DoMany		1

	Event
		Ename	Male_CrystalHandR
		Type	Local
		At	LARMR
		Flags	OneAtATime
		While	FEMALE
		Bhvr	CustomizeablePowers\StoneMelee\Crystal\CrystalHands.bhvr
		geom	Male_CrystalHandR
	End

	Event
		Ename	Male_CrystalHandL
		Type	Local
		At	LARML
		Flags	OneAtATime
		While	FEMALE
		Bhvr	CustomizeablePowers\StoneMelee\Crystal\CrystalHands.bhvr
		geom	Male_CrystalHandL
	End
End

Condition
	On 		TriggerBits
	TriggerBits	HUGE
	DoMany		1

	Event
		Ename	Huge_CrystalHandR
		Type	Local
		At	LARMR
		Flags	OneAtATime
		While	HUGE
		Bhvr	CustomizeablePowers\StoneMelee\Crystal\CrystalHands.bhvr
		geom	Huge_CrystalHandR
	End

	Event
		Ename	Huge_CrystalHandL
		Type	Local
		At	LARML
		Flags	OneAtATime
		While	HUGE
		Bhvr	CustomizeablePowers\StoneMelee\Crystal\CrystalHands.bhvr
		geom	Huge_CrystalHandL
	End
End

#############################################################

End