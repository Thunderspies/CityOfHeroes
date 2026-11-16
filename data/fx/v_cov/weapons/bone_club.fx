#############################################################
## Header
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	4

	Event
		Type	Local
		At	WepR
		Sound boneClub2 70 70 .98
	End
End

Condition
	On 	Time
	Time 	9

	Event
		EName 	P
		Type	Local
		At	WepR
		Geom	Bone_Club
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Sound boneclub_loop 60 60 .33
	End

	Event
		Type	Local
		At	wepR

		Bhvr	:OffsetBoneClubEyes.bhvr
		Part	:BoneClubEyesMistLeft.part
		#Part	:BoneClubEyesMistRight.part
	End

	Event
		Type	Local
		At	wepR
		BHVR	:OffsetBoneClubEyesR.bhvr
		#Part	:BoneClubEyesMistLeft.part
		Part	:BoneClubEyesMistRight.part

	End
End

#############################################################

End