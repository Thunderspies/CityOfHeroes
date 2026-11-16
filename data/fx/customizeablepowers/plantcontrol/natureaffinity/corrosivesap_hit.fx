#                                                          
FxInfo

	Input
		InpName Chest
	End

	LifeSpan 40


###########################################################
Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	NA_CorrosiveSap_Hit_01 100 100 .8
	End
End

	Condition
		On Time
		Time 0

		#Event
			#At Chest
			#Type Local
			#EName SFX
			#Sound WB_WaterHit 100 100 0.8
		#End

		Event
			At Chest
			Type Local
			EName geom
			Bhvr Behaviors\SpinHit2.bhvr
			Geom CenterDummy
		End

		Event
			At Chest
			Type Local
			EName hit2
			Part CustomizeablePowers\WaterBlast\Hit_Streaks.part
			Part CustomizeablePowers\WaterBlast\Splash_Flash.part
			Part CustomizeablePowers\WaterBlast\Splash_Splatter_Add.part
			Part CustomizeablePowers\PlantControl\NatureAffinity\Corrosive_Fumes.part
			Part CustomizeablePowers\PlantControl\NatureAffinity\Corrosive_Fumes2.part
			#Part CustomizeablePowers\WaterBlast\Splash_Splatter2_Add.part
			LifeSpan 2
		End

	End

	Condition
		On Time
		Time 5

		Event
			At Chest
			Type Local
			EName hit2
			Part CustomizeablePowers\PlantControl\NatureAffinity\Corrosive_Fumes.part
			Part CustomizeablePowers\PlantControl\NatureAffinity\Corrosive_Fumes2.part
			LifeSpan 2
		End

	End

	Condition
		On TriggerBits
		TriggerBits MALE

		Event
			At Chest
			Type Local
			ChildFx CustomizeablePowers\WaterBlast\Hit_Drips_Child.fx
			EName Drips
		End

	End

	Condition
		On TriggerBits
		TriggerBits FEMALE

		Event
			At Chest
			Type Local
			ChildFx CustomizeablePowers\WaterBlast\Hit_Drips_Child.fx
			EName Drips
		End

	End

	Condition
		On TriggerBits
		TriggerBits HUGE

		Event
			At Chest
			Type Local
			ChildFx CustomizeablePowers\WaterBlast\Hit_Drips_Child.fx
			EName Drips
		End

	End

End
