#############################################################
## LitchEyes.fx
#############################################################

FxInfo
Flags    InheritAlpha

########################################################

Condition
	On 	Time
	Time 	30

	Event
		EName 	Prime
		Type	Local
		At	Eyes
		Geom	CircleOfThorns
	End

	Event
		Type	Local
		At	T_Head
		BhvrOverride
			PositionOffset	0 0.05 0
		End
		Part	:Head_Cold_Glow.part
		Part	:Fire_Cold.part
	End

	Event
		Type	Local
		At	Prime
		AltPiv 	1
		BhvrOverride
			PositionOffset	-0.1 0 0.2
		End
		BHVR	V_COV\Powers\Necromacy\Litcheye.bhvr
		Part	:Eyes_Cold_Glow_Core.part
		Part	:Eyes_Cold_Glow.part
		Part	:Eyes_Cold_Fire.part
	End

	Event
		Type	Local
		At	Prime
		AltPiv 	2
		BhvrOverride
			PositionOffset	0.1 0 0.2
		End
		BHVR	V_COV\Powers\Necromacy\Litcheye.bhvr
		Part	:Eyes_Cold_Glow_Core.part
		Part	:Eyes_Cold_Glow.part
		Part	:Eyes_Cold_Fire_Right.part
	End

End

End
##################################
