#########################################################
## 	Huge Right Foot
#########################################################

FxInfo

Input
	InpName Root
End

LifeSpan 1

Condition
	TriggerBits	CONCRETE
	Event
		Type	Local
		At	Root
		SoundNoRepeat 2
		Sound FF_Sidewalk_02 50 50 .3
		Sound FF_Sidewalk_04 50 50 .3
		Sound FF_Sidewalk_06 50 50 .3
		Sound FF_Sidewalk_08 50 50 .3
		Sound FF_Sidewalk_10 50 50 .3
	End
	Event
		Type	Local
		At	origin
		Sound FF_HugeSweet_01 50 50 .1
		Sound FF_HugeSweet_02 50 50 .1
	End
End

Condition
	TriggerBits	ASPHALT
	Event
		Type	Local
		At	Root
		SoundNoRepeat 2
		Sound FF_Street_02 50 50 .25
		Sound FF_Street_04 50 50 .25
		Sound FF_Street_06 50 50 .25
		Sound FF_Street_08 50 50 .25
		Sound FF_Street_10 50 50 .25
	End
	Event
		Type	Local
		At	origin
		Sound FF_HugeSweet_01 50 50 .1
		Sound FF_HugeSweet_02 50 50 .1
	End
End

Condition
	TriggerBits	GRASS
	Event
		Type	Local
		At	Root
		SoundNoRepeat 2
		Sound FF_Grass_02 50 50 .1
		Sound FF_Grass_04 50 50 .1
		Sound FF_Grass_06 50 50 .1
		Sound FF_Grass_08 50 50 .1
		Sound FF_Grass_10 50 50 .1
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
		At	Root
		SoundNoRepeat 2
		Sound FF_Dirt_02 50 50 .1
		Sound FF_Dirt_04 50 50 .1
		Sound FF_Dirt_06 50 50 .1
		Sound FF_Dirt_08 50 50 .1
		Sound FF_Dirt_10 50 50 .1
	End
	Event
		Type	Local
		At	origin
		Sound FF_HugeSweet_01 50 50 .1
		Sound FF_HugeSweet_02 50 50 .1
	End
End

Condition
	TriggerBits	SAND
	Event
		Type	Local
		At	Root
		SoundNoRepeat 2
		Sound FF_Sand_02 50 50 .1
		Sound FF_Sand_04 50 50 .1
		Sound FF_Sand_06 50 50 .1
		Sound FF_Sand_08 50 50 .1
		Sound FF_Sand_10 50 50 .1
	End
	Event
		Type	Local
		At	origin
		Sound FF_HugeSweet_01 50 50 .1
		Sound FF_HugeSweet_02 50 50 .1
	End
End

Condition
	TriggerBits	GRAVEL
	Event
		Type	Local
		At	Root
		SoundNoRepeat 2
		Sound FF_Gravel_02 50 50 .25
		Sound FF_Gravel_04 50 50 .25
		Sound FF_Gravel_06 50 50 .25
		Sound FF_Gravel_08 50 50 .25
		Sound FF_Gravel_10 50 50 .25
	End
	Event
		Type	Local
		At	origin
		Sound FF_HugeSweet_01 50 50 .08
		Sound FF_HugeSweet_02 50 50 .08
	End
End

Condition
	TriggerBits	PUDDLE
	Event
		Type	Local
		At	Root
		SoundNoRepeat 2
		Sound FF_Puddle_02 50 50 .1
		Sound FF_Puddle_04 50 50 .1
		Sound FF_Puddle_06 50 50 .1
		Sound FF_Puddle_08 50 50 .1
		Sound FF_Puddle_10 50 50 .1
	End
	Event
		Type	Local
		At	origin
		Sound FF_HugeSweet_01 50 50 .08
		Sound FF_HugeSweet_02 50 50 .08
	End
End
Condition
	TriggerBits PUDDLE
	Chance	0.1
	Event
		Type	Local
		At 		Root
		SoundNoRepeat 2
		Sound WaterExit1 50 50 .05
		Sound WaterExit2 50 50 .05
		Sound WaterExit3 50 50 .05
		Sound WaterExit4 50 50 .05
		Sound WaterExit5 50 50 .05
	End
End

Condition
	TriggerBits	INWATER
	Event
		Type	Local
		At	Root
		SoundNoRepeat 2
		Sound FF_Water_02 50 50 .25
		Sound FF_Water_04 50 50 .25
		Sound FF_Water_06 50 50 .25
		Sound FF_Water_08 50 50 .25
		Sound FF_Water_10 50 50 .25
	End
End

