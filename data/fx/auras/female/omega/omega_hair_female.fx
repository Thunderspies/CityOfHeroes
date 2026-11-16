#############################################################
## Omega_Hair_Huge.fx
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
		At	Head
		BhvrOverride
			PositionOffset	0.0 0.25 0.0
		End
		Part	:Plasma_Core.part
		Part	:Plasma_Core_Trail.part
		Part	:Smoke_Body.part
		Part	:Smoke_Body_Highlight.part
		Part	:Smoke_Body_Trail.part
		Part	:Smoke_Body_Trail_Small.part
		POther  Head
	End
End

#########################################################

End