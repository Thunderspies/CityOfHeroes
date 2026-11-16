#############################################################
## Omega_FistR_Huge.fx
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
		At	UArmR
		Part	:Smoke_Body.part
		Part	:Smoke_Body_Highlight_Front.part
		Part	:Plasma_Core.part
		Part	:Plasma_Core_Trail.part
		Part	:Smoke_Body_Trail.part
		Part	:Smoke_Body_Trail_Small.part
		POther 	HandR
	End

	Event
		Type	Local
		At	LArmR
		Part	:Smoke_Hands.part
		Part	:Plasma_Core.part
		Part	:Plasma_Core_Trail.part
		POther	HandR
	End

	Event
		Type	Local
		At	HandR
		Part	:Plasma_Core_Big.part
	End

End

#########################################################

End