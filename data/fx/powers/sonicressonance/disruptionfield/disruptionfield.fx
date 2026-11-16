#############################################################
## Disruption Field
#############################################################

FxInfo

#############################################################

Condition
	On	Time	
	Time 	7

	Event
		Type 	Local
		At	Origin
		Sound sonichands 80 80 .8
	End
End

Condition
	On	Time	
	Time 	17

	Event
		Type 	Local
		At	Origin
		Sound sonic2_loop 100.0 100.0 .77
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan	30
	End
End

Condition
	On 	Time
	Time 	30

	Event
		Type	Local
		At	Chest
		Sound sonic3 70 70 .75
	End
End

## HANDS ###########################################################

Condition
	On 	Time
	Time 	16

	Event
		Type	Local 
		At	HandL
		Bhvr	behaviors\GenericParticleFadeQuick.bhvr
		BhvrOverride
			PyrRotate	0 0 -90
		End
		Part	POWERS\SonicRessonance\SonicRings2.part
		LifeSpan	30
	End
End

Condition
	On 	Time
	Time 	16

	Event
		Type	Local 
		At	HandR
		Bhvr	behaviors\GenericParticleFadeQuick.bhvr
		BhvrOverride
			PyrRotate	0 0 90
		End
		Part	POWERS\SonicRessonance\SonicRings2.part
		LifeSpan	30
	End
End

#############################################################

End