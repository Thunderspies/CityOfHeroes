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
			PositionOffset	0.025 0.075 -0.05
			Scale		0.5 0.5 0.5
			StartColor	1 1 1
		End
		Geom	Prop_Pen
	End

	Event
		Type	Local
		At	WepL
		BhvrOverride
			PositionOffset	0.3 0.05 -0.1
			PyrRotate	180 5 5
			Scale		0.7 0.6 0.6
		End
		Geom	_ArchEnt_Prop_Tablet_01
	End

	Event
		Type	Local
		At	WepL
		BhvrOverride
			PositionOffset	0.3 0.05 -0.1
			PyrRotate	180 5 5
			Scale		0.7 0.6 0.6
		End
		Geom	_ArchEnt_Prop_TabletUI_01
	End
End

#############################################################

End