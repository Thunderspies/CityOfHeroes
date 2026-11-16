#########################################################
## Staff of the M.A.G.I.
#########################################################

FxInfo

Flags    InheritAlpha

## SOUND FX #######################################################

Condition
	On 	Time
	Time 	0

	
	Event
		EName 	hookup
		Type	Local 
		At	wepR
		Sound DiviningRodOut 50 50 .65
	End
End

## STAFF FX #######################################################

Condition
	On 	Time
	Time 	0
	
	Event
		EName 	hookup
		Type	Local 
		At	WepR
		BHVR	Behaviors/Weapon_Fade_UP.bhvr
		Geom	GEO_WepR_VanguardStaff01
	End
End

#Condition
#	On 	Time
#	Time 	0
#	
#	Event
#		EName 	StaffOffset
#		Type	Local 
#		At	WepR
#		BhvrOverride
#			PositionOffset	-0.125 0.0 -0.25
#		End
#	End
#
#	Event	
#		ENAME	Glow
#		Type	Local
#		At	StaffOffset
#		part1	:StaffFX\Staff_HandleGlow01.part
#		part2	:StaffFX\Staff_HandleGlow02.part
#	End
#End

Condition
	On 	Time
	Time 	0
	
	Event
		EName 	StaffEnd
		Type	Local 
		At	WepR
		BhvrOverride
			PositionOffset	0 0 1.5
		End
	End

	Event	
		ENAME	Glow
		Type	Local
		At	StaffEnd
		part1	:StaffFX\Staff_Motes01.part
		#part2	:StaffFX\Staff_Glow01.part
		#part3	:StaffFX\Staff_Glow02.part
		part4	:StaffFX\Staff_Glow03.part
		part4	:StaffFX\Staff_Glow04.part
		part4	:StaffFX\Staff_Flare01.part
	End

	Event	
		ENAME	Glow
		Type	Local
		At	StaffEnd
		part4	:StaffFX\Staff_Streak01.part
		part4	:StaffFX\Staff_Streak02.part
	End

End

## SWIRLING MOTES #######################################################

Condition
	On 	Time
	Time 	0
	
	Event
		EName 	StaffEnd_Rotate1
		Type	Local 
		At	StaffEnd
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
		part1	:StaffFX\Staff_SwirlingMote_Flare01.part
		part4	:StaffFX\Staff_SwirlingMote_Streak01.part
		part4	:StaffFX\Staff_SwirlingMote_Streak02.part
	End
End

Condition
	On 	Time
	Time 	15
	
	Event
		EName 	2StaffEnd_Rotate1
		Type	Local 
		At	StaffEnd
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
		part1	:StaffFX\Staff_SwirlingMote_Flare01.part
		part4	:StaffFX\Staff_SwirlingMote_Streak01.part
		part4	:StaffFX\Staff_SwirlingMote_Streak02.part
		Magnet	2StaffEnd_Offset2
	End
End

#########################################################

End	