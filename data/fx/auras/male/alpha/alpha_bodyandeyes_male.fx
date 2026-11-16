#############################################################
## Alpha_Body_Male.fx
#############################################################

FxInfo
Flags NotCompatibleWithAnimalHead DontSuppress

#############################################################

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
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Star_Eyes.part
		Part	:Glow_Eyes.part
	End

	Event
		Type	Local
		At	EyeAnchor
		AltPiv	2
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Star_Eyes.part
		Part	:Glow_Eyes.part
	End

#############################################################

	Event
		EName	Ring
		Type	Local
		At	Chest
		BhvrOverride
			PositionOffset	0.0 0.3 0.0
		End
		part	:LightRay_Up.part
		part	:LightRay_Down.part
		part	:LightRay_Right.part
		part	:LightRay_Left.part

		part	:Star_Body.part
		Part	:Glow_Ring.part
		Part	:Glow_Large_Continuing.part
	End

End

#############################################################

End