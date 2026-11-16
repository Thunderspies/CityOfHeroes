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
		At	HandL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Plasma_Hand_Glow.part
		Part	:Plasma_Core_Hands.part
		Part	:Plasma_Long.part
		Part	:Plasma_Highlight_Hands.part
		POther	LArmL
	End
End

#############################################################

End