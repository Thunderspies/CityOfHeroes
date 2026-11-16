#############################################################
## Alpha_Body_Male.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	Ring
		Type	Local
		At	WepL
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