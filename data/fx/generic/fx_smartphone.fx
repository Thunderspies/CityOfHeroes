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
		At	WepL
		Geom	Prop_SmartPhone
			BhvrOverride
				PositionOffset 0 -0.035 0
			End
	End
End

#Condition
#	On TimeOfDay
#	DayStart 16.000000
#	DayEnd 7.000000
#
#	Event
#		At WepL
#		Type Local
#		Flags oneAtATime
#		EName Prop_Glow
#		Geom Prop_SmartPhone_Glow
#		BhvrOverride
#			PositionOffset 0 -0.035 0
#		End
#	End
#
#End

End
##################################

