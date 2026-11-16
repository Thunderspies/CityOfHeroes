#############################################################
## Android_Plasma_WeldingRay_Continuing.fx
#############################################################

FxInfo

#############################################################

Input
	InpName Hips
End

#############################################################


Condition
	On 	Time
	Time 	0

	Event
		Type	PositOnly
		At	Head
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 -0.25 0.0
		End
		Part	:Plasma_Hand_Glow.part
		Part	:Plasma_Core.part
		Part	:Plasma_Long.part
		Part	:Plasma_Highlight_Hands.part
		POther	Hair
	End
End

#############################################################

End