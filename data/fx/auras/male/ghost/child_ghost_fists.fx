#############################################################
## Costume Aura - Flies (Body)
#############################################################

FxInfo
LIfespan 80

########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	HeadAnchor
		Type	Local
		At	Origin
		BhvrOverride
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
			Spin		0.0 0.05 0.0
			SpinJitter 	0.1 0.025 0.1
		End
	End

	Event
		Type	Local
		At	Prime
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	1.0 0.0 0.0
			StartJitter	0.25 0.0 0.0
		End
		Part	:Ghost_Trail_Small.part
		Part	:Ghost_HalfBody_01_Small.part

		Lifespan 60
	End
End

#############################################################

End