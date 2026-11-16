#
FxInfo

	Flags DontSuppress

	LifeSpan 150


###########################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	DC_FearsomeStare_Attack_01 100 100 .8
	End
End

	Condition
		On Time
		Time 0

		Event
			At Head
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :FearsomeStare_Trail_Large.part
			Part :FearsomeStare_Trail_Large_Solid.part
			LifeSpan 50
		End

		Event
			At Head
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			Part CustomizeablePowers\DarknessControl\FearsomeStare_Trail_Large_Highlights.part
			LifeSpan 40
		End

	End

	Condition
		On Time
		Time 5

		Event
			At Head
			Type Local
			Bhvr CustomizeablePowers\DarknessControl\FearsomeStare_Tunnel.bhvr
			Geom Tintable_DarkGrasp_Flipped
			LifeSpan 20
		End

	End

	Condition
		On Time
		Time 10

		Event
			At Head
			Type Local
			Bhvr CustomizeablePowers\DarknessControl\FearsomeStare_Tunnel.bhvr
			Geom Tintable_DarkGrasp_Flipped
			LifeSpan 20
		End

	End

	Condition
		On Time
		Time 15

		Event
			At Head
			Type Local
			Bhvr CustomizeablePowers\DarknessControl\FearsomeStare_Tunnel.bhvr
			Geom Tintable_DarkGrasp_Flipped
			LifeSpan 20
		End

	End

	Condition
		On Time
		Time 20

		Event
			At Head
			Type Local
			Bhvr CustomizeablePowers\DarknessControl\FearsomeStare_Tunnel.bhvr
			Geom Tintable_DarkGrasp_Flipped
			LifeSpan 20
		End

	End

	Condition
		On Time
		Time 25

		Event
			At Head
			Type Local
			Bhvr CustomizeablePowers\DarknessControl\FearsomeStare_Tunnel.bhvr
			Geom Tintable_DarkGrasp_Flipped
			LifeSpan 20
		End

	End

	Condition
		On Time
		Time 30

		Event
			At Head
			Type Local
			Bhvr CustomizeablePowers\DarknessControl\FearsomeStare_Tunnel.bhvr
			Geom Tintable_DarkGrasp_Flipped
			LifeSpan 20
		End

	End

	Condition
		On Time
		Time 35

		Event
			At Head
			Type Local
			Bhvr CustomizeablePowers\DarknessControl\FearsomeStare_Tunnel.bhvr
			Geom Tintable_DarkGrasp_Flipped
			LifeSpan 20
		End

	End

	Condition
		On TriggerBits
		TriggerBits MALE
		DoMany 1

		Event
			At Head
			Type Local
			ChildFx :CHILD_Possess_Eyes_Male.fx
			EName EyeGlows_Male
			Flags OneAtATime
			LifeSpan 75
		End

	End

	Condition
		On TriggerBits
		TriggerBits FEMALE

		Event
			At Head
			Type Local
			ChildFx :CHILD_Possess_Eyes_Female.fx
			EName EyeGlows_Female
			Flags OneAtATime
			LifeSpan 75
		End

	End

	Condition
		On TriggerBits
		TriggerBits HUGE
		DoMany 1

		Event
			At Head
			Type Local
			ChildFx :CHILD_Possess_Eyes_Huge.fx
			EName EyeGlows_Huge
			Flags OneAtATime
			LifeSpan 75
		End

	End

	Condition
		On Time
		Time 0

	End

End
