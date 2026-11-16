#########################################################			
## 	Huge Left Foot		
#########################################################			

FxInfo			

Input			
	InpName Origin		
End			

LifeSpan 1			

Condition			
	TriggerBits	CONCRETE	
		Chance 0.6
	Event		
		Type	Local
		At	Origin
		SoundNoRepeat 2
		Sound FF_Sidewalk_01 50 50 0.4
		Sound FF_Sidewalk_03 50 50 0.4
		Sound FF_Sidewalk_05 50 50 0.4
		Sound FF_Sidewalk_07 50 50 0.4
		Sound FF_Sidewalk_09 50 50 0.4
	End					
	Event					
		Type	Local			
		At	origin			
		Sound FF_DemonSweet_01 50 50 0.133333333
		Sound FF_DemonSweet_02 50 50 0.133333333
	End					
End						
						
Condition						
	TriggerBits	ASPHALT	
	Chance 0.6	
	Event					
		Type	Local			
		At	Origin
		SoundNoRepeat 2
		Sound FF_Street_01 50 50 0.333333333
		Sound FF_Street_03 50 50 0.333333333
		Sound FF_Street_05 50 50 0.333333333
		Sound FF_Street_07 50 50 0.333333333
		Sound FF_Street_09 50 50 0.333333333
	End					
	Event					
		Type	Local			
		At	origin			
		Sound FF_DemonSweet_01 50 50 0.133333333
		Sound FF_DemonSweet_02 50 50 0.133333333
	End					
End						
						
Condition						
	TriggerBits	GRASS	
	Chance 0.6	
	Event					
		Type	Local			
		At	Origin
		SoundNoRepeat 2
		Sound FF_Grass_01 50 50 0.133333333
		Sound FF_Grass_03 50 50 0.133333333
		Sound FF_Grass_05 50 50 0.133333333
		Sound FF_Grass_07 50 50 0.133333333
		Sound FF_Grass_09 50 50 0.133333333
	End					
	Event					
		Type	Local			
		At	origin			
		Sound FF_DemonSweet_01 50 50 0.133333333
		Sound FF_DemonSweet_02 50 50 0.133333333
	End					
End						
						
Condition						
	TriggerBits	DIRT	
	Chance 0.6	
	Event					
		Type	Local			
		At	Origin
		SoundNoRepeat 2
		Sound FF_Dirt_01 50 50 0.133333333
		Sound FF_Dirt_03 50 50 0.133333333
		Sound FF_Dirt_05 50 50 0.133333333
		Sound FF_Dirt_07 50 50 0.133333333
		Sound FF_Dirt_09 50 50 0.133333333
	End					
	Event					
		Type	Local			
		At	origin			
		Sound FF_DemonSweet_01 50 50 0.133333333
		Sound FF_DemonSweet_02 50 50 0.133333333
	End					
End						
						
Condition						
	TriggerBits	SAND	
	Chance 0.6	
	Event					
		Type	Local			
		At	Origin
		SoundNoRepeat 2
		Sound FF_Sand_01 50 50 0.133333333
		Sound FF_Sand_03 50 50 0.133333333
		Sound FF_Sand_05 50 50 0.133333333
		Sound FF_Sand_07 50 50 0.133333333
		Sound FF_Sand_09 50 50 0.133333333
	End					
	Event					
		Type	Local			
		At	origin			
		Sound FF_DemonSweet_01 50 50 0.133333333
		Sound FF_DemonSweet_02 50 50 0.133333333
	End					
End						
						
Condition						
	TriggerBits	GRAVEL	
	Chance 0.6	
	Event					
		Type	Local			
		At	Origin
		SoundNoRepeat 2
		Sound FF_Gravel_01 50 50 0.333333333
		Sound FF_Gravel_03 50 50 0.333333333
		Sound FF_Gravel_05 50 50 0.333333333
		Sound FF_Gravel_07 50 50 0.333333333
		Sound FF_Gravel_09 50 50 0.333333333
	End					
	Event					
		Type	Local			
		At	origin			
		Sound FF_DemonSweet_01 50 50 0.106666667
		Sound FF_DemonSweet_02 50 50 0.106666667
	End					
