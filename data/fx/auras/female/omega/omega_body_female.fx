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
		At	Neck
		Part	:Smoke_Body.part
		Part	:Smoke_Body_Highlight.part
		POther  Head
	End

	Event
		Type	Local
		At	Hips
		Part	:Plasma_Core_Trail.part
		Part	:Smoke_Body_Highlight_Front.part
	End

	Event
		Type	Local
		At	UArmL
		Part	:Smoke_Body.part
		Part	:Smoke_Body_Highlight.part
		Part	:Plasma_Core.part
		Part	:Plasma_Core_Trail.part
		Part	:Smoke_Body_Trail.part
		Part	:Smoke_Body_Trail_Small.part
		POther  Head
	End

	Event
		Type	Local
		At	UArmR
		Part	:Smoke_Body.part
		Part	:Smoke_Body_Highlight.part
		Part	:Plasma_Core.part
		Part	:Plasma_Core_Trail.part
		Part	:Smoke_Body_Trail.part
		Part	:Smoke_Body_Trail_Small.part
		POther  Head
	End


	Event
		Type	Local
		At	HandR
		Part	:Smoke_Hands.part
		Part	:Plasma_Core.part
		Part	:Plasma_Core_Trail.part
		POther 	UarmR
	End

	Event
		Type	Local
		At	HandL
		Part	:Smoke_Hands.part
		Part	:Plasma_Core.part
		Part	:Plasma_Core_Trail.part
		POther 	UarmL
	End

	Event
		Type	Local
		At	HandL
		Part	:Plasma_Core.part
		POther	LArmL
	End

	Event
		Type	Local
		At	HandR
		Part	:Plasma_Core.part
		POther	LArmR

	End

	Event
		Type	Local
		At	LLegR
		Part	:Smoke_Body.part
		Part	:Plasma_Core.part
		Part	:Plasma_Core_Trail.part
		Part	:Smoke_Body_Trail.part
		Part	:Smoke_Body_Trail_Small.part
		POther 	ULegR
	End

	Event
		Type	Local
		At	LLegL
		Part	:Smoke_Body.part
		Part	:Plasma_Core.part
		Part	:Plasma_Core_Trail.part
		Part	:Smoke_Body_Trail.part
		Part	:Smoke_Body_Trail_Small.part
		POther 	ULegL
	End


	Event
		Type	Local
		At	FootR
		Part	:Smoke_Hands.part
		Part	:Plasma_Core_Big.part
		Part	:Plasma_Core_Trail.part
		POther 	LLegR
	End

	Event
		Type	Local
		At	FootL
		Part	:Smoke_Hands.part
		Part	:Plasma_Core_Big.part
		Part	:Plasma_Core_Trail.part
		POther 	LLegL
	End

	Event
		Type	Local
		At	FootR
		Part	:Plasma_Core_Big.part
	End

	Event
		Type	Local
		At	FootL
		Part	:Plasma_Core_Big.part
	End
End

#########################################################

End