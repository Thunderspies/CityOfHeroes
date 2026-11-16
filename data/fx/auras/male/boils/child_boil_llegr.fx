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
		At	LLegR
		Bhvr	:Boil_Scale_Legs.bhvr
		BhvrOverride
			PositionOffset	0.1 -0.5 -0.15
			StartJitter	0.1 0.5 0.1
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

Condition
	On	Time
	Time	28
	DoMany 	1
	Chance	0.5

	Event
		Type	Start
		At	Root
		Until	FLY
		ChildFX	:CHILD_Splatter_SmallR.fx
		Lifespan 1
	End
End

#############################################################

End