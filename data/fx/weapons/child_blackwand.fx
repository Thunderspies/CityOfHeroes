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
		EName 	Blackwand
		Type	Local
		At	Origin
		BHVR	Behaviors\Weapon_Fade_UP.bhvr
		Geom	GEO_WepR_Blackwand
		Sound Blackwand2_loop 50 50 0.1
	End

	Event
		Ename	Gem
		Type	Local
		At	Blackwand
		AltPiv	1
		BHVR	Behaviors\Weapon_Fade_UP.bhvr
		BhvrOverride
			Spin	0 0 0.1
		End
		Geom	FX_Blackwand_Gem
		part1	:Blackwand\Staff_GemFlare01.part
		part1	:Blackwand\Staff_GemFlare02.part
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		ENAME	Glow
		Type	Local
		At	Blackwand
		AltPiv	1
		part1	:Blackwand\Staff_Motes01.part
		part4	:Blackwand\Staff_Flare01.part
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	StaffEnd_Rotate1
		Type	Local
		At	Blackwand
		AltPiv	1
		BhvrOverride
			Spin 0.0 0.0 0.125
		End
	End

	Event
		EName 	StaffEnd_Offset1
		Type	Local
		At	StaffEnd_Rotate1
		BhvrOverride
			PositionOffset	0.5 0 0
		End
	End

	Event
		EName 	StaffEnd_Rotate2
		Type	Local
		At	StaffEnd_Offset1
		BhvrOverride
			Spin 0.0625 0.0 0.0
		End
	End


	Event
		EName 	StaffEnd_Offset2
		Type	Local
		At	StaffEnd_Rotate2
		BhvrOverride
			PositionOffset	0 0 0.5
		End
	End

	Event
		ENAME	Glow
		Type	Local
		At	StaffEnd_Offset2
		part1	:Blackwand\Staff_SwirlingMote_Flare01.part
		part4	:Blackwand\Staff_SwirlingMote_Streak01.part
		part4	:Blackwand\Staff_SwirlingMote_Streak02.part
	End
End

Condition
	On 	Time
	Time 	15

	Event
		EName 	2StaffEnd_Rotate1
		Type	Local
		At	Blackwand
		AltPiv	1
		BhvrOverride
			Spin 0.0 0.0 -0.125
		End
	End

	Event
		EName 	2StaffEnd_Offset1
		Type	Local
		At	2StaffEnd_Rotate1
		BhvrOverride
			PositionOffset	-0.5 0 0
		End
	End

	Event
		EName 	2StaffEnd_Rotate2
		Type	Local
		At	2StaffEnd_Offset1
		BhvrOverride
			Spin -0.0625 0.0 0.0
		End
	End


	Event
		EName 	2StaffEnd_Offset2
		Type	Local
		At	2StaffEnd_Rotate2
		BhvrOverride
			PositionOffset	0 0 -0.5
		End
	End

	Event
		ENAME	Glow
		Type	Local
		At	2StaffEnd_Offset2
		part1	:Blackwand\Staff_SwirlingMote_Flare01.part
		part4	:Blackwand\Staff_SwirlingMote_Streak01.part
		part4	:Blackwand\Staff_SwirlingMote_Streak02.part
		Magnet	2StaffEnd_Offset2
	End
End

#############################################################

End