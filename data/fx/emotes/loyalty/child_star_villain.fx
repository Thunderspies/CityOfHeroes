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
		Part	:Star_Hand_Villain.part
		Part	:Glow_Large_Continuing_Villain.part
		Part	:Glow_Large_Continuing_Villain_Alpha.part

		Part	:Glow_Ring_Villain.part
	End
End

#############################################################

End