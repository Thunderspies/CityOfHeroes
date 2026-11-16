#########################################################
## 	Huge Stops
#########################################################

FxInfo

Input
	InpName Origin
End

LifeSpan 1

Condition
	TriggerBits	CONCRETE
	Event
		Type	Local
		At	Origin
		SoundNoRepeat 2
		Sound FF_Sidewalk_01 50 50 .3
		Sound FF_Sidewalk_03 50 50 .3
		Sound FF_Sidewalk_05 50 50 .3
		Sound FF_Sidewalk_07 50 50 .3
		Sound FF_Sidewalk_09 50 50 .3
	End
	Event
		Type	Local
		At	origin
		Sound FF_HugeSweet_01 50 50 .15
		Sound FF_HugeSweet_02 50 50 .15
	End
End

Condition
	TriggerBits	ASPHALT
	Event
		Type	Local
		At	Origin
		SoundNoRepeat 2
		Sound FF_Street_01 50 50 .25
		Sound FF_Street_03 50 50 .25
		Sound FF_Street_05 50 50 .25
		Sound FF_Street_07 50 50 .25
		Sound FF_Street_09 50 50 .25
	End
	Event
		Type	Local
		At	origin
		Sound FF_HugeSweet_01 50 50 .15
		Sound FF_HugeSweet_02 50 50 .15
	End
End

Condition
	TriggerBits	GRASS
	Event
		Type	Local
		At	Origin
		SoundNoRepeat 2
		Sound FF_Grass_01 50 50 .1
		Sound FF_Grass_03 50 50 .1
		Sound FF_Grass_05 50 50 .1
		Sound FF_Grass_07 50 50 .1
		Sound FF_Grass_09 50 50 .1
	End
	Event
		Type	Local
		At	origin
		Sound FF_HugeSweet_01 50 50 .1
		Sound FF_HugeSweet_02 50 50 .1
	End
End

Condition
	TriggerBits	DIRT
	Event
		Type	Local
		At	Origin
		SoundNoRepeat 2
		Sound FF_Dirt_01 50 50 .15
		Sound FF_Dirt_03 50 50 .15
		Sound FF_Dirt_05 50 50 .15
		Sound FF_Dirt_07 50 50 .15
		Sound FF_Dirt_09 50 50 .15
	End
	Event
		Type	Local
		At	origin
		Sound FF_HugeSweet_01 50 50 .15
		Sound FF_HugeSweet_02 50 50 .15
	End
End

Condition
	TriggerBits	SAND
	Event
		Type	Local
		At	Origin
		SoundNoRepeat 2
		Sound FF_Sand_01 50 50 .15
		Sound FF_Sand_03 50 50 .15
		Sound FF_Sand_05 50 50 .15
		Sound FF_Sand_07 50 50 .15
		Sound FF_Sand_09 50 50 .15
	End
	Event
		Type	Local
		At	origin
		Sound FF_HugeSweet_01 50 50 .15
		Sound FF_HugeSweet_02 50 50 .15
	End
End

Condition
	TriggerBits	GRAVEL
	Event
		Type	Local
		At	Origin
		SoundNoRepeat 2
		Sound FF_Gravel_01 50 50 .25
		Sound FF_Gravel_03 50 50 .25
		Sound FF_Gravel_05 50 50 .25
		Sound FF_Gravel_07 50 50 .25
		Sound FF_Gravel_09 50 50 .25
	End
	Event
		Type	Local
		At	origin
		Sound FF_HugeSweet_01 50 50 .15
		Sound FF_HugeSweet_02 50 50 .15
	End
End

Condition
	TriggerBits	PUDDLE
	Event
		Type	Local
		At	Origin
		SoundNoRepeat 2
		Sound FF_Puddle_01 50 50 .10
		Sound FF_Puddle_03 50 50 .10
		Sound FF_Puddle_05 50 50 .10
		Sound FF_Puddle_07 50 50 .10
		Sound FF_Puddle_09 50 50 .10
	End
End
Condition
	TriggerBits PUDDLE
	Chance	0.1
	Event
		Type	Local
		At 		Origin
		SoundNoRepeat 2
		Sound WaterExit1 50 50 .05
		Sound WaterExit2 50 50 .05
		Sound WaterExit3 50 50 .05
		Sound WaterExit4 50 50 .05
		Sound WaterExit5 50 50 .05
	End
	Event
		Type	Local
		At	origin
		Sound FF_HugeSweet_01 50 50 .15
		Sound FF_HugeSweet_02 50 50 .15
	End
End

Condition
	TriggerBits	INWATER
	Event
		Type	Local
		At	Origin
		SoundNoRepeat 2
		Sound FF_Water_01 50 50 .25
		Sound FF_Water_03 50 50 .25
		Sound FF_Water_05 50 50 .25
		Sound FF_Water_07 50 50 .25
		Sound FF_Water_09 50 50 .25
	End
