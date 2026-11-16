#########################################################
## Slime Aura Head
#########################################################

FxInfo
Flags DontSuppress

########################################################



## SOUND FX #######################################################

Condition
	On	Time
	Time	1
	Repeat  6

	Event
		EName	Geo
		Type	Local
		At	HandR
		ChildFX	:CHILD_Magnetic_3DDebris_Fist.fx

	End

End

#Condition
#	On	Time
#	Time	1
#
#	Event
#		EName	HeadSlime
#		Type	local
#		At	HandL
#		BhvrOverride
#          		PositionOffset  0 0 0
#
#         	End
#
#		Part2	:Magnetic_Lines.part
#
#	End
#End
#Condition
#	On	Time
#	Time	1
#
#	Event
#		EName	HeadSlime
#		Type	local
#		At	HandL
#		BhvrOverride
#          		PositionOffset  0 0 0
#
#         	End
#
##		Part2	:Magnetic_LinesB.part
#
#	End
#End

End