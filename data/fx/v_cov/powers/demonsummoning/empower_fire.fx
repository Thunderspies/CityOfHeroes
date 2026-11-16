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
		BhvrOverride
			PositionOffset	0.0 -0.1 0.1
		End
		Part	:Fire_Fire.part
		Part	:Head_Fire_Glow.part

	End

	Event
		Type	Local
		At	Prime
		AltPiv 1
		BhvrOverride
			PositionOffset	-0.05 0.2 0.3
		End
		BHVR	V_COV\Powers\Necromacy\Litcheye.bhvr
		Part	:Eyes_Fire_Smoke_Left.part
		Part	:Eyes_Fire_Glow.part
		Part	:Eyes_Fire_Fire_Left.part

	End

	Event
		Type	Local
		At	Prime
		AltPiv 2
		BhvrOverride
			PositionOffset	0.05 0.2 0.3
		End
		BHVR	V_COV\Powers\Necromacy\Litcheye.bhvr
		Part	:Eyes_Fire_Smoke.part
		Part	:Eyes_Fire_Glow.part
		Part	:Eyes_Fire_Fire.part
	End

End

End
##################################
