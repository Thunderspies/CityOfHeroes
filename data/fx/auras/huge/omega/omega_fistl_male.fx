#############################################################
## Omega_Body_Huge.fx
#############################################################

FxInfo

#########################################################
#################### BODY GLOW 1 ########################
#########################################################



Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	UArmL
		Part	:Smoke_Body.part
		Part	:Smoke_Body_Highlight_Front.part
		Part	:Plasma_Core.part
		Part	:Plasma_Core_Trail.part
		Part	:Smoke_Body_Trail.part
		Part	:Smoke_Body_Trail_Small.part
		POther 	HandL
	End

	Event
		Type	Local
		At	LArmL
		Part	:Smoke_Hands.part
		Part	:Plasma_Core.part
		Part	:Plasma_Core_Trail.part

		POther	HandL
	End

	Event
		Type	Local
		At	HandL
		Part	:Plasma_Core_Big.part
	End

End

#########################################################

End