#############################################################
## FairyDust_Eyes.fx
#############################################################

FxInfo
Flags	DontSuppress

#########################################################

Condition
	On	Time
	Time	0

	Event
		EName 	EyeAnchor
		Type	Local
		At	Eyes
		Bhvr	behaviors/GenericParticleFade.bhvr
		Geom	CircleOfThorns
	End

	Event
		Type	Local
		At	EyeAnchor
		AltPiv	1
		BhvrOverride
			PositionOffset	0.0 0.02 -0.1
		End
		Part	:EyeGlow_Pupil.part
		Part	:EyeGlow_BG.part
		Part	:Dust_Stationary_Eyes.part
	End

	Event
		Type	Local
		At	EyeAnchor
		AltPiv	2
		BhvrOverride
			PositionOffset	0.0 0.02 -0.1
		End
		Part	:EyeGlow_Pupil.part
		Part	:EyeGlow_BG.part
		Part	:Dust_Stationary_Eyes.part
	End
End

#############################################################

End