#############################################################
## Alpha_Body_Male.fx
#############################################################

FxInfo
Flags DontSuppress

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	Ring
		Type	Local
		At	Chest
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 0.35 0.6
		End
		part	:LightRay_Right_Chest.part
		part	:LightRay_Left_Chest.part
		Part	:Star_Chest.part
		Part	:Star_Chest_Moving.part
		Part	:Glow_Large_Continuing_Chest.part
		Part	:Glow_Ring_Chest.part
	End

End

#############################################################

End