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
		EName	SpinnerAnchor
		Type	Local
		At	Origin
		BhvrOverride
			PyrRotateJitter	90 90 90
			Spin	0 0.2 0.0
		End
	End

	Event
		Type	Local
		At	SpinnerAnchor
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.6 0.0 0.0
			StartJitter	0.15 0.0 0.0
		End
		part	:Electron_Head_Core_Small.part
		part	:Electron_Head_Flare_Small.part
		part	:Electron_Trail_Small.part
		Part	:Electron_Trail_Bright_Small.part
	End

	Event
		Type	Local
		At	SpinnerAnchor
		Bhvr	behaviors/GenericParticleFade.bhvr

		part	:Electron_Trail_Stationary_Hand.part
	End
End

#############################################################

End