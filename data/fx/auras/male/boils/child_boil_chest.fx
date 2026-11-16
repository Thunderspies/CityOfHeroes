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
		At	Chest
		Bhvr	:Boil_Scale_Chest.bhvr
		BhvrOverride
			PositionOffset	0.0 0.0 0.15
			StartJitter	0.5 0.4 0.3
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
#############################################################

End