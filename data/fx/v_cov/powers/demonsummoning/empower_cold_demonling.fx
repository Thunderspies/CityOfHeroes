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
		At	Head
		BhvrOverride
			PositionOffset	0 -.3 0.7
		End
		Part	:Fire_Cold_Demonling_Small.part
	#	Part	:Head_Cold_Glow.part
		Part	:Eyes_Cold_Glow_Demonling.part

	End

	Event
		Type	Local
		At	Prime
		AltPiv 1
		BHVR	V_COV\Powers\Necromacy\Litcheye.bhvr
		BhvrOverride
			PositionOffset	-0.135 0.3 0.3
		End
		Part	:Eyes_Cold_Glow_Demonling_Core.part
		Part	:Eyes_Cold_Glow_Demonling.part
		Part	:Eyes_Cold_Fire.part
	End

	Event
		Type	Local
		At	Prime
		AltPiv 2
		BHVR	V_COV\Powers\Necromacy\Litcheye.bhvr
		BhvrOverride
			PositionOffset	0.135 0.3 0.3
		End
		Part	:Eyes_Cold_Glow_Demonling_Core.part
		Part	:Eyes_Cold_Glow_Demonling.part
		Part	:Eyes_Cold_Fire_Right.part
	End

End

End
##################################