Condition
	TriggerBits	CATWALK
	Event
		Type	Local
		At	Root
		SoundNoRepeat 2
		Sound FF_Catwalk_02 50 50 .14
		Sound FF_Catwalk_04 50 50 .14
		Sound FF_Catwalk_06 50 50 .14
		Sound FF_Catwalk_08 50 50 .14
		Sound FF_Catwalk_10 50 50 .14
	End
	Event
		Type	Local
		At	origin
		Sound FF_HugeSweet_01 50 50 .1
		Sound FF_HugeSweet_02 50 50 .1
	End
End

Condition
	TriggerBits	METALPLATE
	Event
		Type	Local
		At	Root
		SoundNoRepeat 2
		Sound FF_MetalPlate_02 50 50 .23
		Sound FF_MetalPlate_04 50 50 .23
		Sound FF_MetalPlate_06 50 50 .23
		Sound FF_MetalPlate_08 50 50 .23
		Sound FF_MetalPlate_10 50 50 .23
	End
	Event
		Type	Local
		At	origin
		Sound FF_HugeSweet_01 50 50 .1
		Sound FF_HugeSweet_02 50 50 .1
	End
End

Condition
	TriggerBits	METAL
	Event
		Type	Local
		At	Root
		SoundNoRepeat 2
		Sound FF_Metal_02 50 50 .13
		Sound FF_Metal_04 50 50 .13
		Sound FF_Metal_06 50 50 .13
		Sound FF_Metal_08 50 50 .13
		Sound FF_Metal_10 50 50 .13
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
		At	Root
		SoundNoRepeat 2
		Sound FF_Wood_02 50 50 .35
		Sound FF_Wood_04 50 50 .35
		Sound FF_Wood_06 50 50 .35
		Sound FF_Wood_08 50 50 .35
		Sound FF_Wood_10 50 50 .35
	End
	Event
		Type	Local
		At	origin
		Sound FF_HugeSweet_01 50 50 .1
		Sound FF_HugeSweet_02 50 50 .1
	End
End

Condition
	TriggerBits	WOODHOLLOW
	Event
		Type	Local
		At	Origin
		SoundNoRepeat 2
		Sound FF_WoodHollow_01 50 50 .2
		Sound FF_WoodHollow_03 50 50 .2
		Sound FF_WoodHollow_05 50 50 .2
		Sound FF_WoodHollow_07 50 50 .2
		Sound FF_WoodHollow_09 50 50 .2
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
		At	Root
		SoundNoRepeat 2
		Sound FF_Carpet_02 50 50 .25
		Sound FF_Carpet_04 50 50 .25
		Sound FF_Carpet_06 50 50 .25
		Sound FF_Carpet_08 50 50 .25
		Sound FF_Carpet_10 50 50 .25
	End
	Event
		Type	Local
		At	origin
		Sound FF_HugeSweet_01 50 50 .1
		Sound FF_HugeSweet_02 50 50 .1
	End
End

Condition
	TriggerBits	TILE
	Event
		Type	Local
		At	Root
		SoundNoRepeat 2
		Sound FF_Tile_02 50 50 .25
		Sound FF_Tile_04 50 50 .25
		Sound FF_Tile_06 50 50 .25
		Sound FF_Tile_08 50 50 .25
		Sound FF_Tile_10 50 50 .25
	End
	Event
		Type	Local
		At	origin
		Sound FF_HugeSweet_01 50 50 .1
		Sound FF_HugeSweet_02 50 50 .1
	End
End

Condition
	TriggerBits	TILE
	Event
		Type	Local
		At	Root
		SoundNoRepeat 2
		Sound FF_Tile_02 50 50 .2
		Sound FF_Tile_04 50 50 .2
		Sound FF_Tile_06 50 50 .2
		Sound FF_Tile_08 50 50 .2
		Sound FF_Tile_10 50 50 .2
	End
	Event
		Type	Local
		At	origin
		Sound FF_HugeSweet_01 50 50 .1
		Sound FF_HugeSweet_02 50 50 .1
	End
End

Condition
	TriggerBits	SNOW
	Event
		Type	Local
		At	Root
		SoundNoRepeat 2
		Sound FF_Snow_02 50 50 .1
		Sound FF_Snow_04 50 50 .1
		Sound FF_Snow_06 50 50 .1
		Sound FF_Snow_08 50 50 .1
		Sound FF_Snow_10 50 50 .1
	End
	Event
		Type	Local
		At	origin
		Sound FF_HugeSweet_01 50 50 .08
		Sound FF_HugeSweet_02 50 50 .08
	End
End