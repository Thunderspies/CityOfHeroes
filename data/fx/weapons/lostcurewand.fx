#############################################################
## FX System Name
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Offset
		Type	Local
		At	WepR
		Geom	Staff_02
		Sound Blackwand2_loop 50 50 .1
	End

	Event
		Type	Local
		At	WepR
		BHVR	Behaviors\Weapon_Fade_UP.bhvr
		Geom	GEO_WepR_Staff_3
		Sound DiviningRodOut 50 50 .65
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		ENAME	Glow
		Type	Local
		At	Offset
		AltPiv	1
		Part	WEAPONS\LostCureWand\LostCureWand_Bubbles01.part
		Part	WEAPONS\LostCureWand\LostCureWand_Bubbles02.part
		Part	WEAPONS\LostCureWand\LostCureWand_Flare01.part
		Part	WEAPONS\LostCureWand\LostCureWand_Flare02.part
		Part	WEAPONS\LostCureWand\LostCureWand_Glow01.part
	End

#	Event
#		ENAME	Glow
#		Type	Local
#		At	Offset
#		AltPiv	1
#		part4	WEAPONS\LostCureWand\LostCureWand_Streak01.part
#		part4	WEAPONS\LostCureWand\LostCureWand_Streak02.part
#	End
End

#Condition
#	On 	Time
#	Time 	0
#
#	Event
#		EName 	StaffEnd_Rotate1
#		Type	Local
#		At	Offset
#		AltPiv	1
#		BhvrOverride
#			Spin 0.0 0.0 0.125
#		End
#	End
#
#	Event
#		EName 	StaffEnd_Offset1
#		Type	Local
#		At	StaffEnd_Rotate1
#		BhvrOverride
#			PositionOffset	0.5 0 0
#		End
#	End
#
#	Event
#		EName 	StaffEnd_Rotate2
#		Type	Local
#		At	StaffEnd_Offset1
#		BhvrOverride
#			Spin 0.0625 0.0 0.0
#		End
#	End
#
#
#	Event
#		EName 	StaffEnd_Offset2
#		Type	Local
#		At	StaffEnd_Rotate2
#		BhvrOverride
#			PositionOffset	0 0 0.5
#		End
#	End
#
#	Event
#		ENAME	Glow
#		Type	Local
#		At	StaffEnd_Offset2
#		part1	:Blackwand\Staff_SwirlingMote_Flare01.part
#		part4	:Blackwand\Staff_SwirlingMote_Streak01.part
#		part4	:Blackwand\Staff_SwirlingMote_Streak02.part
#	End
#End
#
#Condition
#	On 	Time
#	Time 	15
#
#	Event
#		EName 	2StaffEnd_Rotate1
#		Type	Local
#		At	Offset
#		AltPiv	1
#		BhvrOverride
#			Spin 0.0 0.0 -0.125
#		End
#	End
#
#	Event
#		EName 	2StaffEnd_Offset1
#		Type	Local
#		At	2StaffEnd_Rotate1
#		BhvrOverride
#			PositionOffset	-0.5 0 0
#		End
#	End
#
#	Event
#		EName 	2StaffEnd_Rotate2
#		Type	Local
#		At	2StaffEnd_Offset1
#		BhvrOverride
#			Spin -0.0625 0.0 0.0
#		End
#	End
#
#
#	Event
#		EName 	2StaffEnd_Offset2
#		Type	Local
#		At	2StaffEnd_Rotate2
#		BhvrOverride
#			PositionOffset	0 0 -0.5
#		End
#	End
#
#	Event
#		ENAME	Glow
#		Type	Local
#		At	2StaffEnd_Offset2
#		part1	:Blackwand\Staff_SwirlingMote_Flare01.part
#		part4	:Blackwand\Staff_SwirlingMote_Streak01.part
#		part4	:Blackwand\Staff_SwirlingMote_Streak02.part
#		Magnet	2StaffEnd_Offset2
#	End
#End

#############################################################

End