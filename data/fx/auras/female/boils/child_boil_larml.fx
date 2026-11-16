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
		At	LArmL
		Bhvr	:Boil_Scale.bhvr
		BhvrOverride
			PositionOffset	-0.25 -0.025 0.0
			StartJitter	0.25 0.0 0.1
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
		Type	Local
		At	TumorGeo
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Toxic_Splash_Droplets_Small.part
		Part	:Toxic_Splash_Small.part
	End
End

Condition
	On	Time
	Time	23
	Chance	0.5

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

	Event
		Type	Start
		At	Root
		Until	FLY
		ChildFX	:CHILD_Splatter_Small.fx
		Lifespan 1
	End
End


#############################################################

End