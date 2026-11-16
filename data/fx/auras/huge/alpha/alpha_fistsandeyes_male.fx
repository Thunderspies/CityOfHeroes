#############################################################
## Alpha_FistsAndEyes_Male.fx
#############################################################

FxInfo
Flags NotCompatibleWithAnimalHead DontSuppress ##InheritAlpha - Don't use inheret alpha on Aura FX

#############################################################


Condition
	On	Time
	Time	0

#############################################################

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
		BhvrOverride
			PositionOffset	0.0 -0.05 -0.1
		End
		Part	:Star_Eyes.part
		Part	:Glow_Eyes.part
	End

	Event
		Type	Local
		At	EyeAnchor
		AltPiv	2
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 -0.05 -0.1
		End
		Part	:Star_Eyes.part
		Part	:Glow_Eyes.part
	End

#############################################################

	Event
		EName	Ring
		Type	Local
		At	HandL
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:LightRay_Right_Hand.part
		part	:LightRay_Left_Hand.part
		Part	:Star_Hand.part
		Part	:Star_Hand_Moving.part
		Part	:Glow_Large_Continuing_Chest.part
		Part	:Glow_Ring_Hand.part
	End

	Event
		EName	Ring
		Type	Local
		At	HandR
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:LightRay_Right_Hand.part
		part	:LightRay_Left_Hand.part
		Part	:Star_Hand.part
		Part	:Star_Hand_Moving.part
		Part	:Glow_Large_Continuing_Chest.part
		Part	:Glow_Ring_Hand.part
	End
End

#############################################################

End