#########################################################
##
########################################################
FxInfo

Input
	Inpname	WepR
End

Input
	Inpname	WepL
End


##################################################################

Condition
	On	Time
	Time	0


	Event
		EName 	Prop_L
		Type	Local
		Bhvr	behaviors/Smartphone_Huge.bhvr
		At	WepL
		Geom	Prop_SmartPhone
		BhvrOverride
			PositionOffset 0 0 0.1
		End
	End
End

Condition
	On TimeOfDay
	DayStart 16.000000
	DayEnd 7.000000

	Event
		At WepL
		Type Local
		EName Prop_Glow
		Flags oneAtATime
		Geom Prop_SmartPhone_Glow
		BhvrOverride
			PositionOffset 0 0 0.1
		End
	End

End
##################################

End