End

Condition
	TriggerBits	CATWALK
	Event
		Type	Local
		At	Origin
		SoundNoRepeat 2
		Sound FF_Catwalk_01 50 50 .14
		Sound FF_Catwalk_03 50 50 .14
		Sound FF_Catwalk_05 50 50 .14
		Sound FF_Catwalk_07 50 50 .14
		Sound FF_Catwalk_09 50 50 .14
	End
	Event
		Type	Local
		At	origin
		Sound FF_HugeSweet_01 50 50 .2
		Sound FF_HugeSweet_02 50 50 .2
	End
End

Condition
	TriggerBits	METALPLATE
	Event
		Type	Local
		At	Origin
		SoundNoRepeat 2
		Sound FF_MetalPlate_01 50 50 .23
		Sound FF_MetalPlate_03 50 50 .23
		Sound FF_MetalPlate_05 50 50 .23
		Sound FF_MetalPlate_07 50 50 .23
		Sound FF_MetalPlate_09 50 50 .23
	End
	Event
		Type	Local
		At	origin
		Sound FF_HugeSweet_01 50 50 .2
		Sound FF_HugeSweet_02 50 50 .2
	End
End

Condition
	TriggerBits	METAL
	Event
		Type	Local
		At	Origin
		SoundNoRepeat 2
		Sound FF_Metal_01 50 50 .13
		Sound FF_Metal_03 50 50 .13
		Sound FF_Metal_05 50 50 .13
		Sound FF_Metal_07 50 50 .13
		Sound FF_Metal_09 50 50 .13
	End
	Event
		Type	Local
		At	origin
		Sound FF_HugeSweet_01 50 50 .2
		Sound FF_HugeSweet_02 50 50 .2
	End	
End

Condition
	TriggerBits	WOOD
	Event
		Type	Local
		At	Origin
		SoundNoRepeat 2
		Sound FF_Wood_01 50 50 .45
		Sound FF_Wood_03 50 50 .45
		Sound FF_Wood_05 50 50 .45
		Sound FF_Wood_07 50 50 .45
		Sound FF_Wood_09 50 50 .45
	End
	Event
		Type	Local
		At	origin
		Sound FF_HugeSweet_01 50 50 .2
		Sound FF_HugeSweet_02 50 50 .2
	End
End

Condition
	TriggerBits	WOODHOLLOW
	Event
		Type	Local
		At	Origin
		SoundNoRepeat 2
		Sound FF_WoodHollow_01 50 50 .45
		Sound FF_WoodHollow_03 50 50 .45
		Sound FF_WoodHollow_05 50 50 .45
		Sound FF_WoodHollow_07 50 50 .45
		Sound FF_WoodHollow_09 50 50 .45
	End
	Event
		Type	Local
		At	origin
		Sound FF_HugeSweet_01 50 50 .2
		Sound FF_HugeSweet_02 50 50 .2
	End
End

Condition
	TriggerBits	CARPET
	Event
		Type	Local
		At	Origin
		SoundNoRepeat 2
		Sound FF_Carpet_01 50 50 .25
		Sound FF_Carpet_03 50 50 .25
		Sound FF_Carpet_05 50 50 .25
		Sound FF_Carpet_07 50 50 .25
		Sound FF_Carpet_09 50 50 .25
	End
	Event
		Type	Local
		At	origin
		Sound FF_HugeSweet_01 50 50 .15
		Sound FF_HugeSweet_02 50 50 .15
	End
End

Condition
	TriggerBits	TILE
	Event
		Type	Local
		At	Origin
		SoundNoRepeat 2
		Sound FF_Tile_01 50 50 .25
		Sound FF_Tile_03 50 50 .25
		Sound FF_Tile_05 50 50 .25
		Sound FF_Tile_07 50 50 .25
		Sound FF_Tile_09 50 50 .25
	End
	Event
		Type	Local
		At	origin
		Sound FF_HugeSweet_01 50 50 .15
		Sound FF_HugeSweet_02 50 50 .15
	End
End

Condition
	TriggerBits	SNOW
	Event
		Type	Local
		At	Origin
		SoundNoRepeat 2
		Sound FF_Snow_01 50 50 .15
		Sound FF_Snow_03 50 50 .15
		Sound FF_Snow_05 50 50 .15
		Sound FF_Snow_07 50 50 .15
		Sound FF_Snow_09 50 50 .15
	End
	Event
		Type	Local
		At	origin
		Sound FF_HugeSweet_01 50 50 .15
		Sound FF_HugeSweet_02 50 50 .15
	End
End