#############################################################
## Cryo_Body.fx
#############################################################

FxInfo

#########################################################

Condition
	On 	Time
	Time 	0

#########################################################

	Event
		EName 	EyeAnchor
		Type	Local
		At	Eyes
		Geom	CircleOfThorns
	End

	Event
		Type	Local
		At	EyeAnchor
		AltPiv	1
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 -0.06 -0.125
		End
		Part	:Eye_GlowL.part
		Part	:Eye_Glow.part
	End

	Event
		Type	Local
		At	EyeAnchor
		AltPiv	2
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 -0.06 -0.125
		End
		Part	:Eye_GlowR.part
		Part	:Eye_Glow.part
	End

#########################################################

	Event
		EName	UArmLFX
		Type	PositOnly
		At	Hair
		Part	:Mist_Cold_Detached.part
		Part	:Mist_Big.part
		Part	:Mist_Big_Trail.part
		POther	Neck
	End

	Event
		EName	LArmLFX
		Type	PositOnly
		At	LArmL
		Part	:Mist_Cold_Detached.part
		Part	:Mist_Big.part
		Part	:Mist_Big_Trail.part
		POther	HandL
	End

	Event
		EName	LArmRFX
		Type	PositOnly
		At	LArmR
		Part	:Mist_Cold_Detached.part
		Part	:Mist_Big.part
		Part	:Mist_Big_Trail.part
		POther	HandR
	End

	Event
		EName	LArmLFX
		Type	PositOnly
		At	HandL
		Part	:Mist_Cold_Detached.part
		Part	:Mist_Big.part
	End

	Event
		EName	LArmRFX
		Type	PositOnly
		At	HandR
		Part	:Mist_Cold_Detached.part
		Part	:Mist_Big.part
	End

#########################################################

End