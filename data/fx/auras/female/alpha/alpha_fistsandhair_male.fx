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
		EName	HeadAnchor
		Type	Local
		At	Head
		BhvrOverride
			PositionOffset	0.0 0.175 0.0
		End
	End

	Event
		EName	Ring
		Type	Local
		At	HeadAnchor

		Part	:Star_Head.part
		Part	:Star_Hand_Moving.part
		Part	:Glow_Large_Continuing_Chest.part
		Part	:Glow_Ring_Head.part
	End

#############################################################

	Event
		EName	Ring
		Type	Local
		At	WepL

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
		At	WepR

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