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
		Bhvr	:Boil_Scale_Legs.bhvr
		BhvrOverride
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
		At	Origin
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
		At	Origin
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Toxic_Strands.part
	End
End

#############################################################

End