#########################################################
##	Fire_Ball
##
FxInfo

#Condition
#	On	Time
#	Time	0
#
#	Event
#		Ename	smoke
#		Type	Local
#		At	root
#		ChildFX	powers/DarknessControl/DarkArmorNoRing.fx
#
#	End
#
#End
#
#

Condition
	On 	Time
	Time 	0
	
	Event
		Type	Local
		At 	origin
		Sound chargeup4 60 60 .66
	End
End

Condition
	On	Time
	Time	28
	Event
		Ename	Logo
		Type	Local
		At	Chest
		Part	:Logo01.part
	End

	Event
		Ename	Logo
		Type	Local
		At	Waist
		BhvrOverride
			PositionOffset	0 .5 0
		End
		Part	:Logo02.part
	End

End


End	


			