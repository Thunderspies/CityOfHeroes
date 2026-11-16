#############################################################
## Sparks_Body.fx
#############################################################

FxInfo
Flags InheritGeoScale InheritAlpha

#############################################################

Condition
	On	Time
	Time	0


	Event
		EName	TumorGeo
		Type	Local
		At	ULegL
		Bhvr	:Boil_Scale_Legs.bhvr
		BhvrOverride
			PositionOffset	-0.175 -0.7 0.2
			StartJitter	0.1 0.7 0.1
			TintGeom	1
		End
		Geom	FX_Tintable_Boil
		Lifespan 25
	End
End

Condition
	On	Time
	Time	23

	Event
		EName	Arm
		Type	Local
		At	TumorGeo
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Toxic_Splash_Droplets.part
		Part	:Toxic_Splash_Main.part
	End
End

Condition
	On	Time
	Time	23
	Chance	0.33

	Event
		Type	PositOnly
		At	TumorGeo
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Toxic_Strands.part
	End
End

#############################################################

End