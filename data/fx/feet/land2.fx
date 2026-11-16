#########################################################
##	Female	Lands
#########################################################

FxInfo

Input
	InpName	Origin
End

LifeSpan	1

Condition
	TriggerBits	CONCRETE
	Event
	Type	Local
	At	Origin
		SoundNoRepeat 2
	Sound FF_FemSidewalk_01 50 50 0.5
	Sound FF_FemSidewalk_03 50 50 0.5
	Sound FF_FemSidewalk_05 50 50 0.35
	Sound FF_FemSidewalk_07 50 50 0.5
	Sound FF_FemSidewalk_09 50 50 0.5
	End
End

Condition
	TriggerBits	ASPHALT
	Event
	Type	Local
	At	Origin
		SoundNoRepeat 2
	Sound FF_FemStreet_01 50 50 0.7
	Sound FF_FemStreet_03 50 50 0.7
	Sound FF_FemStreet_05 50 50 0.7
	Sound FF_FemStreet_07 50 50 0.7
	Sound FF_FemStreet_09 50 50 0.7
	End
End

	Condition
		TriggerBits	GRASS
		Event
		Type	Local
		At	Origin
		SoundNoRepeat 2
		Sound FF_Grass_01 50 50 0.2
		Sound FF_Grass_03 50 50 0.2
		Sound FF_Grass_05 50 50 0.2
		Sound FF_Grass_07 50 50 0.2
		Sound FF_Grass_09 50 50 0.2
		End
	End

Condition
	TriggerBits	DIRT
	Event
	Type	Local
	At	Origin
		SoundNoRepeat 2
	Sound FF_Dirt_01 50 50 0.3
	Sound FF_Dirt_03 50 50 0.3
	Sound FF_Dirt_05 50 50 0.3
	Sound FF_Dirt_07 50 50 0.3
	Sound FF_Dirt_09 50 50 0.3
	End
End

	Condition
		TriggerBits	SAND
		Event
		Type	Local
		At	Origin
		SoundNoRepeat 2
		Sound FF_Sand_01 50 50 0.3
		Sound FF_Sand_03 50 50 0.3
		Sound FF_Sand_05 50 50 0.3
		Sound FF_Sand_07 50 50 0.3
		Sound FF_Sand_09 50 50 0.3
		End
	End

Condition
	TriggerBits	GRAVEL
	Event
	Type	Local
	At	Origin
		SoundNoRepeat 2
	Sound FF_Gravel_01 50 50 0.5
	Sound FF_Gravel_03 50 50 0.5
	Sound FF_Gravel_05 50 50 0.5
	Sound FF_Gravel_07 50 50 0.5
	Sound FF_Gravel_09 50 50 0.5
	End
End

	Condition
		TriggerBits	PUDDLE
		Event
		Type	Local
		At	Origin
		SoundNoRepeat 2
		Sound FF_Puddle_01 50 50 0.2
		Sound FF_Puddle_03 50 50 0.2
		Sound FF_Puddle_05 50 50 0.2
		Sound FF_Puddle_07 50 50 0.2
		Sound FF_Puddle_09 50 50 0.2
		End
	End
	Condition
		TriggerBits	PUDDLE
		Chance	0.07
		Event
		Type	Local
		At	Origin
		SoundNoRepeat 2
		Sound WaterExit1 50 50 0.07
		Sound WaterExit2 50 50 0.07
		Sound WaterExit3 50 50 0.07
		Sound WaterExit4 50 50 0.07
		Sound WaterExit5 50 50 0.07
		End
	End

Condition
	TriggerBits	INWATER
	Event
	Type	Local
	At	Origin
		SoundNoRepeat 2
	Sound FF_Water_01 50 50 0.5
	Sound FF_Water_03 50 50 0.5
	Sound FF_Water_05 50 50 0.5
	Sound FF_Water_07 50 50 0.5
	Sound FF_Water_09 50 50 0.5
	End
