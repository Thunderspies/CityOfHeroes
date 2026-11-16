#
FxInfo

	Flags DontSuppress

	LifeSpan 300


###########################################################


Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	DC_ShadowField_Hit_Loop 100 100 .15
		bhvr	behaviors/Sound/SoundIn1.bhvr
		lifespan 300
	End

		Event
			At Root
			Type Local
			EName CrushingHateGeo
			Bhvr CustomizeablePowers\DarknessControl\ShadowyBinds_Scale.bhvr
			Geom Tintable_ShadowyBinds
			LifeSpan 10
		End

		Event
			EName	Particles
			At Root
			Type Local
			Bhvr behaviors\GenericParticleFade.bhvr
			BhvrOverride
				Spin 0 -0.1000 0
			End
			Part CustomizeablePowers\DarknessControl\ShadowyBinds_Flat_Highlights.part
			Part :ShadowyBinds_Flat.part
			LifeSpan 35
		End

End

	Condition
		On Cycle
		Time 10

		Event
			At Root
			Type Local
			EName CrushingHateGeo
			Bhvr CustomizeablePowers\DarknessControl\ShadowyBinds_Scale.bhvr
			Geom Tintable_ShadowyBinds
			LifeSpan 10
		End

		Event
			EName	Particles
			At Root
			Type Local
			Bhvr behaviors\GenericParticleFade.bhvr
			BhvrOverride
				Spin 0 -0.1000 0
			End
			Part CustomizeablePowers\DarknessControl\ShadowyBinds_Flat_Highlights.part
			Part :ShadowyBinds_Flat.part
			LifeSpan 35
		End

	End

	Condition
		On Death

		Event
			Type Local
			EName CrushingHateGeo
		End

	End

End