End						
						
Condition						
	TriggerBits	PUDDLE	
	Chance 0.6	
	Event					
		Type	Local			
		At	Origin
		SoundNoRepeat 2
		Sound FF_Puddle_01 50 50 0.266666667
		Sound FF_Puddle_03 50 50 0.266666667
		Sound FF_Puddle_05 50 50 0.266666667
		Sound FF_Puddle_07 50 50 0.266666667
		Sound FF_Puddle_09 50 50 0.266666667
	End					
	Event					
		Type	Local			
		At	origin			
		Sound FF_DemonSweet_01 50 50 0.106666667
		Sound FF_DemonSweet_02 50 50 0.106666667
	End					
End						
Condition						
	TriggerBits PUDDLE					
	Chance	0.1				
	Event					
		Type	Local			
		At 		Origin
		SoundNoRepeat 2
		Sound WaterExit1 50 50 0.133333333
		Sound WaterExit2 50 50 0.133333333
		Sound WaterExit3 50 50 0.133333333
		Sound WaterExit4 50 50 0.133333333
		Sound WaterExit5 50 50 0.133333333
	End					
End						
						
Condition						
	TriggerBits	INWATER	
	Chance 0.6	
	Event					
		Type	Local			
		At	Origin	
		SoundNoRepeat 2
		Sound FF_Water_01 50 50 0.333333333
		Sound FF_Water_03 50 50 0.333333333
		Sound FF_Water_05 50 50 0.333333333
		Sound FF_Water_07 50 50 0.333333333
		Sound FF_Water_09 50 50 0.333333333
	End					
End						
						
Condition						
	TriggerBits	CATWALK		
	Chance 0.6	
	Event					
		Type	Local			
		At	Origin	
		SoundNoRepeat 2
		Sound FF_Catwalk_01 50 50 0.266666667
		Sound FF_Catwalk_03 50 50 0.266666667
		Sound FF_Catwalk_05 50 50 0.266666667
		Sound FF_Catwalk_07 50 50 0.266666667
		Sound FF_Catwalk_09 50 50 0.266666667
	End					
	Event					
		Type	Local			
		At	origin			
		Sound FF_DemonSweet_01 50 50 0.133333333
		Sound FF_DemonSweet_02 50 50 0.133333333
	End					
End						
						
Condition						
	TriggerBits	METALPLATE	
	Chance 0.6	
	Event					
		Type	Local			
		At	Origin	
		SoundNoRepeat 2
		Sound FF_MetalPlate_01 50 50 0.466666667
		Sound FF_MetalPlate_03 50 50 0.466666667
		Sound FF_MetalPlate_05 50 50 0.466666667
		Sound FF_MetalPlate_07 50 50 0.466666667
		Sound FF_MetalPlate_09 50 50 0.466666667
	End					
	Event					
		Type	Local			
		At	origin			
		Sound FF_DemonSweet_01 50 50 0.133333333
		Sound FF_DemonSweet_02 50 50 0.133333333
	End					
End						
						
Condition						
	TriggerBits	METAL	
	Chance 0.6	
	Event					
		Type	Local			
		At	Origin	
		SoundNoRepeat 2
		Sound FF_Metal_01 50 50 0.333333333
		Sound FF_Metal_03 50 50 0.333333333
		Sound FF_Metal_05 50 50 0.333333333
		Sound FF_Metal_07 50 50 0.333333333
		Sound FF_Metal_09 50 50 0.333333333
	End					
	Event					
		Type	Local			
		At	origin			
		Sound FF_DemonSweet_01 50 50 0.266666667
		Sound FF_DemonSweet_02 50 50 0.266666667
	End					
End						
Condition						
	TriggerBits METAL					
	Chance	0.075				
	Event					
		Type	Local			
		At 		Origin
		SoundNoRepeat 1
		Sound FF_Metal_Sweetener_01 50 50 0.133333333
		Sound FF_Metal_Sweetener_02 50 50 0.133333333
		Sound FF_Metal_Sweetener_03 50 50 0.133333333
		Sound FF_Metal_Sweetener_04 50 50 0.133333333
	End					
