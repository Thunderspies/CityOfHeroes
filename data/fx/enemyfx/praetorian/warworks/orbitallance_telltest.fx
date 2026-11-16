#############################################################
## OrbitalLance_Tell.fx
#############################################################

FxInfo
LifeSpan 180

#############################################################

Input
	InpName Hips
End

#############################################################


Condition
	On 	Time
	Time 	0

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
		Lifespan 80
	End
End

#############################################################

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy
	End
End

#############################################################

End