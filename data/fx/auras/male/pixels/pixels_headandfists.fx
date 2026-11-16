#############################################################
## Pixels_Body.fx
#############################################################

FxInfo

Flags NotCompatibleWithAnimalHead

#########################################################

##########		EYES

Condition
	On 	Time
	Time 	0

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
		Part	:Pixel_EyeL.part
		Part	:Eye_Glow.part
	End

	Event
		Type	Local
		At	EyeAnchor
		AltPiv	2
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Pixel_EyeR.part
		Part	:Eye_Glow.part
	End

###############################


	Event
		Type	Local
		At	Head
		BhvrOverride
			PositionOffset	0 0 0
		End
		Part	:Pixel_Sm.part
		Part	:Pixel_Run.part
		Part	:Pixel_Run.part
		Part	:Pixel_Run2.part
		Part	:Pixel_Run2.part
	End


	Event
		Type	Local
		At	Head
		Bhvr	Auras\Male\Glow\HeadOffset1.bhvr
		BhvrOverride
			PositionOffset	0 0.2 0
		End
		part	:Pixel_BodyGlow01.part
		Pmagnet Head
	End


#######################		FISTS

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