#############################################################
## Android_Plasma_ChargedBlast.fx
#############################################################

FxInfo
LifeSpan 240

#############################################################

Input
	InpName Hips
End

#############################################################


Condition
	On	Time
	Time	0

	Event
		Type	Start
		At	Root
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			Spin		0.0 0.025 0.0
			PositionOffset	0.0 0.5 0.0
		End
		Part	:Target_Reticle.part
		Part	:Target_Reticle_Glow.part
		Part	:Target_Rings_Expanding.part
		Sound WW_OrbitalLance_Tell_01 200 200 .8
		Lifespan 120
	End
End

#############################################################

End