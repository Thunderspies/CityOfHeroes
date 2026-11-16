#############################################################
## PraetorianBox.fx
#############################################################



FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	BoxAnchor
		Type	StartPositONly
		At	Hips
		BhvrOverride
			PositionOffset	0 -1.2 0
			PyrRotate	0 90 0

			Alpha	1
		End
		Geom	FX_BoxAnchor
	End

	Event
		Type	Local
		At	BoxAnchor
		AltPiv	1
		BhvrOverride
			PyrRotate	0 7 0
		End
		Geom	FX_PraetorianBox
	End

	Event
		Type	Local
		At	BoxAnchor
		AltPiv	2
		BhvrOverride
			PyrRotate	0 -7 0
		End
		Geom	FX_PraetorianBox
	End


End

#############################################################

End
