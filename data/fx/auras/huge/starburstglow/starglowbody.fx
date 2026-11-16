#############################################################
## StarGlowBody.fx
#############################################################

FxInfo

Flags DontSuppress ##InheritAlpha - Don't use inheret alpha on Aura FX

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Head
		Bhvr	:HeadOffset1.bhvr
		part	:BodyStarGlow01.part
		part	:BodyStarGlow02.part
		Pmagnet Head
	End

	Event
		Type	Local
		At	Chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodyStarGlow01.part
		part	:BodyStarGlow02.part
		Pmagnet Neck
		POther  Neck
	End

	Event
		Type	Local
		At	Hair
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodyStarGlow01.part
		part	:BodyStarGlow02.part
		Pmagnet Chest
		POther  Chest
	End

	Event
		Type	Local
		At	Chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodyStarGlow01.part
		part	:BodyStarGlow02.part
		Pmagnet Hips
		POther  Hips
	End

	Event
		Type	Local
		At	Hips
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodyStarGlow01.part
		part	:BodyStarGlow02.part
		Pmagnet UArmL
		POther  UArmL
	End

	Event
		Type	Local
		At	Hips
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodyStarGlow01.part
		part	:BodyStarGlow02.part
		Pmagnet UarmR
		POther  UarmR
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	UarmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodyStarGlow01.part
		part	:BodyStarGlow02.part
		Pmagnet UarmR
	End

	Event
		Type	Local
		At	UarmL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodyStarGlow01.part
		part	:BodyStarGlow02.part
		Pmagnet UarmL
	End

	Event
		Type	Local
		At	UarmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodyStarGlow01.part
		part	:BodyStarGlow02.part
		Pmagnet LarmR
		POther  LarmR
	End

	Event
		Type	Local
		At	HandR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodyStarGlow01.part
		part	:BodyStarGlow02.part
		Pmagnet LarmR
		POther  LarmR
	End

	Event
		Type	Local
		At	UarmL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodyStarGlow01.part
		part	:BodyStarGlow02.part
		Pmagnet LarmL
		POther  LarmL
	End

	Event
		Type	Local
		At	HandL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodyStarGlow01.part
		part	:BodyStarGlow02.part
		Pmagnet LarmL
		POther  LarmL
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	UlegL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:UlegStarGlow01.part
		part	:BodyStarGlow02.part
		Pmagnet Llegl
		POther  Llegl
	End

	Event
		Type	Local
		At	UlegR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:UlegStarGlow01.part
		part	:BodyStarGlow02.part
		Pmagnet LlegR
		POther  LlegR
	End

	Event
		Type	Local
		At	LlegR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodyStarGlow01.part
		part	:BodyStarGlow02.part
		Pmagnet FootR
		POther  FootR
	End

	Event
		Type	Local
		At	LlegL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodyStarGlow01.part
		part	:BodyStarGlow02.part
		Pmagnet FootL
		POther  FootL
	End

	Event
		Type	Local
		At	FootR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodyStarGlow01.part
		part	:BodyStarGlow02.part
		Pmagnet FootR
	End

	Event
		Type	Local
		At	FootL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodyStarGlow01.part
		part	:BodyStarGlow02.part
		Pmagnet FootL
	End
End

#############################################################

End