End

	Condition
		TriggerBits	CATWALK
		Event
		Type	Local
		At	Origin
		SoundNoRepeat 2
		Sound FF_Catwalk_01 50 50 0.27
		Sound FF_Catwalk_03 50 50 0.27
		Sound FF_Catwalk_05 50 50 0.27
		Sound FF_Catwalk_07 50 50 0.27
		Sound FF_Catwalk_09 50 50 0.27
		End
	Event
		Type	Local
		At 		Origin
		SoundNoRepeat 1
		Sound FF_Catwalk_Sweetener_01 50 50 .07
		Sound FF_Catwalk_Sweetener_02 50 50 .07
		Sound FF_Catwalk_Sweetener_03 50 50 .07
		Sound FF_Catwalk_Sweetener_04 50 50 .07
	End
	End

	Condition
		TriggerBits	METALPLATE
		Event
		Type	Local
		At	Origin
		SoundNoRepeat 2
		Sound FF_MetalPlate_01 50 50 0.5
		Sound FF_MetalPlate_03 50 50 0.5
		Sound FF_MetalPlate_05 50 50 0.5
		Sound FF_MetalPlate_07 50 50 0.5
		Sound FF_MetalPlate_09 50 50 0.5
		End
	End

Condition
	TriggerBits	METAL
	Event
	Type	Local
	At	Origin
		SoundNoRepeat 2
	Sound FF_Metal_01 50 50 0.18
	Sound FF_Metal_03 50 50 0.18
	Sound FF_Metal_05 50 50 0.18
	Sound FF_Metal_07 50 50 0.18
	Sound FF_Metal_09 50 50 0.18
	End
	Event
		Type	Local
		At 		Origin
		SoundNoRepeat 1
		Sound FF_Metal_Sweetener_01 50 50 .2
		Sound FF_Metal_Sweetener_02 50 50 .2
		Sound FF_Metal_Sweetener_03 50 50 .2
		Sound FF_Metal_Sweetener_04 50 50 .2
	End
End

Condition
	TriggerBits	WOOD
	Event
	Type	Local
	At	Origin
		SoundNoRepeat 2
	Sound FF_FemWood_01 50 50 0.6
	Sound FF_FemWood_03 50 50 0.6
	Sound FF_FemWood_05 50 50 0.6
	Sound FF_FemWood_07 50 50 0.6
	Sound FF_FemWood_09 50 50 0.6
	End
End

Condition
	TriggerBits	WOODHOLLOW
	Event
		Type	Local
		At	Origin
		SoundNoRepeat 2
		Sound FF_FemWoodHollow_01 50 50 .35
		Sound FF_FemWoodHollow_03 50 50 .35
		Sound FF_FemWoodHollow_05 50 50 .35
		Sound FF_FemWoodHollow_07 50 50 .35
		Sound FF_FemWoodHollow_09 50 50 .35
	End
End
Condition
	TriggerBits WOODHOLLOW
	Chance	0.4
	Event
		Type	Local
		At 		Origin
		SoundNoRepeat 2
		Sound FF_WoodHollow_Sweetener_01 50 50 .075
		Sound FF_WoodHollow_Sweetener_02 50 50 .075
		Sound FF_WoodHollow_Sweetener_03 50 50 .075
		Sound FF_WoodHollow_Sweetener_04 50 50 .075
		Sound FF_WoodHollow_Sweetener_05 50 50 .075
		Sound FF_WoodHollow_Sweetener_06 50 50 .075
	End
End

Condition
	TriggerBits	CARPET
	Event
	Type	Local
	At	Origin
		SoundNoRepeat 2
	Sound FF_Carpet_01 50 50 0.5
	Sound FF_Carpet_03 50 50 0.5
	Sound FF_Carpet_05 50 50 0.5
	Sound FF_Carpet_07 50 50 0.5
	Sound FF_Carpet_09 50 50 0.5
	End
End

Condition
	TriggerBits	CARPET
	Event
	Type	Local
	At	Origin
		SoundNoRepeat 2
	Sound FF_FemTile_01 50 50 0.5
	Sound FF_FemTile_03 50 50 0.5
	Sound FF_FemTile_05 50 50 0.5
	Sound FF_FemTile_07 50 50 0.5
	Sound FF_FemTile_09 50 50 0.5
	End
End

Condition
	TriggerBits	TILE
	Event
	Type	Local
	At	Origin
		SoundNoRepeat 2
	Sound FF_FemTile_01 50 50 0.5
	Sound FF_FemTile_03 50 50 0.5
	Sound FF_FemTile_05 50 50 0.5
	Sound FF_FemTile_07 50 50 0.5
	Sound FF_FemTile_09 50 50 0.5
	End
End

Condition
	TriggerBits	SNOW
	Event
	Type	Local
	At	Origin
		SoundNoRepeat 2
	Sound FF_Snow_01 50 50 0.2
	Sound FF_Snow_03 50 50 0.2
	Sound FF_Snow_05 50 50 0.2
	Sound FF_Snow_07 50 50 0.2
	Sound FF_Snow_09 50 50 0.2
	End
End