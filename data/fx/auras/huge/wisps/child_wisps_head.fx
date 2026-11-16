#############################################################
## Costume Aura - Flies (Body)
#############################################################

FxInfo
LIfespan 60
########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	HeadAnchor
		Type	Local
		At	Head
		BhvrOverride
			PositionOffset	0.0 0.35 0.0
			PyrRotateJitter	90 90 90
			Spin	0 0.2 0.0
		End
	End

	Event
		EName	Prime
		Type	Start
		At	HeadAnchor
		BhvrOverride
			PositionOffset	0.0 0.0 0.0
			PyrRotateJitter	90 90 90
			Spin		0.0 0.1 0.0
			SpinJitter 	0.1 0.05 0.1
		End
	End

	Event
		Type	Local
		At	Prime
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	1.0 0.0 0.0
			StartJitter	0.5 0.0 0.0
		End
		Part	:Soul_Trail_Small.part
		Part	:Soul_Ring_Small.part
		Part	:Soul_Core_Small.part
		Part	:Soul_Trail_Small_Moving.part
		Lifespan 55
	End
End

#############################################################

End