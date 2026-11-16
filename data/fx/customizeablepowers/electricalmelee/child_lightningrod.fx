#############################################################
## Red_LightningRodChild.fx
#############################################################

FxInfo

Lifespan 165

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	Hookup
		Type	local
		At	Root
		BHVR	Powers\ItemsAndTempPowers\OverheadOffset.bhvr
	End

	Event
		Ename	Hookup2
		Type	local
		At	Root
		BHVR	Powers\ItemsAndTempPowers\OverheadOffset2.bhvr
	End
End

#############################################################

Condition
	On 	Time
	Time 	1

	Event
		Type	local
		At	Hookup
		Part	:LightningStrike3.part
		Pmagnet	Root
	End
End

Condition
	On 	Time
	Time 	3

	Event
		Type	local
		At	Hookup2
		Part	:LightningStrike2.part
		Pmagnet	Root
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	Offset01
		Type	Start
		At	Root
		BhvrOverride
			Startjitter	6 6 6
			PositionOffset	0 40 0
		End
	End

	Event
		Ename	Offset02
		Type	Start
		At	Root
		BhvrOverride
			Startjitter	6 6 6
			PositionOffset	0 20 0
		End
	End
End

Condition
	On	Time
	Time	0

	Event
		Type	Start
		At	Offset01
		Part	:LightningRod_ChargedBoltCore.Part
		Part	:LightningRod_ChargedBoltCore2.Part
		POther	Offset02
	End

	Event
		Type	Start
		At	Offset02
		Part	:LightningRod_ChargedBoltCore.Part
		Part	:LightningRod_ChargedBoltCore2.Part
		POther	WepR
	End
End

#############################################################

End