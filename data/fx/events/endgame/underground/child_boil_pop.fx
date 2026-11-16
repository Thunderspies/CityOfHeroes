#############################################################
## Sparks_Body.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	TumorGeo
		Type	Local
		At	Origin
		Bhvr	:Boil_Scale.bhvr
		BhvrOverride
			StartColor	50 130 150
		End
		Geom	FX_Tintable_Boil
		Lifespan 40
	End

End

Condition
	On	Time
	Time	40
	Chance	0.5

	Event
		Type	Local
		At	Origin
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Toxic_Splash_Small.part

	End
End



#############################################################

End