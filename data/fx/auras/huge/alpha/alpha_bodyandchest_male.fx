#############################################################
## Alpha_Body_Male.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

#############################################################
	Event
		EName	Ring
		Type	Local
		At	Chest
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