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
		At	T_Eyes
		Geom	CircleOfThorns
	End

	Event
		Type	Local
		At	Hair
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 -0.1 0.1
		End
		Part	:Head_Hellfire_Glow.part
		Part	:Fire_Hellfire.part
		Part	:Fire_Hellfire_Trail_Small.part
		Part	:Fire_Hellfire_Alpha.part
		Part	:Fire_Hellfire_Trail_Small_Alpha.part
	End

	Event
		Type	Local
		At	Prime
		AltPiv 	1
		BHVR	V_COV\Powers\Necromacy\Litcheye.bhvr
		BhvrOverride
			PositionOffset	-0.05 0.25 0.3
		End
		Part	:Eyes_Hellfire_Glow.part
		Part	:Eyes_Hellfire_Fire_Left.part
		Part	:Eyes_Hellfire_Fire_Left_Alpha.part
	End

	Event
		Type	Local
		At	Prime
		AltPiv 	2
		BhvrOverride
			PositionOffset	0.05 0.25 0.3
		End
		BHVR	V_COV\Powers\Necromacy\Litcheye.bhvr
		Part	:Eyes_Hellfire_Glow.part
		Part	:Eyes_Hellfire_Fire.part
		Part	:Eyes_Hellfire_Fire_Alpha.part
	End

End

#########################################################

End
