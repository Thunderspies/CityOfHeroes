#############################################################
## Header
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	hookup
		Type	Local
		At	WepR
		Geom	SthenoSnakeStaff
		Sound BoneClubout 60 60 .4
		Bhvr	Behaviors/StaffFade.bhvr
	End

	Event
		Type	Local
		At	wepR

		BhvrOverride
			PositionOffset	.07 -.085 2.34
		End
		Part1	:DaemonPupil2.part
	End

	Event
		Type	Local
		At	wepR

		BhvrOverride
			PositionOffset	.06 .085 2.34
		End

		Part1	:DaemonPupil.part
	End
End

#############################################################

End