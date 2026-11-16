#########################################################
## Slime Aura Head
#########################################################

FxInfo

Flags DontSuppress
########################################################



## SOUND FX #######################################################

Condition
	On	Time
	Time	0

	Event
		Type	start
		At	Root
#		Sound explohit 80 80 0.77
	End
End



Condition
	On	Time
	Time	1
	Repeat  10

	Event
		EName	Geo
		Type	PositOnly
		At	Chest
		ChildFX	:CHILD_Magnetic_3DDebris.fx

	End

End

#Condition
#	On	Time
#	Time	1
#
#	Event
#		EName	HeadSlime
#		Type	local
#		At	hips
#		BhvrOverride
#          		PositionOffset  0 1.5 0
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
#		At	hips
#		BhvrOverride
#          		PositionOffset  0 -1.0 0
#
#         	End
#
#		Part2	:Magnetic_LinesB.part
#
#	End
#End

End