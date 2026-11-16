#############################################################
## Alpha_Fists_Male.fx
#############################################################

FxInfo
Flags DontSuppress ##InheritAlpha - Don't use inheret alpha on Aura FX

#############################################################

Condition
	On	Time
	Time	0

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