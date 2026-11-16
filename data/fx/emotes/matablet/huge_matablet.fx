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
			PositionOffset	0.1 0.2 -0.1
			Scale		0.75 0.75 0.75
			StartColor	1 1 1
		End
		Geom	Prop_Pen
	End

	Event
		Type	Local
		At	WepL
		BhvrOverride
			PositionOffset	0.15 -0.1 0.1
			PyrRotate	180 10 -10
			Scale		0.9 0.8 0.8
		End
		Geom	_ArchEnt_Prop_Tablet_01
	End

	Event
		Type	Local
		At	WepL
		BhvrOverride
			PositionOffset	0.15 -0.1 0.1
			PyrRotate	180 10 -10
			Scale		0.9 0.8 0.8
		End
		Geom	_ArchEnt_Prop_TabletUI_01
	End
End

#############################################################

End