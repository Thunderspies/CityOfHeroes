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
		Part	:Glow_Large_Highlight.part
		Part	:Glow_Large_Continuing.part
	End

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
		part	:LightRay_UpLeft.part
		part	:LightRay_UpRight.part
		part	:LightRay_DownLeft.part
		part	:LightRay_DownRight.part
	End

#############################################################

	Event
		EName	Ring
		Type	PositOnly
		At	WepL

		part	:LightRay_Up_Head.part
		part	:LightRay_Down_Head.part
		part	:LightRay_Right_Head.part
		part	:LightRay_Left_Head.part
		part	:LightRay_UpLeft_Head.part
		part	:LightRay_UpRight_Head.part
		part	:LightRay_DownLeft_Head.part
		part	:LightRay_DownRight_Head.part

		Part	:Glow_Large_Continuing_Belt.part
		Part	:Glow_Ring_Chest.part

	End

	Event
		EName	Ring
		Type	PositOnly
		At	WepR

		part	:LightRay_Up_Head.part
		part	:LightRay_Down_Head.part
		part	:LightRay_Right_Head.part
		part	:LightRay_Left_Head.part
		part	:LightRay_UpLeft_Head.part
		part	:LightRay_UpRight_Head.part
		part	:LightRay_DownLeft_Head.part
		part	:LightRay_DownRight_Head.part

		Part	:Glow_Large_Continuing_Belt.part
		Part	:Glow_Ring_Chest.part

	End
End

#Condition
#	On	Time
#	Time	10
#
#	Event
#		EName	Ring
#		Type	Local
#		At	Chest
#		Part	:Glow_Ring_Quick.part
#	End
#End

#############################################################

End