#############################################################
## Judgment_Ion.fx
#############################################################

FxInfo

#############################################################

Input
	Inpname	Hips
End

#############################################################


Condition
	On	Time
	Time	0

	Event
		EName	ColRAnchor
		Type	Local
		At	Chest
		BhvrOverride
			PositionOffset	0.0 1.35 -0.7
			Scale		0.2 0.2 0.2
		End
		Part	:Flash_Continuing_Head.part
		Part	:Electricity_Hands.part
		Part	:Electricity_Hands_Bright.part
		Part	:Electricity_Burst.part
		Part	:Sparks_Continuing_Directional.part
	End
End

Condition
	On	Time
	Time	10

	Event
		EName	ColRAnchor
		Type	Local
		At	UArmR
		BhvrOverride
			PositionOffset	0.0 0.3 0.0
			Scale		0.2 0.2 0.2
		End
		Part	:Bolt_Connecting.part
		Part	:Bolt_Connecting_Broken.part
		Part	:Flash_Continuing_Arms.part

		Part	:Flash_Continuing_Small.part
		Part	:Sparks_Continuing_Directional.part
	End
End

Condition
	On	Time
	Time	13

	Event
		EName	ColLAnchor
		Type	Local
		At	UArmL
		BhvrOverride
			PositionOffset	0.0 0.3 0.0
			Scale		0.2 0.2 0.2
		End
		Part	:Bolt_Connecting.part
		Part	:Bolt_Connecting_Broken.part
		Part	:Flash_Continuing_Arms.part

		Part	:Flash_Continuing_Small.part
		Part	:Sparks_Continuing_Directional.part
	End
End

Condition
	On	Time
	Time	16

	Event
		EName	UArmRAnchor
		Type	Local
		At	UArmR
		BhvrOverride
			PositionOffset	0.6 0.35 0.0
			Scale		0.2 0.2 0.2
		End
		Part	:Bolt_Connecting.part
		Part	:Bolt_Connecting_Broken.part
		Part	:Flash_Continuing_Arms.part

		Part	:Flash_Continuing_Small.part
		Part	:Sparks_Continuing_Directional.part
	End
End

Condition
	On	Time
	Time	20

	Event
		EName	UArmLAnchor
		Type	Local
		At	UArmL
		BhvrOverride
			PositionOffset	-0.6 0.35 0.0
			Scale		0.2 0.2 0.2
		End
		Part	:Bolt_Connecting.part
		Part	:Bolt_Connecting_Broken.part
		Part	:Flash_Continuing_Arms.part

		Part	:Flash_Continuing_Small.part
		Part	:Sparks_Continuing_Directional.part
	End
End

#############################################################

End