End						
						
Condition						
	TriggerBits	WOOD	
	Chance 0.6	
	Event					
		Type	Local			
		At	Origin
		SoundNoRepeat 2
		Sound FF_Wood_01 50 50 0.466666667
		Sound FF_Wood_03 50 50 0.466666667
		Sound FF_Wood_05 50 50 0.466666667
		Sound FF_Wood_07 50 50 0.466666667
		Sound FF_Wood_09 50 50 0.466666667
	End					
	Event					
		Type	Local			
		At	origin			
		Sound FF_DemonSweet_01 50 50 0.133333333
		Sound FF_DemonSweet_02 50 50 0.133333333
	End					
End						
						
Condition						
	TriggerBits	WOODHOLLOW		
	Chance 0.6	
	Event					
		Type	Local			
		At	Origin	
		SoundNoRepeat 2
		Sound FF_WoodHollow_01 50 50 0.266666667
		Sound FF_WoodHollow_03 50 50 0.266666667
		Sound FF_WoodHollow_05 50 50 0.266666667
		Sound FF_WoodHollow_07 50 50 0.266666667
		Sound FF_WoodHollow_09 50 50 0.266666667
	End					
	Event					
		Type	Local			
		At	origin			
		Sound FF_DemonSweet_01 50 50 0.266666667
		Sound FF_DemonSweet_02 50 50 0.266666667
	End					
End						
Condition						
	TriggerBits WOODHOLLOW					
	Chance	0.075				
	Event					
		Type	Local			
		At 		Origin	
		SoundNoRepeat 2
		Sound FF_WoodHollow_Sweetener_01 50 50 0.133333333
		Sound FF_WoodHollow_Sweetener_02 50 50 0.133333333
		Sound FF_WoodHollow_Sweetener_03 50 50 0.133333333
		Sound FF_WoodHollow_Sweetener_04 50 50 0.133333333
		Sound FF_WoodHollow_Sweetener_05 50 50 0.133333333
		Sound FF_WoodHollow_Sweetener_06 50 50 0.133333333
	End					
End						
						
Condition						
	TriggerBits	CARPET		
	Chance 0.6	
	Event					
		Type	Local			
		At	Origin	
		SoundNoRepeat 2
		Sound FF_Carpet_01 50 50 0.333333333
		Sound FF_Carpet_03 50 50 0.333333333
		Sound FF_Carpet_05 50 50 0.333333333
		Sound FF_Carpet_07 50 50 0.333333333
		Sound FF_Carpet_09 50 50 0.333333333
	End					
	Event					
		Type	Local			
		At	origin			
		Sound FF_DemonSweet_01 50 50 0.133333333
		Sound FF_DemonSweet_02 50 50 0.133333333
	End					
End						
						
Condition						
	TriggerBits	TILE	
	Chance 0.6	
	Event					
		Type	Local			
		At	Origin
		SoundNoRepeat 2
		Sound FF_Tile_01 50 50 0.4
		Sound FF_Tile_03 50 50 0.4
		Sound FF_Tile_05 50 50 0.4
		Sound FF_Tile_07 50 50 0.4
		Sound FF_Tile_09 50 50 0.4
	End					
	Event					
		Type	Local			
		At	origin			
		Sound FF_DemonSweet_01 50 50 0.133333333
		Sound FF_DemonSweet_02 50 50 0.133333333
	End					
End						
						
Condition						
	TriggerBits	SNOW	
	Chance 0.6	
	Event					
		Type	Local			
		At	Origin
		SoundNoRepeat 2
		Sound FF_Snow_01 50 50 0.133333333
		Sound FF_Snow_03 50 50 0.133333333
		Sound FF_Snow_05 50 50 0.133333333
		Sound FF_Snow_07 50 50 0.133333333
		Sound FF_Snow_09 50 50 0.133333333
	End					
	Event					
		Type	Local			
		At	origin			
		Sound FF_DemonSweet_01 50 50 0.106666667
		Sound FF_DemonSweet_02 50 50 0.106666667
	End					
End						
