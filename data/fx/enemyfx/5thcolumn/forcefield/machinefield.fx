#############################################################
## BaseForceBubble.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Root
		BhvrOverride
			Scale		8 8 8
		End
		Geom	GEO_FX_TestSphere
	End

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0.0 3.0 0.0
			Scale		2.0 2.0 2.0
			Spin		0.05 0.0 0.0
			StartColor	150 255 200
			Alpha		125
		End
		Geom	FX_5thColumn_MachineField
	End

	Event
		Type	Local
		At	Root
		BhvrOverride
			Scale		1.25 1.0 1.0
			PositionOffset	0.0 0.25 0.0
			PyrRotate	0.0 90.0 0.0
		End
		Part	:MachineField_Ground.part
	End

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	.75 3.0 0.0
			Scale		2.9 1.5 1.5
			StartColor	255 200 0
			Alpha		255
		End
		Geom	Cagearchs
		Lifespan 0
	End

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0.0 3.0 0.0
			Scale		2.9 1.5 1.5
			Spin		0.05 0.0 0.0
			PyrRotate	180.0 0.0 0.0
			StartColor	255 200 0
			Alpha		150
		End
		Geom	Cagearchs
		Lifespan 0
	End
End

#############################################################

End