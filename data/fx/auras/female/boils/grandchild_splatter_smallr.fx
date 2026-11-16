#############################################################
## Sparks_Body.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Root
		Until	FLY
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	1.0 0.0 0.0
			StartJitter	0.75 0 0.75
		End
		Part	:Splash.part
		Part	:Ripples.part
		Lifespan 1
	End
End


#############################################################

End