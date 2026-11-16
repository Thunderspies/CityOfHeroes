#############################################################
## Pixels_Body.fx
#############################################################

FxInfo

Flags NotCompatibleWithAnimalHead

#########################################################

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

#########################################################

#######		PIXELS

	Event
		Type	Local
		At	Chest
		BhvrOverride
			PositionOffset	0 0.5 0
		End
		Part	:Pixel_Square.part
		Part	:Pixel_Run.part
		Part	:Pixel_Run2.part
	End

	Event
		Type	Local
		At	Hips
		Part	:Pixel_Sm.part
		Part	:Pixel_Run.part
		Part	:Pixel_Run2.part
	End

	Event
		Type	Local
		At	HandL
		Part	:Pixel_Sm2.part
	End
	Event
		Type	Local
		At	HandR
		Part	:Pixel_Sm2.part
	End

	Event
		Type	Local
		At	LArmL
		Part	:Pixel_Sm.part
		Part	:Pixel_Run.part
		Part	:Pixel_Run2.part
	End

	Event
		Type	Local
		At	LArmR
		Part	:Pixel_Sm.part
		Part	:Pixel_Run.part
		Part	:Pixel_Run2.part
	End

	Event
		Type	Local
		At	UArmL
		Part	:Pixel_Sm.part
		Part	:Pixel_Run.part
		Part	:Pixel_Run2.part
	End

	Event
		Type	Local
		At	UArmR
		Part	:Pixel_Sm.part
		Part	:Pixel_Run.part
		Part	:Pixel_Run2.part
	End

	Event
		Type	Local
		At	ULegL
		Part	:Pixel_Bg.part
	End

	Event
		Type	Local
		At	ULegR
		Part	:Pixel_Bg.part
		Part	:Pixel_Run.part
		Part	:Pixel_Run2.part
	End

	Event
		Type	Local
		At	LLegL
		Part	:Pixel_Bg.part
		Part	:Pixel_Run.part
		Part	:Pixel_Run2.part
	End

	Event
		Type	Local
		At	LLegR
		Part	:Pixel_Bg.part
		Part	:Pixel_Run.part
		Part	:Pixel_Run2.part
	End

	Event
		Type	Local
		At	FootL
		Part	:Pixel_Sm.part
	End

	Event
		Type	Local
		At	FootR
		Part	:Pixel_Sm.part
	End

#######		GLOW

	Event
		Type	Local
		At	Head
		Bhvr	Auras\Male\Glow\HeadOffset1.bhvr
		part	:Pixel_BodyGlow01.part
		Pmagnet Head
	End

	Event
		Type	Local
		At	Head
		part	:Pixel_BodyGlow01.part
		Pmagnet Chest
		POther  Chest
	End


	Event
		Type	Local
		At	Chest
		part	:Pixel_BodyGlow01.part
		Pmagnet Neck
		POther  Neck
	End

	Event
		Type	Local
		At	Chest
		part	:Pixel_BodyGlow01.part
		Pmagnet Hips
		POther  Hips
	End

	Event
		Type	Local
		At	Hips
		part	:Pixel_BodyGlow01.part
		Pmagnet UArmL
		POther  UArmL
	End

	Event
		Type	Local
		At	Hips
		part	:Pixel_BodyGlow01.part
		Pmagnet UarmR
		POther  UarmR
	End

	Event
		Type	Local
		At	UarmR
		part	:Pixel_BodyGlow01.part
		Pmagnet LarmR
		POther  LarmR
	End

	Event
		Type	Local
		At	UarmL
		part	:Pixel_BodyGlow01.part
		Pmagnet LarmL
		POther  LarmL
	End

	Event
		Type	Local
		At	HandL
		part	:Pixel_HandGlow.part
		Pmagnet LarmL
		POther  LarmL
	End

	Event
		Type	Local
		At	HandR
		part	:Pixel_HandGlow.part
		Pmagnet LarmR
		POther  LarmR
	End


	Event
		Type	Local
		At	UlegL
		part	:Pixel_UlegGlow01.part
		Pmagnet Llegl
		POther  Llegl
	End

	Event
		Type	Local
		At	UlegR
		part	:Pixel_UlegGlow01.part
		Pmagnet LlegR
		POther  LlegR
	End

	Event
		Type	Local
		At	LlegR
		part	:Pixel_BodyGlow01.part
		Pmagnet FootR
		POther  FootR
	End

	Event
		Type	Local
		At	LlegL
		part	:Pixel_BodyGlow01.part
		Pmagnet FootL
		POther  FootL
	End

	Event
		Type	Local
		At	FootR
		part	:Pixel_FootGlow01.part
		Pmagnet FootR
	End

	Event
		Type	Local
		At	FootL
		part	:Pixel_FootGlow01.part
		Pmagnet FootL
	End
End
#########################################################
End