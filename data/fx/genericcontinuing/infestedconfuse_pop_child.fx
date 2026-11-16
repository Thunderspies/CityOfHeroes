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
		Bhvr	Auras\Male\Boils\Boil_Scale.bhvr
		BhvrOverride
			#TintGeom	1
			StartColor	47 243 250
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
		At	Origin
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Infested_Splash_Droplets_Small.part
		Part	:Infested_Splash_Small.part
	End
End

Condition
	On	Time
	Time	23
	Chance	0.75

	Event
		Type	PositOnly
		At	Origin
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Infested_Strands.part
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
		ChildFX	:InfestedSplatter_CHILD.fx
		Lifespan 1
	End
End


#############################################################

End