#############################################################
## PraetorianBox_Open.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	BoxAnchor
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 2.4 1.5
			PyrRotate	0 90 0

			Alpha	1
		End
		Geom	FX_BoxAnchor
	End

	Event
		Type	Start
		At	BoxAnchor
		AltPiv	1
		BhvrOverride
			PyrRotate	0 90 0
 			StartColor 255 255 255
		End
		Geom	FX_PraetorianBox_Open
	End

	Event
		Type	Start
		At	BoxAnchor
		AltPiv	2
		BhvrOverride
			PyrRotate	0 250 0
 			StartColor 255 255 255
		End
		Geom	FX_PraetorianBox_Open
	End

	Event
		Type	Start
		At	BoxAnchor
		AltPiv	1
		BhvrOverride
			PyrRotate	0 85 0
 			StartColor 255 255 255
		End
		Geom	FX_P_BottleGroup
	End

	Event
		Type	Start
		At	BoxAnchor
		AltPiv	2
		BhvrOverride
			PyrRotate	0 250 0
		End
		Geom	FX_P_BottleGroup
	End


End

#############################################################

End
