#############################################################
## MATablet.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	WepR
		BhvrOverride
			PositionOffset	-0.02 0.05 0.1
			Scale		0.625 0.625 0.625
			StartColor	1 1 1
		End
		Geom	Prop_Pen
	End

	Event
		Type	Local
		At	WepL
		BhvrOverride
			PositionOffset	0.35 -0.1 -0.1
			PyrRotate	180 15 0
			Scale		0.875 0.75 0.75
		End
		Geom	_ArchEnt_Prop_Tablet_01
	End

	Event
		Type	Local
		At	WepL
		BhvrOverride
			PositionOffset	0.35 -0.1 -0.1
			PyrRotate	180 15 0
			Scale		0.875 0.75 0.75
		End
		Geom	_ArchEnt_Prop_TabletUI_01
	End
End

#############################################################

End