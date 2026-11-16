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
		At	Origin
		part	:LightRay_Right_Vigilante.part
		part	:LightRay_Left_Vigilante.part
		Part	:Star_Hand_Vigilante.part
		Part	:Glow_Large_Continuing_Vigilante.part
		Part	:Glow_Ring_Vigilante.part
	End
End

#############################################################

End