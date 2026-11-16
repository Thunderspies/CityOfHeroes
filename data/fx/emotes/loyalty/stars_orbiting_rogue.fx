#############################################################
## Alpha_Body_Male.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	SpinAnchor
		Type	Local
		At	Chest
		BhvrOverride
			Spin	0 .01 0
		End
	End

	Event
		EName	StarOffsets
		Type	Local
		At	SpinAnchor
		BhvrOverride
			Spin		0.0 0.015 0
			Scale		2.0 2.0 2.0
			PYRrotate	-30 90 -30
			Alpha		1
		End
		GEOM	FX_GEO_StarOffsets
	End


	Event
		EName	Ring
		Type	Local
		At	StarOffsets
		AltPiv	1
		ChildFX	:CHILD_Star_Rogue.fx
	End

	Event
		EName	Ring
		Type	Local
		At	StarOffsets
		AltPiv	2
		ChildFX	:CHILD_Star_Rogue.fx
	End

	Event
		EName	Ring
		Type	Local
		At	StarOffsets
		AltPiv	3
		ChildFX	:CHILD_Star_Rogue.fx
	End

	Event
		EName	Ring
		Type	Local
		At	StarOffsets
		AltPiv	4
		ChildFX	:CHILD_Star_Rogue.fx
	End

	Event
		EName	Ring
		Type	Local
		At	StarOffsets
		AltPiv	5
		ChildFX	:CHILD_Star_Rogue.fx
	End

	Event
		EName	Ring
		Type	Local
		At	StarOffsets
		AltPiv	6
		ChildFX	:CHILD_Star_Rogue.fx
	End

	Event
		EName	Ring
		Type	Local
		At	StarOffsets
		AltPiv	7
		ChildFX	:CHILD_Star_Rogue.fx
	End
End

#############################################################

End