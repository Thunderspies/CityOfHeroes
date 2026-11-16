#############################################################
## Pixels_FistsAndEyes.fx
#############################################################

FxInfo

#########################################################

Condition
	On 	Time
	Time 	0

#######		EYES

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
			PositionOffset	0.0 -0.05 -0.1
		End
		Part	:Pixel_EyeL.part
		Part	:Eye_Glow.part
	End

	Event
		Type	Local
		At	EyeAnchor
		AltPiv	2
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 -0.05 -0.1
		End
		Part	:Pixel_EyeR.part
		Part	:Eye_Glow.part
	End

################################################

	Event
		Type	Local
		At	HandR
		Part	:Pixel_Sm.part
		Part	:Pixel_Run_Sm.part
		Part	:Pixel_Run_Sm.part
		#Part	:Pixel_Run2.part
	End

	Event
		Type	Local
		At	HandL
		Part	:Pixel_Sm.part
		Part	:Pixel_Run_Sm.part
		Part	:Pixel_Run_Sm.part
		#Part	:Pixel_Run2.part
	End


#######		GLOW

	Event
		Type	Local
		At	HandR
		part	:Pixel_Handglow.part
		Pmagnet LarmR
		POther  LarmR
	End

	Event
		Type	Local
		At	HandR
		part	:Pixel_HandGlow.part
	End

	Event
		Type	Local
		At	HandL
		part	:Pixel_Handglow.part
		Pmagnet LarmL
		POther  LarmL
	End

	Event
		Type	Local
		At	HandL
		part	:Pixel_HandGlow.part
	End
End
#########################################################
End