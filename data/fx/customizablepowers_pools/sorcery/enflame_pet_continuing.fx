#
FxInfo

	#LifeSpan 200


###########################################################

	Condition
		On time
		Time 0

		Event
			At Root
			Type StartPositOnly
			Part :Ward_Glow.part
			Part :RadStreaks_In.part
			Lifespan 15
		End

		Event
			at Root
			Type StartPositOnly
			Part :Enflame_Pet_Glow.part
			Part :Enflame_pet_fire_Flat.part
			Part :Enflame_pet_fire_Add_Flat.part
			Part :Enflame_pet_fire_add.part
			Part :Enflame_pet_fire.part
			Part :Init_pet_fire.part
			Part :Init_pet_fire_add.part
			Part :Init_pet_fire_Add_Flat.part
			Part :Init_pet_fire_Flat.part
			Part :Init_Pet_Glow.part
			LifeSpan 100
		End

	End

End
