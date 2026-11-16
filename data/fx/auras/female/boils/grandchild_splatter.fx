#############################################################
## Sparks_Body.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Start
		At	Root
		Until	FLY
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			StartJitter	1.5 0 1.0
		End
		Part	:Splash.part
		Part	:Ripples.part
		Lifespan 1
	End
End


#############################################################

End